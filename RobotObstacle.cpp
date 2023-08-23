#include "RobotObstacle.h"
#include "Engine/SphereCollider.h"
#include "Engine/Model.h"
#include "Player.h"
#include "ObstacleManager.h"
#include <vector>
#include "Missile.h"

RobotObstacle::RobotObstacle(GameObject* parent)
	:Obstacle(parent), pPlayer_(nullptr), backMove_(false), nearestLocation_(0), hModelHead_(-1), count_(0)
{
	SetObjectName("RobotObstacle");
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

	Model::SetAnimFrame(hModel_, 61, 240, 1);

	count_ = 90;
}

void RobotObstacle::Update()
{
	if (!active_) return;

	if (backMove_) {
		transform_.position_.y += 0.2f;
		if (transform_.position_.y > 10.0f) {
			KillMe();
		}

		return;
	}

	static int leng = 2.0f;
	if (nearestLocation_ - (int)leng <= (int)transform_.position_.z) {
		backMove_ = true;
		return;
	}

	if (transform_.position_.z < pPlayer_->GetPosition().z + 8.0f)
		backMove_ = true;

	static float moveSpeed = 0.1f;
	static float moveStop = -50;
	count_--;
	if (count_ > 0)
	{
		transform_.position_.z += moveSpeed;
		moveStop;
	}
	if (count_ <= moveStop)
	{
		Missile* pMissile1 = Instantiate<Missile>(GetParent()->GetParent());
		pMissile1->SetPosition(transform_.position_.x + 0.6f, transform_.position_.y + 2.2f, transform_.position_.z);
		pMissile1->SetTarget(0.3f, 0.3f, 0.0f);
		Missile* pMissile2 = Instantiate<Missile>(GetParent()->GetParent());
		pMissile2->SetPosition(transform_.position_.x - 0.6f, transform_.position_.y + 2.2f, transform_.position_.z);
		pMissile2->SetTarget(-0.3f, 0.3f, 0.0f);

		count_ = 90;
	}
	transform_.position_.y = 0.0f;

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

void RobotObstacle::SetLearestLocation()
{
	nearestLocation_ = 99999;
	ObstacleManager* pObstacleManager = (ObstacleManager*)FindObject("ObstacleManager");
	std::vector<Obstacle*> obj = pObstacleManager->GetObstacleList();
	for (Obstacle* e : obj) {
		if (e->GetObjectName() == "WallObstacle" && e->GetPosition().z > transform_.position_.z &&
			e->GetPosition().z < nearestLocation_) {
			nearestLocation_ = (int)e->GetPosition().z;
		}
	}
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