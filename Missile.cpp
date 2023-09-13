#include "Missile.h"
#include "Engine/Model.h"
#include "Player.h"
#include "Engine/VFX.h"
#include "RobotObstacle.h"
#include "AudioManager.h"

Missile::Missile(GameObject* parent)
	:GameObject(parent, "Missile"), hModel_(-1), position{0,0,0,0}, velocity{0,0,0,0}, target{0,0,0,0},maxCentripetalAccel(0),
    propulsion(0),countPerMeter(0),speed(0),damping(0),impact(0), pPlayer_(nullptr), launchPoint_{0,0,0}, missileReflected_(false),
    rotationAngle_{0,0,0}, isActive_(true), isTargetHit_(false), isExplode_(false)
{
}

Missile::~Missile()
{
}

void Missile::Initialize()
{
	hModel_ = Model::Load("Model/Missile.fbx", 0, 0);
	assert(hModel_ >= 0);

	transform_.position_.y = 1.0f;
    rotationAngle_ = { -(float)(rand() % 5), (float)(rand() % 10), (float)(rand() % 10) };

    pPlayer_ = (Player*)FindObject("Player");
    XMFLOAT3 pPos = pPlayer_->GetPosition();
    target = XMLoadFloat3(&pPos);

    countPerMeter = 1.0f;
    speed = 0.05f;
    damping = 0.0f;
    impact = 1.0f;

    //円運動の向心力
    maxCentripetalAccel = 0.1f;

    //終端速度に到達するための加速度の計算方法↓
    //a = v / k なので、a = v * k
    propulsion = speed * damping;
    
    //火の粉
    dataExp_.textureFileName = "Particle/missilePar.png";
    dataExp_.position = transform_.position_;
    dataExp_.delay = 0;
    dataExp_.number = 1;
    dataExp_.lifeTime = 60;
    dataExp_.positionRnd = XMFLOAT3(0.5f, 0.0f, 0.5f);
    dataExp_.direction = XMFLOAT3(0.0f, 1.0f, 0.0f);
    dataExp_.directionRnd = XMFLOAT3(90.0f, 90.0f, 90.0f);
    dataExp_.speed = 0.1f;
    dataExp_.speedRnd = 1.0f;
    dataExp_.accel = 0.93f;
    dataExp_.size = XMFLOAT2(0.4f, 0.4f);
    dataExp_.sizeRnd = XMFLOAT2(0.4f, 0.4f);
    dataExp_.scale = XMFLOAT2(0.9f, 0.9f);
    dataExp_.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
    dataExp_.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
    dataExp_.gravity = 0.003f;

}

void Missile::Update()
{
    if (isActive_ == false) return;

    //火の粉
    dataExp_.position = transform_.position_;
    //VFX::Start(dataExp_);

    //跳ね返されてる場合の処理
    if (missileReflected_) {
        float reflectSpeed = 1.0f;
        XMVECTOR pos1, pos2;
        pos1 = XMLoadFloat3(&launchPoint_);
        pos2 = XMLoadFloat3(&transform_.position_);
        XMVECTOR pos = pos1 - pos2;
        pos = XMVector3Normalize(pos) * reflectSpeed;
        XMFLOAT3 posF;
        XMStoreFloat3(&posF, pos);
        transform_.position_ = { transform_.position_.x + posF.x, transform_.position_.y + posF.y, transform_.position_.z + posF.z };

        transform_.rotate_.x += rotationAngle_.x;
        transform_.rotate_.y += rotationAngle_.y;
        transform_.rotate_.z += rotationAngle_.z;

        //爆発の範囲内かどうか
        float distance = XMVectorGetX(XMVector3Length((pos2 + pos) - pos1));
        if (distance <= 1.0f) {
            CreateExplodeParticle();
            isTargetHit_ = true;
            AudioManager::PlaySoundMa(AUDIO_ROBOT_HIT);
        }

        return;
    }

    //移動ーーーーーーーーーーーーーーーーーーーー
    position = XMLoadFloat3(&transform_.position_);
    XMVECTOR toTarget = target - position;
    XMVECTOR vn = XMVector3Normalize(velocity);
    
    XMFLOAT3 vec1, vec2;
    XMStoreFloat3(&vec1, toTarget);
    XMStoreFloat3(&vec2, vn);
    float dot = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;

    XMVECTOR centripetalAccel = toTarget - (vn * dot);
    float centripetalAccelMagnitude = XMVectorGetX(XMVector3Length(centripetalAccel));
    if (centripetalAccelMagnitude > 1.0f)
    {
        centripetalAccel /= centripetalAccelMagnitude;
    }
    XMVECTOR force = centripetalAccel * maxCentripetalAccel;
    force += vn * propulsion;
    force -= velocity * damping;
    velocity += force;
    position += velocity;

    XMFLOAT3 pos;
    XMStoreFloat3(&pos, position);
    transform_.position_ = pos;

    //回転ーーーーーーーーーーーーーーーーーーーーーー
    const XMVECTOR vFront{ 0, 0, 1, 0 };
    XMFLOAT3 tar;
    XMStoreFloat3(&tar, target);
    XMFLOAT3 fAimPos = XMFLOAT3(transform_.position_.x - tar.x, 0, transform_.position_.z - tar.z);
    XMVECTOR vAimPos = XMLoadFloat3(&fAimPos);
    vAimPos = XMVector3Normalize(vAimPos);
    XMVECTOR vDot = XMVector3Dot(vFront, vAimPos);
    dot = XMVectorGetX(vDot);
    float angle = (float)acos(dot);
    // 外積を求めて半回転だったら angle に -1 を掛ける
    XMVECTOR vCross = XMVector3Cross(vFront, vAimPos);
    if (XMVectorGetY(vCross) < 0) {
        angle *= -1;
    }
    transform_.rotate_.y = XMConvertToDegrees(angle);
    transform_.rotate_.y += 180.0f; //Blender

    //////////////////////

    //もう爆発してるから判定しない
    if (isExplode_ || isTargetHit_) return;
    
    pos = transform_.position_;
    float distance = (float)sqrt(
        (tar.x - pos.x) * (tar.x - pos.x) +
        (tar.y - pos.y) * (tar.y - pos.y) +
        (tar.z - pos.z) * (tar.z - pos.z)
    );
    //ここはTargetの場所の範囲に入ったら
    if (distance < 1.0f) {
        CreateExplodeParticle();        
        AudioManager::PlaySoundMa(AUDIO_MISSILE_EXPLODE);
        isExplode_ = true;

    }
    
    //こっちはプレイヤーとの範囲内か
    XMFLOAT3 pPos = pPlayer_->GetPosition();
    pPos.y += 0.5f;
    distance = (float)sqrt(
        (pPos.x - pos.x) * (pPos.x - pos.x) +
        (pPos.y - pos.y) * (pPos.y - pos.y) +
        (pPos.z - pos.z) * (pPos.z - pos.z)
    );
    if (distance < 1.0f) {
        CreateExplodeParticle();
        AudioManager::PlaySoundMa(AUDIO_MISSILE_EXPLODE);

        pPlayer_->DecreaseHp(5);
        isExplode_ = true;

    }
}

void Missile::Draw()
{
	Transform trs = transform_;
	trs.rotate_.y += 180.0f;
	Model::SetTransform(hModel_, trs);
}

void Missile::Release()
{
}

void Missile::SetTarget(float x, float y, float z)
{
    XMFLOAT3 t = { x,y,z };
    velocity = XMLoadFloat3(&t);
    launchPoint_ = transform_.position_;

}

void Missile::Reflect()
{
    XMFLOAT3 pPos = pPlayer_->GetPosition();
    pPos.y += 0.5f;
    XMFLOAT3 pos = transform_.position_;

    float distance = (float)sqrt(
        (pPos.x - pos.x) * (pPos.x - pos.x) +
        (pPos.y - pos.y) * (pPos.y - pos.y) +
        (pPos.z - pos.z) * (pPos.z - pos.z)
    );

    int i = 0;

    if (distance <= 7.0f) {
        missileReflected_ = true;
    }
}

void Missile::KillMeSub()
{
    Model::StopDraw(hModel_);
    KillMe();
}


void Missile::CreateExplodeParticle()
{
    //爆発
    EmitterData data;
    data.position = transform_.position_;
    data.textureFileName = "Particle/missilePar.png";
    data.position = transform_.position_;
    data.delay = 0;
    data.number = 3;
    data.lifeTime = 30;
    data.direction = XMFLOAT3(0.0f, 1.0f, 0.0f);
    data.directionRnd = XMFLOAT3(90.0f, 90.0f, 90.0f);
    data.speed = 0.1f;
    data.speedRnd = 0.8f;
    data.size = XMFLOAT2(0.4f, 0.4f);
    data.sizeRnd = XMFLOAT2(0.4f, 0.4f);
    data.scale = XMFLOAT2(1.05f, 1.05f);
    data.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
    data.deltaColor = XMFLOAT4(0.0f, -1.0f / 20.0f, 0.0f, -1.0f / 20.0f);
    //VFX::Start(data);

}
