#include "UfoObstacle.h"
#include "Engine/SphereCollider.h"
#include "Engine/Model.h"

UfoObstacle::UfoObstacle(GameObject* parent)
	:Obstacle(parent), firstMoveZ_(0.0f), first_(false), hModelLa_(-1)
{
}

UfoObstacle::~UfoObstacle()
{
}

void UfoObstacle::Initialize()
{
	//モデルロード
	hModel_ = Model::Load("Ufo.fbx");
	assert(hModel_ >= 0);

	//モデルロード
	hModelLa_ = Model::Load("Laser.fbx");
	assert(hModelLa_ >= 0);

	//当たり判定付与
	BoxCollider* collision = new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(3.0f, 1.0f, 3.0f));
	AddCollider(collision);

}

void UfoObstacle::Update()
{
	if (!active_) return;

	static float moveSpeedX = 0.5f;
	static float moveSpeedZ = 0.5f;
	static float move = 0.0f;
	static bool x = true;

	transform_.position_.y = 9.0f;

	if (firstMoveZ_ < 1.0f) {
		if (first_) {
			transform_.position_.z = -10.0f;
			first_ = true;
		}
		transform_.position_.z += (-EaseOutExpo(firstMoveZ_) * moveSpeedX);
		firstMoveZ_ += 0.1f;
		
		return;
	}

	if (x) {
		move += 0.01f;
		if (move >= 0.5) x = false;
	}
	else {
		move -= 0.01f;
		if (move <= -0.5) x = true;
	}
	transform_.position_.x += (move * moveSpeedX);

	transform_.position_.z -= 0.1f;

}

void UfoObstacle::Draw()
{
	if (!active_) return;

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	Transform pos = transform_;
	pos.position_.y -= 0.5f;
	Model::SetTransform(hModelLa_, pos);
	Model::Draw(hModelLa_);
	
}

void UfoObstacle::Release()
{
}

void UfoObstacle::OnCollision(GameObject* pTarget)
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