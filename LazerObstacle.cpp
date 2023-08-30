#include "LazerObstacle.h"
#include "Engine/Model.h"

LazerObstacle::LazerObstacle(GameObject* parent)
	:Obstacle(parent), hModelHead_(-1)
{
	objectName_ = "LazerObstacle";
}

LazerObstacle::~LazerObstacle()
{
}

void LazerObstacle::Initialize()
{
	//モデルロード
	hModel_ = Model::Load("Model/Robot_body_lazer.fbx");
	assert(hModel_ >= 0);

	//モデルロード
	hModelHead_ = Model::Load("Model/Robot_Head_lazer.fbx");
	assert(hModelHead_ >= 0);

	transform_.rotate_.y = 180.0f;
}

void LazerObstacle::Update()
{
	if (!active_) return;

	transform_.position_.y = 0;
}

void LazerObstacle::Draw()
{
	if (!active_) return;

	Transform body = transform_;
	body.rotate_.y = 180;
	Model::SetTransform(hModel_, body);
	Model::Draw(hModel_);

	Model::SetTransform(hModelHead_, transform_);
	Model::Draw(hModelHead_);
}

void LazerObstacle::Release()
{
}
