#include "PauseMenu.h"
#include "Engine/Input.h"
#include "ButtonFactory.h"
#include "ExitMenu.h"
#include "Engine/SceneManager.h"
#include "AudioManager.h"
#include "PlayScene.h"
#include <future>
#include "Setting.h"

//メニュークラスを作るとして
//Pause、Settingクラスを継承して作ることになる　と思う

//Pauseはプレイシーン限定で出す
//Settign画面はすべてのシーンで出す
//二つともPlayシーンの動きを止めることは確定

//Setting画面に必要な機能絶対なやつ
//マウス感度と、キーコンフィグの設定が欲しい
//あと音量の設定、


PauseMenu::PauseMenu(GameObject* parent)
	:GameObject(parent, "PauseMenu"), hPict_{ -1,-1 }, pButtonFactory_(nullptr)
{
}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::Initialize()
{
	pButtonFactory_ = Instantiate<ButtonFactory>(this);
	pButtonFactory_->ButtonCreate(0.0f, 400.0f, 1.0f, 1.0f, "Retry");
	pButtonFactory_->ButtonCreate(0.0f, 100.0f, 1.0f, 1.0f, "ReturnGame");
	pButtonFactory_->ButtonCreate(0.0f, -200.0f, 1.0f, 1.0f, "Setting");
	pButtonFactory_->ButtonCreate(0.0f, -500.0f, 1.0f, 1.0f, "ReturnTitle");
	pButtonFactory_->SetAlpha(200);
	pButtonFactory_->SetFrameAlpha(200);
	pButtonFactory_->SetBlendMode(0);

	PlayScene* pPlayScene = (PlayScene*)FindObject("PlayScene");
	pPlayScene->SetObjectActive(false);

	hPict_[0] = Image::Load("Png/Black.png");
	assert(hPict_[0] >= 0);
	hPict_[1] = Image::Load("Png/cross.png");
	assert(hPict_[1] >= 0);

	AudioManager::Release();
	AudioManager::Initialize(AudioManager::PLAYMENUE);

}

void PauseMenu::Update()
{
	if (pButtonFactory_->CheckButtonPressed() == "Retry") {
		PlayScene* pPlayScene = (PlayScene*)FindObject("PlayScene");
		pPlayScene->ResetGame();

		KillMe();
		return;
	}

	if (pButtonFactory_->CheckButtonPressed() == "ReturnGame") {
		PlayScene* pPlayScene = (PlayScene*)FindObject("PlayScene");
		pPlayScene->SetObjectActive(true);

		AudioManager::Release();
		AudioManager::Initialize(AudioManager::PLAY);

		KillMe();

	}
	else if (pButtonFactory_->CheckButtonPressed() == "Setting") {
		Instantiate<Setting>(this);
		AudioManager::PlaySoundMa(AUDIO_ENTERCURSOR);

		pButtonFactory_->SetActive(false);
		pButtonFactory_->SetAlpha(10);
		pButtonFactory_->SetFrameAlpha(10);

	}
	else if (pButtonFactory_->CheckButtonPressed() == "ReturnTitle"){
		AudioManager::PlaySoundMa(AUDIO_ENTERCURSOR);
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //待機
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);

	}

}

void PauseMenu::Draw()
{
	Transform back = transform_;
	back.scale_.y = 1.5f;
	Image::SetAlpha(hPict_[0], 100);
	Image::SetTransform(hPict_[0], back);
	Image::Draw(hPict_[0]);

	Transform abb;
	abb.position_ = Input::GetMousePositionSub();
	static float screenWidth = (float)GetSystemMetrics(SM_CXSCREEN);		//スクリーンの幅
	static float screenHeight = (float)GetSystemMetrics(SM_CYSCREEN);	//スクリーンの高さ
	abb.position_ = { abb.position_.x / screenWidth, -abb.position_.y / screenHeight , 0 };

	Image::SetTransform(hPict_[1], abb);
	Image::Draw(hPict_[1]);
}

void PauseMenu::Release()
{
}