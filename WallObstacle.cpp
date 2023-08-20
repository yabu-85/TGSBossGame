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
	hModel_ = Model::Load("Wall_Stone.fbx");
	assert(hModel_ >= 0);

	//当たり判定付与
	BoxCollider* collision = new BoxCollider(XMFLOAT3(0, 3.0, 0), XMFLOAT3(8.5, 4, 0.8));
	AddCollider(collision);
}

void WallObstacle::Update()
{

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