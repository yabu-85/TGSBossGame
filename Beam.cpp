#include "Beam.h"
#include "Engine/Model.h"
#include "Player.h"
#include "AudioManager.h"

namespace {
	float moveSpeed_ = 1.0f;
	float leng = 2.5f;	//targetの判定長さ
}

Beam::Beam(GameObject* parent)
	: GameObject(parent, "Beam"), hModel_(-1), isActive_(true), pPlayer_(nullptr), isTargetHit_(false), isPlayerHit_(false),
	prevPosition_(0,0,0), target_(0,0,0), minLeng_(99999)
{
}

Beam::~Beam()
{
}

void Beam::Initialize()
{
	hModel_ = Model::Load("Model/Beam.fbx", 0, 0);
	assert(hModel_ >= 0);

	pPlayer_ = (Player*)FindObject("Player");

	target_ = pPlayer_->GetPosition();
	target_.y += 0.5f;

	prevPosition_ = transform_.position_;

}

void Beam::Update()
{
	if (isActive_ == false) return;

	prevPosition_ = transform_.position_;

	//移動
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vTar = XMLoadFloat3(&target_);
	XMVECTOR vVec = XMVector3Normalize(vTar - vPos) * moveSpeed_;
	XMStoreFloat3(&transform_.position_, vPos + vVec);
	vPos = XMLoadFloat3(&transform_.position_);
	vTar = XMLoadFloat3(&target_);

	Rotate();

	if (isPlayerHit_ || isTargetHit_) return;

	//ターゲット位置に入ったか
	float distance = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (distance <= minLeng_) minLeng_ = distance;
	else isTargetHit_ = true;

	//こっちはプレイヤーとの範囲内か(真ん中
	XMFLOAT3 plaPos = pPlayer_->GetPosition();
	plaPos.y += 0.5f;
	vTar = XMLoadFloat3(&plaPos);
	distance = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (distance <= 0.5f) {
		AudioManager::PlaySoundMa(AUDIO_MISSILE_EXPLODE);
		pPlayer_->DecreaseHp(5);
		isPlayerHit_ = true;
		return;
	}

	//プレイヤーの判定2個目移動速度が速いため補完(前
	XMVECTOR vPrePos = XMLoadFloat3(&prevPosition_);
	XMVECTOR v = vPos + (vPos - vPrePos);
	distance = XMVectorGetX(XMVector3Length(vTar - v));
	if (distance <= 0.5f) {
		AudioManager::PlaySoundMa(AUDIO_MISSILE_EXPLODE);
		pPlayer_->DecreaseHp(5);
		isPlayerHit_ = true;
		return;
	}

	//プレイヤーの判定3個目移動速度が速いため補完(後ろ
	vPrePos = XMLoadFloat3(&prevPosition_);
	v = vPos + (vPrePos - vPos);
	distance = XMVectorGetX(XMVector3Length(vTar - v));
	if (distance <= 0.5f) {
		AudioManager::PlaySoundMa(AUDIO_MISSILE_EXPLODE);
		pPlayer_->DecreaseHp(5);
		isPlayerHit_ = true;
		return;
	}

}

void Beam::Draw()
{
	Transform trs = transform_;
	trs.rotate_.y += 180.0f;
	Model::SetTransform(hModel_, trs);
}

void Beam::Release()
{
}

void Beam::KillMeSub()
{
	Model::StopDraw(hModel_);
	KillMe();
}

//----------------------private---------------------

void Beam::Rotate()
{
	const XMVECTOR vFront{ 0, 0, 1, 0 };
	XMFLOAT3 fAimPos = XMFLOAT3(transform_.position_.x - target_.x, 0, transform_.position_.z - target_.z);
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
}