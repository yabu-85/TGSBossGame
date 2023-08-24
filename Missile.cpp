#include "Missile.h"
#include "Engine/Model.h"
#include "Player.h"
#include "Engine/VFX.h"

Missile::Missile(GameObject* parent)
	:GameObject(parent, "Missile"), hModel_(-1), position{0,0,0,0}, velocity{0,0,0,0}, target{0,0,0,0},maxCentripetalAccel(0),
    propulsion(0),countPerMeter(0),speed(0),damping(0),impact(0), pPlayer_(nullptr)
{
}

Missile::~Missile()
{
}

void Missile::Initialize()
{
	hModel_ = Model::Load("Missile.fbx");
	assert(hModel_ >= 0);
	transform_.position_.y = 1.0f;

    pPlayer_ = (Player*)FindObject("Player");
    XMFLOAT3 pPos = pPlayer_->GetPosition();
    pPos.z += 3.0f;
    target = XMLoadFloat3(&pPos);

    countPerMeter = 1.0f;
    speed = 0.05f;
    damping = 0.0f;
    impact = 0.5f;

    //円運動の向心力
    maxCentripetalAccel = 0.1f;

    //終端速度に到達するための加速度を計算します。
    //a = v / k なので、a = v * k
    propulsion = speed * damping;


   data.textureFileName = "cloudA.png";
    data.position = transform_.position_;
    data.delay = 0;
    data.number = 5;
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

    dataExp.textureFileName = "cloudA.png";
    dataExp.position = transform_.position_;
    dataExp.delay = 0;
    dataExp.number = 1;
    dataExp.lifeTime = 100;
    dataExp.positionRnd = XMFLOAT3(0.5, 0, 0.5);
    dataExp.direction = XMFLOAT3(0, 1, 0);
    dataExp.directionRnd = XMFLOAT3(90, 90, 90);
    dataExp.speed = 0.25f;
    dataExp.speedRnd = 1;
    dataExp.accel = 0.93;
    dataExp.size = XMFLOAT2(0.1, 0.1);
    dataExp.sizeRnd = XMFLOAT2(0.4, 0.4);
    dataExp.scale = XMFLOAT2(0.99, 0.99);
    dataExp.color = XMFLOAT4(1, 1, 0.1, 1);
    dataExp.deltaColor = XMFLOAT4(0, 0, 0, 0);
    dataExp.gravity = 0.003f;

}

void Missile::Update()
{
    /*
    // 角度をラジアンに変換
    float angleRad = (-transform_.rotate_.y + 90.0f) * XM_PI / 180.0f;

    // 角度から方向ベクトルを計算
    XMFLOAT3 direction = { cosf(angleRad), 0.0f, sinf(angleRad) };

    // 速度を乗算して位置を更新
    float speed = 0.5f; // 移動速度を調整
    transform_.position_.x += direction.x * speed;
    transform_.position_.z += direction.z * speed;
    */
    
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
    if (distance < impact) {
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


    XMFLOAT3 pPos = pPlayer_->GetPosition();
    pPos.y += 0.5f;
    distance = sqrt(
        (pPos.x - pos.x) * (pPos.x - pos.x) +
        (pPos.y - pos.y) * (pPos.y - pos.y) +
        (pPos.z - pos.z) * (pPos.z - pos.z)
    );


    //炎
    if (distance < 1.0f) {

        data.position = transform_.position_;
        VFX::Start(data);

        KillMe();
    }

    //火の粉
    dataExp.position = transform_.position_;
    VFX::Start(dataExp);

}

void Missile::Draw()
{
	Transform trs = transform_;
	trs.rotate_.y += 180.0f;
	Model::SetTransform(hModel_, trs);
	Model::Draw(hModel_);
}

void Missile::Release()
{
}

void Missile::SetTarget(float x, float y, float z)
{
    XMFLOAT3 t = { x,y,z };
    velocity = XMLoadFloat3(&t);
}
