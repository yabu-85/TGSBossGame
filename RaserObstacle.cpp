#include "RaserObstacle.h"
#include "Engine/Model.h"
#include "Player.h"
#include "Beam.h"
#include "AudioManager.h"
#include "ObstacleManager.h"

RaserObstacle::RaserObstacle(GameObject* parent)
	:Obstacle(parent), hModelHead_(-1), count_(0), pBeam_(nullptr)
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

	hModelHead_ = Model::Load("Model/Robot_Head_Raser.fbx");
	assert(hModelHead_ >= 0);

	pPlayer_ = (Player*)FindObject("Player");

	transform_.rotate_.y = 180.0f;
}

void RaserObstacle::Update()
{
	if (!active_) return;

	Rotate();

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

void RaserObstacle::KillMeSub()
{
	if(pBeam_ != nullptr) pBeam_->KillMeSub();

	KillMe();
}

void RaserObstacle::SetBeamActive(bool b)
{
	if(pBeam_ != nullptr)
	pBeam_->SetActive(b);
}

//--------------------state----------------------//

void RaserObstacle::UpdateEnter()
{
	if (stateEnter_) {
		stateEnter_ = false;
		transform_.position_.y = 40.0f;
	}

	transform_.position_.y -= 2.0f;
	if (transform_.position_.y <= 0.0f) {
		transform_.position_.y = 0.0f;
		AudioManager::PlaySoundMa(AUDIO_ROBOT_HIT, 0.5f);

		ChangeState(S_CHARGING);
	}
}

void RaserObstacle::UpdateCharging()
{
	if (stateEnter_) {
		stateEnter_ = false;
		count_ = 80;
	}

	count_--;
	if (count_ <= 0)
		ChangeState(S_SHOT);
}

void RaserObstacle::UpdateShot()
{
	AudioManager::PlaySoundMa(AUDIO_BEAM_SHOT);

	ShotBeam();
	ChangeState(S_IDLE);
}

void RaserObstacle::UpdateIdle()
{
	if (stateEnter_) {
		stateEnter_ = false;
		count_ = 120; //待ち時間
	}

	if (pBeam_->IsTargetHit() || pBeam_->IsPlayerHit()) {
		pBeam_->KillMeSub();
		ChangeState(S_LEAVING);

	}
	
	count_--;
	if (count_ <= 0)
		ChangeState(S_LEAVING);
}

void RaserObstacle::UpdateLeaving()
{
	transform_.position_.y += 2.0f;
	if (transform_.position_.y > 40.0f) {
		ObstacleManager* pOM = (ObstacleManager*)FindObject("ObstacleManager");
		pOM->removeObstacle(this);

		KillMeSub();
	}
}

void RaserObstacle::ChangeState(STATE s)
{
	state_ = s;
	stateEnter_ = true;
}

//------------private------------------

void RaserObstacle::Rotate()
{
	XMFLOAT3 plaPos = pPlayer_->GetPosition();

	const XMVECTOR vFront{ 0, 0, 1, 0 };
	XMFLOAT3 fAimPos = XMFLOAT3(transform_.position_.x - plaPos.x, 0, transform_.position_.z - plaPos.z);
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

	transform_.rotate_.y = XMConvertToDegrees(angle);
	transform_.rotate_.y += 180.0f; //Blender

}

void RaserObstacle::ShotBeam()
{
	pBeam_ = Instantiate<Beam>(GetParent());
	pBeam_->SetPosition(transform_.position_.x, transform_.position_.y + 2.2f, transform_.position_.z);

}