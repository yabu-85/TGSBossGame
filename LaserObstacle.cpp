#include "LaserObstacle.h"
#include "Engine/Model.h"

LaserObstacle::LaserObstacle(GameObject* parent)
	:Obstacle(parent), hModelHead_(-1)
{
	objectName_ = "LaserObstacle";
}

LaserObstacle::~LaserObstacle()
{
}

void LaserObstacle::Initialize()
{
	//モデルロード
	hModel_ = Model::Load("Model/Robot_body_lazer.fbx");
	assert(hModel_ >= 0);

	//モデルロード
	hModelHead_ = Model::Load("Model/Robot_Head_lazer.fbx");
	assert(hModelHead_ >= 0);
}

void LaserObstacle::Update()
{
	if (!active_) return;

	transform_.position_.y = 0;
}

void LaserObstacle::Draw()
{
	if (!active_) return;

	Transform body = transform_;
	body.rotate_.y = 180;
	Model::SetTransform(hModel_, body);
	Model::Draw(hModel_);

	Model::SetTransform(hModelHead_, transform_);
	Model::Draw(hModelHead_);
}

void LaserObstacle::Release()
{
}
