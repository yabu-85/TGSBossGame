#include "TestObstacle.h"
#include "Engine/BoxCollider.h"
#include "Engine/Model.h"

TestObstacle::TestObstacle(GameObject* parent)
	:Obstacle(parent)
{
}

TestObstacle::~TestObstacle()
{
}

void TestObstacle::Initialize()
{
	//モデルロード
	hModel_ = Model::Load("Wall.fbx");
	assert(hModel_ >= 0);

	//当たり判定付与
	BoxCollider* collision = new BoxCollider(XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1, 1, 1));
	AddCollider(collision);
}

void TestObstacle::Update()
{
	///////////////移動テスト
	transform_.position_.z -= 0.1f;
}

void TestObstacle::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void TestObstacle::Release()
{
}

void TestObstacle::OnCollision(GameObject* pTarget)
{
	//Playerに当たったとき
	if (pTarget->GetObjectName() == "Player")
	{
		SetPosition(XMFLOAT3(0, 0, 30));
		//pTarget->SetPosition(0, 0, 30);
	}
}