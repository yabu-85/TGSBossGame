#include "RaserObstacle.h"
#include "Engine/Model.h"

RaserObstacle::RaserObstacle(GameObject* parent)
	:Obstacle(parent), hModelHead_(-1)
{
	objectName_ = "RaserObstacle";
}

RaserObstacle::~RaserObstacle()
{
}

void RaserObstacle::Initialize()
{
	//モデルロード
	hModel_ = Model::Load("Model/Robot_Body_Raser.fbx");
	assert(hModel_ >= 0);

	//モデルロード
	hModelHead_ = Model::Load("Model/Robot_Head_Raser.fbx");
	assert(hModelHead_ >= 0);

	transform_.rotate_.y = 180.0f;
}

void RaserObstacle::Update()
{
	if (!active_) return;

	transform_.position_.y = 0;
}

void RaserObstacle::Draw()
{
	if (!isDraw_) return;

	Transform body = transform_;
	body.rotate_.y = 180;
	Model::SetTransform(hModel_, body);
	Model::Draw(hModel_);

	Model::SetTransform(hModelHead_, transform_);
	Model::Draw(hModelHead_);
}

void RaserObstacle::Release()
{
}


void RaserObstacle::UpdateEnter()
{
}

void RaserObstacle::UpdateCharging()
{
}

void RaserObstacle::UpdateShot()
{
}

void RaserObstacle::UpdateIdle()
{
}

void RaserObstacle::UpdateLeaving()
{
}

void RaserObstacle::ChangeState(STATE s)
{
	state_ = s;
	stateEnter_ = true;
}