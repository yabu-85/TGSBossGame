#include "BossObstacle.h"
#include "Engine/Model.h"
#include "Player.h"

BossObstacle::BossObstacle(GameObject* parent)
	:Obstacle(parent), pPlayer_(nullptr)
{
	objectName_ = "BossObstacle";
}

BossObstacle::~BossObstacle()
{
}

void BossObstacle::Initialize()
{
	//ƒ‚ƒfƒ‹ƒ[ƒh
	hModel_ = Model::Load("Model/Boss_03.fbx");
	assert(hModel_ >= 0);

	pPlayer_ = (Player*)FindObject("Player");
}

void BossObstacle::Update()
{
	if (!active_) return;

	transform_.position_.y = 2.0f;
	//transform_.rotate_.y += 1.0f;

	if (pPlayer_->GetPosition().z + 30.0f >= transform_.position_.z) {
		transform_.position_.z += 0.2f;
	}

}

void BossObstacle::Draw()
{
	if (!isDraw_) return;

	Transform body = transform_;
	body.rotate_.y += 180;
	Model::SetTransform(hModel_, body);
	Model::Draw(hModel_, 0);
}

void BossObstacle::Release()
{
}

void BossObstacle::UpdateEnter()
{
}

void BossObstacle::UpdateDead()
{
}

void BossObstacle::ChangeState(STATE s)
{
	state_ = s;
	stateEnter_ = true;
}
