#include "MissileObstacle.h"
#include "Engine/BoxCollider.h"
#include "Engine/Model.h"

MissileObstacle::MissileObstacle(GameObject* parent)
	:Obstacle(parent)
{
}

MissileObstacle::~MissileObstacle()
{
}

void MissileObstacle::Initialize()
{
	//モデルロード
	hModel_ = Model::Load("Missile.fbx");
	assert(hModel_ >= 0);

	//当たり判定付与
	BoxCollider* collision = new BoxCollider(XMFLOAT3(0,0,0), XMFLOAT3(0.8f, 0.8f, 1.8f));
	AddCollider(collision);

	//揺れるアニメーション
	Model::SetAnimFrame(hModel_, 0, 60, 1);

}

void MissileObstacle::Update()
{
	if (!active_) return;

	///////////////移動テスト
	transform_.position_.z -= 0.1f;
	transform_.position_.y = 1.0f;


}

void MissileObstacle::Draw()
{
	if (!active_) return;

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void MissileObstacle::Release()
{
}

void MissileObstacle::OnCollision(GameObject* pTarget)
{
	XMFLOAT3 position = {0,0,0};
	//Playerに当たったとき
	if (pTarget->GetObjectName() == "Player")
	{
		//SetPosition(XMFLOAT3(0, 0, 30));
		position = pTarget->GetPosition();
		//////////////////プレイヤーノックバック処理	
	}
}