#include "Missile.h"
#include "Engine/Model.h"
#include "Player.h"
#include "Engine/VFX.h"
#include "RobotObstacle.h"
#include "AudioManager.h"

#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}

Missile::Missile(GameObject* parent)
	:GameObject(parent, "Missile"), hModel_(-1), position{0,0,0,0}, velocity{0,0,0,0}, target{0,0,0,0},maxCentripetalAccel(0),
    propulsion(0),countPerMeter(0),speed(0),damping(0),impact(0), pPlayer_(nullptr), launchPoint_{0,0,0}, missileReflected_(false),
    rotationAngle_{0,0,0}, pRobotObstacle_(nullptr), isActive_(true)
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
    impact = 0.5f;

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
    dataExp_.positionRnd = XMFLOAT3(0.5, 0, 0.5);
    dataExp_.direction = XMFLOAT3(0, 1, 0);
    dataExp_.directionRnd = XMFLOAT3(90, 90, 90);
    dataExp_.speed = 0.1f;
    dataExp_.speedRnd = 1.0f;
    dataExp_.accel = 0.93;
    dataExp_.size = XMFLOAT2(0.4, 0.4);
    dataExp_.sizeRnd = XMFLOAT2(0.4, 0.4);
    dataExp_.scale = XMFLOAT2(0.9, 0.9);
    dataExp_.color = XMFLOAT4(1, 1, 0.1, 1);
    dataExp_.deltaColor = XMFLOAT4(0, 0, 0, 0);
    dataExp_.gravity = 0.003f;

}

void Missile::Update()
{
    if (isActive_ == false) return;

    //火の粉
    dataExp_.position = transform_.position_;
    VFX::Start(dataExp_);

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
        float dist = XMVectorGetX(XMVector3Length((pos2 + pos) - pos1));
        if (dist <= 1.0f) {
            CreateExplodeParticle();

            //まだ撃った親が生きていれば倒す
            if (pRobotObstacle_ != nullptr) {
                pRobotObstacle_->KillMeSub();
            }

            AudioManager::PlaySoundMa(AUDIO_ROBOT_HIT);
            Model::SetDraw(hModel_, false);
            KillMe();
        }

        return;
    }

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
    
    XMFLOAT3 tar;
    pos = transform_.position_;
    XMStoreFloat3(&tar, target);
    float distance = sqrt(
        (tar.x - pos.x) * (tar.x - pos.x) +
        (tar.y - pos.y) * (tar.y - pos.y) +
        (tar.z - pos.z) * (tar.z - pos.z)
    );
    //ここはTargetの場所の範囲に入ったら
    if (distance < impact) {
        CreateExplodeParticle();        
        AudioManager::PlaySoundMa(AUDIO_MISSILE_EXPLODE);
        
        if (!pRobotObstacle_->IsDead())
            pRobotObstacle_->NotifyMissileDestroyed(this);

        Model::SetDraw(hModel_, false);
        KillMe();

    }
    
    const XMVECTOR vFront{ 0, 0, 1, 0 };
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

    //こっちはプレイヤーとの範囲内か
    XMFLOAT3 pPos = pPlayer_->GetPosition();
    pPos.y += 0.5f;
    distance = sqrt(
        (pPos.x - pos.x) * (pPos.x - pos.x) +
        (pPos.y - pos.y) * (pPos.y - pos.y) +
        (pPos.z - pos.z) * (pPos.z - pos.z)
    );
    if (distance < 1.0f) {
        CreateExplodeParticle();
        AudioManager::PlaySoundMa(AUDIO_MISSILE_EXPLODE);

        pPlayer_->DecreaseHp(5);

        if (!pRobotObstacle_->IsDead())
            pRobotObstacle_->NotifyMissileDestroyed(this);

        Model::SetDraw(hModel_, false);
        KillMe();
    }
}

void Missile::Draw()
{
	Transform trs = transform_;
	trs.rotate_.y += 180.0f;
	Model::SetTransform(hModel_, trs);
	//Model::Draw(hModel_);
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

    float distance = sqrt(
        (pPos.x - pos.x) * (pPos.x - pos.x) +
        (pPos.y - pos.y) * (pPos.y - pos.y) +
        (pPos.z - pos.z) * (pPos.z - pos.z)
    );

    int i = 0;

    if (distance <= 7.0f) {
        missileReflected_ = true;
    }
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
    data.direction = XMFLOAT3(0, 1, 0);
    data.directionRnd = XMFLOAT3(90, 90, 90);
    data.speed = 0.1f;
    data.speedRnd = 0.8;
    data.size = XMFLOAT2(0.4, 0.4);
    data.sizeRnd = XMFLOAT2(0.4, 0.4);
    data.scale = XMFLOAT2(1.05, 1.05);
    data.color = XMFLOAT4(1, 1, 0.1, 1);
    data.deltaColor = XMFLOAT4(0, -1.0 / 20, 0, -1.0 / 20);
    VFX::Start(data);

}
