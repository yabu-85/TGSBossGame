#include "Setting.h"
#include "Engine/Input.h"
#include "ButtonFactory.h"
#include "SliderButton.h"
#include "ExitMenu.h"
#include "Engine/SceneManager.h"
#include "AudioManager.h"
#include "PlayScene.h"
#include <future>

Setting::Setting(GameObject* parent)
	:GameObject(parent, "Setting"), hPict_{ -1,-1 }, pButtonFactory_(nullptr)
{
}

Setting::~Setting()
{
}

void Setting::Initialize()
{
	pButtonFactory_ = Instantiate<ButtonFactory>(this);
	pButtonFactory_->ButtonCreate(0.0f, -500.0f, 1.0f, 1.0f, "ReturnTitle");
	pButtonFactory_->SetAlpha(200);
	pButtonFactory_->SetFrameAlpha(200);
	pButtonFactory_->SetBlendMode(0);

	pSlider_ = Instantiate<SliderButton>(this);
	pSlider_->SetValue(0.0f, 150.0f, 1.0f, 1.0f, "ReturnGame");
	pSlider_->SetAlpha(200);
	pSlider_->SetFrameAlpha(200);
	pSlider_->SetBlendMode(0);

	pSlider2_ = Instantiate<SliderButton>(this);
	pSlider2_->SetValue(0.0f, -150.0f, 1.0f, 1.0f, "ReturnGame");
	pSlider2_->SetAlpha(200);
	pSlider2_->SetFrameAlpha(200);
	pSlider2_->SetBlendMode(0);

	hPict_[0] = Image::Load("Png/Black.png");
	assert(hPict_[0] >= 0);
	hPict_[1] = Image::Load("Png/cross.png");
	assert(hPict_[1] >= 0);

}

void Setting::Update()
{
	if (pButtonFactory_->CheckButtonPressed() == "ReturnTitle"){
		GameObject* gs2 = GetParent()->FindObject("ButtonFactory");
		ButtonFactory* pB = (ButtonFactory*)gs2;
		pB->SetActive(true);
		pB->SetAlpha(200);
		pB->SetFrameAlpha(200);
		KillMe();
	}

}

void Setting::Draw()
{
	Image::SetAlpha(hPict_[0], 100);
	Image::SetTransform(hPict_[0], transform_);
	Image::Draw(hPict_[0]);

	Transform abb;
	abb.position_ = Input::GetMousePositionSub();
	static float screenWidth = (float)GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	static float screenHeight = (float)GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ
	abb.position_ = { abb.position_.x / screenWidth, -abb.position_.y / screenHeight , 0 };

	Image::SetTransform(hPict_[1], abb);
	Image::Draw(hPict_[1]);
}

void Setting::Release()
{
}