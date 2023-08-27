#include "UfoObstacle.h"
#include "Engine/SphereCollider.h"
#include "Engine/Model.h"
#include "Engine/VFX.h"
#include "Engine/Direct3D.h"

UfoObstacle::UfoObstacle(GameObject* parent)
	:Obstacle(parent), hModelLa_(-1), state_(S_ENTER), stateEnter_(true), targetPos_(0,0,0), moveSpeed_(0.0f), time_(0)
{
	objectName_ = "UfoObstacle";
}

UfoObstacle::~UfoObstacle()
{
}

void UfoObstacle::Initialize()
{
	//モデルロード
	hModel_ = Model::Load("Ufo.fbx");
	assert(hModel_ >= 0);

	//モデルロード
	hModelLa_ = Model::Load("Laser.fbx");
	assert(hModelLa_ >= 0);

	moveSpeed_ = 1.0f;

}

void UfoObstacle::Update()
{
	if (!active_) return;

	switch (state_) {
	case STATE::S_ENTER:
		UpdateEnter();
		break;
	case STATE::S_CHARGING:
		UpdateCharging();
		break;
	case STATE::S_SHOT:
		UpdateShot();
		break;
	case STATE::S_IDLE:
		UpdateIdle();
		break;
	case STATE::S_LEAVING:
		UpdateLeaving();
		break;
	}

}

void UfoObstacle::Draw()
{
	if (!active_) return;


	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);


	if (state_ == S_SHOT) {
		Transform pos = transform_;
		pos.position_.y -= 0.5f;
		Model::SetTransform(hModelLa_, pos);
		Model::Draw(hModelLa_, 4);

	}
}

void UfoObstacle::Release()
{
}

void UfoObstacle::UpdateEnter()
{
	if (stateEnter_) {
		stateEnter_ = false;

		targetPos_ = transform_.position_;
		targetPos_.y = 10.0f;

		transform_.position_.y = 10.0f;
		transform_.position_.x += (float)(rand() & 5 - 2);
		transform_.position_.y += (float)(rand() & 3 -3);
		transform_.position_.z += -5.0f + (float)(rand() & 5 - 1);
	}

	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vTar = XMLoadFloat3(&targetPos_);
	XMVECTOR vMovePos = vTar - vPos;
	vMovePos = XMVector3Normalize(vMovePos) * moveSpeed_;
	vMovePos += vPos;
	XMFLOAT3 fPos;
	XMStoreFloat3(&fPos, vMovePos);
	transform_.position_ = fPos;

	float leng = XMVectorGetX(XMVector3Length(vMovePos - vTar));
	if (leng <= 1.0f) ChangeState(S_CHARGING);

}

void UfoObstacle::UpdateCharging()
{
	int shotTime = 30;
	if (time_ > shotTime) ChangeState(S_SHOT);
	time_++;
}

void UfoObstacle::UpdateShot()
{
	if (stateEnter_) {
		stateEnter_ = false;
		time_ = 0;

	}

	transform_.position_.z -= 0.3f + (time_ * 0.007f);

	int shotTime = 100;
	if (time_ > shotTime) ChangeState(S_IDLE);
	time_++;
}

void UfoObstacle::UpdateIdle()
{
	if (stateEnter_) {
		stateEnter_ = false;
		time_ = 0;
	}

	int leaveTime = 100;
	if (time_ > leaveTime) ChangeState(S_LEAVING);
	time_++;
}

void UfoObstacle::UpdateLeaving()
{
	transform_.position_.y += 1.0f;
	if (transform_.position_.y >= 20.0f) KillMe();

}

void UfoObstacle::ChangeState(STATE s)
{
	state_ = s;
	stateEnter_ = true;
}
