#include "UfoObstacle.h"
#include "Engine/SphereCollider.h"
#include "Engine/Model.h"

UfoObstacle::UfoObstacle(GameObject* parent)
	:Obstacle(parent), firstMoveZ_(0.0f), first_(false), hModelLa_(-1), move_(0.0f), maxMoveX_(0.0f), x_(true), firstAct_(true)
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

	if (firstAct_) {
		if (rand() % 2 == 0) {
			x_ = false;
			maxMoveX_ = 0.7f;
			transform_.position_.x += 3.0f;
		}
		else {
			maxMoveX_ = 0.5f;
			transform_.position_.x += -3.0f;
		}
		
		firstAct_ = false;
	}

	static float moveSpeedX = 0.5f;
	static float moveSpeedZ = 0.5f;

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

	if (x_) {
		move_ += 0.01f;
		if (move_ >= maxMoveX_) x_ = false;
	}
	else {
		move_ -= 0.01f;
		if (move_ <= -maxMoveX_) x_ = true;
	}
	transform_.position_.x += (move_ * moveSpeedX);

	transform_.position_.z -= 0.1f;

}

void UfoObstacle::Draw()
{
	if (!active_ || firstAct_) return;

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