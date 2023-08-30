#include "SkyBox.h"
#include "Engine/Model.h"
#include "Player.h"

SkyBox::SkyBox(GameObject* parent)
	:GameObject(parent, "SkyBos"), hModel_(-1), pPlayer_(nullptr)
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::Initialize()
{
	hModel_ = Model::Load("Model/SkyBox.fbx");
	assert(hModel_ >= 0);

	//Max950‚­‚ç‚¢
	float size = 950.0f;
	transform_.scale_ = { size, size, size };

}

void SkyBox::Update()
{
}

void SkyBox::Draw()
{
	//ƒ‚ƒfƒ‹•`‰æ
	if(pPlayer_ != nullptr) transform_.position_ = pPlayer_->GetPosition();
	else pPlayer_ = (Player*)FindObject("Player");
	
	transform_.position_.y += 1.5f;	//Aim‚ÌHeight‚Ì’l
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_, 6);
}

void SkyBox::Release()
{
}
