#include "WallObstacle.h"
#include "Engine/Model.h"
#include "Player.h"
#include "PlayerSpeedController.h"

namespace {
	const float leng = 0.4f;
	float parcent = 0.0f;

}

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
	//‚à‚Å‚é
	hModel_ = Model::Load("Model/Wall_Stone.fbx");
	assert(hModel_ >= 0);

	pPlayer_ = (Player*)FindObject("Player");

}

void WallObstacle::Update()
{
	if (!active_) return;

	transform_.position_.y = 0.0f;

	if (!pPlayer_->IsisCrouc()) {
		XMFLOAT3 plaPos = pPlayer_->GetPosition();
		if ((plaPos.z <= transform_.position_.z + leng) && (plaPos.z > transform_.position_.z - leng)) {
			parcent = 1.0f;
			XMFLOAT3 move = { 0, 0, 0 };
			pPlayer_->SetPlayerMovement(move);
			PlayerSpeedController* pSpeed = (PlayerSpeedController*)FindObject("PlayerSpeedController");
			pSpeed->ResetSpeed(true);
		}
	}

	if (parcent > 0) {
		XMFLOAT3 plaPos = pPlayer_->GetPosition();
		plaPos.z -= 1.0f * parcent;
		parcent -= 0.05f;
		pPlayer_->SetPosition(plaPos);

	}



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

void WallObstacle::KillMeSub()
{
	KillMe();
}
