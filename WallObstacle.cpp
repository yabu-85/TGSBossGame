#include "WallObstacle.h"
#include "Engine/BoxCollider.h"
#include "Engine/Model.h"

WallObstacle::WallObstacle(GameObject* parent)
	:Obstacle(parent)
{
}

WallObstacle::~WallObstacle()
{
}

void WallObstacle::Initialize()
{
	//モデルロード
	hModel_ = Model::Load("Wall2.fbx");
	assert(hModel_ >= 0);

	//当たり判定付与
	BoxCollider* collision = new BoxCollider(XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1, 3, 1));
	AddCollider(collision);
}

void WallObstacle::Update()
{
	///////////////移動テスト
	transform_.position_.z -= 0.1f;
}

void WallObstacle::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void WallObstacle::Release()
{
}

void WallObstacle::OnCollision(GameObject* pTarget)
{
	XMFLOAT3 position = { 0,0,0 };
	//Playerに当たったとき
	if (pTarget->GetObjectName() == "Player")
	{
		//SetPosition(XMFLOAT3(0, 0, 30));
		position = pTarget->GetPosition();
		//////////////////プレイヤー減速処理
	}
}