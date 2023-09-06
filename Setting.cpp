#include "Setting.h"
#include "Engine/Input.h"
#include "ButtonFactory.h"
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
	pButtonFactory_->ButtonCreate(0.0f, 300.0f, 1.0f, 1.0f, "ReturnGame");
	pButtonFactory_->ButtonCreate(0.0f, 0.0f, 1.0f, 1.0f, "ReturnTitle");
	pButtonFactory_->SetAlpha(200);
	pButtonFactory_->SetFrameAlpha(200);
	pButtonFactory_->SetBlendMode(0);

	hPict_[0] = Image::Load("Png/White.png");
	assert(hPict_[0] >= 0);
	hPict_[1] = Image::Load("Png/cross.png");
	assert(hPict_[1] >= 0);

	AudioManager::Release();
	AudioManager::Initialize(AudioManager::PLAYMENUE);

}

void Setting::Update()
{
	if (pButtonFactory_->CheckButtonPressed() == "ReturnGame") {


		KillMe();

	}
	else if (pButtonFactory_->CheckButtonPressed() == "ReturnTitle"){
		AudioManager::PlaySoundMa(AUDIO_ENTERCURSOR);
		
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