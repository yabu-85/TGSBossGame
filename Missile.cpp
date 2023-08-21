#include "Missile.h"
#include "Engine/Model.h"

Missile::Missile(GameObject* parent)
	:GameObject(parent, "Missile"), hModel_(-1)
{
}

Missile::~Missile()
{
}

void Missile::Initialize()
{
	hModel_ = Model::Load("Missile.fbx");
	assert(hModel_ >= 0);

	transform_.position_.y = 1.0f;
}

void Missile::Update()
{
	// 角度をラジアンに変換
	float angleRad = (-transform_.rotate_.y + 90.0f) * XM_PI / 180.0f;

	// 角度から方向ベクトルを計算
	XMFLOAT3 direction = { cosf(angleRad), 0.0f, sinf(angleRad) };

	// 速度を乗算して位置を更新
	float speed = 0.5f; // 移動速度を調整
	transform_.position_.x += direction.x * speed;
	transform_.position_.z += direction.z * speed;

}

void Missile::Draw()
{
	Transform trs = transform_;
	trs.rotate_.y += 180.0f;
	Model::SetTransform(hModel_, trs);
	Model::Draw(hModel_);
}

void Missile::Release()
{
}
