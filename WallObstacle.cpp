#include "WallObstacle.h"
#include "Engine/Model.h"

WallObstacle::WallObstacle(GameObject* parent)
	:Obstacle(parent), pPlayer_(nullptr)
{
	objectName_ = "WallObstacle";
}

WallObstacle::~WallObstacle()
{
}

void WallObstacle::Initialize()
{
	//ƒ‚ƒfƒ‹ƒ[ƒh
	hModel_ = Model::Load("Model/Wall_Stone.fbx");
	assert(hModel_ >= 0);

	pPlayer_ = (Player*)FindObject("Player");

}

void WallObstacle::Update()
{
	if (!active_) return;

	transform_.position_.y = 0.0f;

}

void WallObstacle::Draw()
{
	if (!isDraw_) return;

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void WallObstacle::Release()
{
}
