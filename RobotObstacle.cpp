#include "RobotObstacle.h"
#include "Engine/SphereCollider.h"
#include "Engine/Model.h"
#include "Player.h"
#include "ObstacleManager.h"
#include <vector>
#include "Missile.h"

RobotObstacle::RobotObstacle(GameObject* parent)
	:Obstacle(parent), pPlayer_(nullptr), nearestLocation_(0), hModelHead_(-1), count_(0),
	state_(S_ENTER), stateEnter_(true)
{
	objectName_ = "RobotObstacle";
}

RobotObstacle::~RobotObstacle()
{
}

void RobotObstacle::Initialize()
{
	//モデルロード
	hModel_ = Model::Load("Robot_body.fbx");
	assert(hModel_ >= 0);

	//モデルロード
	hModelHead_ = Model::Load("Robot_Head.fbx");
	assert(hModelHead_ >= 0);

	//当たり判定付与
	BoxCollider* collision = new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(3.0f, 1.0f, 3.0f));
	AddCollider(collision);

	pPlayer_ = (Player*)FindObject("Player");
	transform_.rotate_.y = 180;

	count_ = 90;
}

void RobotObstacle::Update()
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

	Rotate();
		
}

void RobotObstacle::Draw()
{
	if (!active_) return;

	Transform body = transform_;
	body.rotate_.y = 180;
	Model::SetTransform(hModel_, body);
	Model::Draw(hModel_);

	Transform head = transform_;
	Model::SetTransform(hModelHead_, head);
	Model::Draw(hModelHead_);
}

void RobotObstacle::Release()
{
}

void RobotObstacle::OnCollision(GameObject* pTarget)
{
	XMFLOAT3 position = { 0,0,0 };
	//Playerに当たったとき
	if (pTarget->GetObjectName() == "Player")
	{
		//SetPosition(XMFLOAT3(0, 0, 30));
		position = pTarget->GetPosition();
		//////////////////プレイヤーノックバック処理	
	}
}

void RobotObstacle::UpdateEnter()
{
	if (stateEnter_) {
		stateEnter_ = false;
		transform_.position_.y = 40.0f;
	}

	transform_.position_.y -= 2.0f;
	if (transform_.position_.y <= 0.0f) {
		transform_.position_.y = 0.0f;
		ChangeState(S_CHARGING);

	}

}

void RobotObstacle::UpdateCharging()
{
	count_--;
	if (count_ <= 0)
		ChangeState(S_SHOT);
}

void RobotObstacle::UpdateShot()
{
	ShotMissile();

	ChangeState(S_IDLE);
}

void RobotObstacle::UpdateIdle()
{
	//跳ね返さなかったら
	if(false) ChangeState(S_LEAVING);

}

void RobotObstacle::UpdateLeaving()
{
	transform_.position_.y += 0.2f;
	if (transform_.position_.y > 10.0f) {
		KillMe();
	}

}

void RobotObstacle::ChangeState(STATE s)
{
	state_ = s;
	stateEnter_ = true;
}

void RobotObstacle::Rotate()
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

void RobotObstacle::ShotMissile()
{
	struct mPos { //Position
		float x, y, z;
	} tblP[] = {
		{0.6f, 2.2f, 0},
		{0.6f, 2.2f, 0},
		{0.6f, 2.2f, 0},
		{0.6f, 2.2f, 0},
		{-0.6f, 2.2f, 0},
		{-0.6f, 2.2f, 0},
		{-0.6f, 2.2f, 0},
		{-0.6f, 2.2f, 0},

	};
	struct mTar { //Target
		float x, y, z;
	} tblT[] = {
		{0.3f, 0.0f, 0.0f},
		{0.25f, 0.1f, 0.0f},
		{0.2f, 0.2f, 0.0f},
		{0.03f, 0.3f, 0.0f},
		{-0.3f, 0.0f, 0.0f},
		{-0.25f, 0.1f, 0.0f},
		{-0.2f, 0.2f, 0.0f},
		{-0.03f, 0.3f, 0.0f},
	};

	int missileCount = 8;
	for (int i = 0; i < missileCount; i++) {
		Missile* pMissile = Instantiate<Missile>(GetParent()->GetParent());
		pMissile->SetPosition(transform_.position_.x + tblP[i].x, transform_.position_.y + tblP[i].y, transform_.position_.z + tblP[i].z);
		pMissile->SetTarget(tblT[i].x, tblT[i].y, tblT[i].z);
		pMissile->SetParent(this);
		if (i == 0) pMissile->SetKillParent(true);
	}
}
