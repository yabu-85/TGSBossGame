#include "BossObstacle.h"
#include "Engine/Model.h"
#include "Player.h"

BossObstacle::BossObstacle(GameObject* parent)
	:Obstacle(parent), pPlayer_(nullptr), stateEnter_(true), state_(S_ENTER), moveSpeed_(0.0f)
{
	objectName_ = "BossObstacle";
    MoveDirection_ = XMVectorSet(0, 0, 0, 0);
}

BossObstacle::~BossObstacle()
{
}

void BossObstacle::Initialize()
{
	//モデルロード
	hModel_ = Model::Load("Model/Boss_03.fbx");
	assert(hModel_ >= 0);

	pPlayer_ = (Player*)FindObject("Player");
}

void BossObstacle::Update()
{
	if (!active_) return;

    switch (state_) {
    case STATE::S_ENTER:
        UpdateEnter();
        break;
    case STATE::S_DEAD:
        UpdateDead();
        break;
    }


}

void BossObstacle::Draw()
{
	if (!isDraw_) return;

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_, 0);
}

void BossObstacle::Release()
{
}

//--------------state---------------//

void BossObstacle::UpdateEnter()
{
	if (stateEnter_) {
        stateEnter_ = false;
        transform_.position_.z -= 100.0f;
		transform_.position_.y = 2.0f;
        transform_.rotate_.y = 0;
	}

    if (!IsInTargetPosition()) {
        targetPosition_ = pPlayer_->GetPosition();
        targetPosition_.y = 5.0f;
        targetPosition_.z += 30.0f;
        moveSpeed_ = 0.6f;
        Move();

        return;
    }

    ChangeState(S_DEAD);

}

void BossObstacle::UpdateDead()
{
    transform_.position_.z = pPlayer_->GetPosition().z + 30.0f;

    XMFLOAT3 plaPos = pPlayer_->GetPosition();
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vTar = XMLoadFloat3(&plaPos);
    XMVECTOR vVec = vTar - vPos;
    XMFLOAT3 f;
    XMStoreFloat3(&f, vVec);

    Rotate(f.x, f.z, 5.0f);
}

void BossObstacle::ChangeState(STATE s)
{
	state_ = s;
	stateEnter_ = true;
}

//--------------private-----------------//

void BossObstacle::Rotate(float x, float z, float _rotateSpeed)
{
    float tx = transform_.position_.x + x;
    float tz = transform_.position_.z + z;

    const XMVECTOR vFront{ 0, 0, 1, 0 };
    XMFLOAT3 fAimPos = XMFLOAT3(transform_.position_.x - tx, 0, transform_.position_.z - tz);
    XMVECTOR vAimPos = XMLoadFloat3(&fAimPos);
    vAimPos = XMVector3Normalize(vAimPos);
    XMVECTOR vDot = XMVector3Dot(vFront, vAimPos);
    float dot = XMVectorGetX(vDot);
    float angle = (float)acos(dot);

    // 外積を求めて半回転だったら angle に -1 を掛ける
    XMVECTOR vCross = XMVector3Cross(vFront, vAimPos);
    if (XMVectorGetY(vCross) < 0) {
        angle *= -1;
    }

    // 目標の回転角度を設定
    targetRotation_ = XMConvertToDegrees(angle);

    // 回転角度をスムーズに変更
    float rotationDiff = NormalizeAngle(targetRotation_ - (transform_.rotate_.y - 180.0f));  //Blender
    if (rotationDiff != 0) {
        if (_rotateSpeed > abs(rotationDiff)) {
            transform_.rotate_.y = targetRotation_;
            transform_.rotate_.y += 180.0f; //Blender
        }
        else {
            transform_.rotate_.y += _rotateSpeed * (rotationDiff > 0 ? 1 : -1);
        }
    }
}

float BossObstacle::NormalizeAngle(float angle)
{
    while (angle > 180.0f) {
        angle -= 360.0f;
    }
    while (angle < -180.0f) {
        angle += 360.0f;
    }
    return angle;
}

void BossObstacle::Move()
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vTar = XMLoadFloat3(&targetPosition_);
    XMVECTOR vVec = XMVector3Normalize(vTar - vPos) * 0.2;
    vVec = XMVector3Normalize(vVec + MoveDirection_);
    MoveDirection_ = vVec;
    XMStoreFloat3(&transform_.position_, vPos + (vVec * moveSpeed_) );

}

bool BossObstacle::IsInTargetPosition()
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vTar = XMLoadFloat3(&targetPosition_);
    float distance = XMVectorGetX(XMVector3Length(vTar - vPos));
    if (distance <= moveSpeed_) return true;

    return false;
}
