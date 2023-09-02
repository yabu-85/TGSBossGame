#include "PauseMenu.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "ButtonFactory.h"
#include "Button.h"
#include "Aim.h"
#include "ExitMenu.h"
#include "Player.h"
#include "Engine/SceneManager.h"
#include "Engine/Model.h"
#include "Timer.h"
#include "ObstacleManager.h"
#include "AudioManager.h"
#include <future>

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
	pButtonFactory_->ButtonCreate(0.0f, 300.0f, 1.0f, 1.0f, "ReturnGame");
	pButtonFactory_->ButtonCreate(0.0f, 0.0f, 1.0f, 1.0f, "ReturnTitle");
	pButtonFactory_->ButtonCreate(0.0f, -300.0f, 1.0f, 1.0f, "Quit");
	pButtonFactory_->SetAlpha(200);
	pButtonFactory_->SetFrameAlpha(200);
	pButtonFactory_->SetBlendMode(0);

	Aim* pAim = (Aim*)FindObject("Aim");
	pAim->SetAimMove(false);
	Player* pPlayer = (Player*)FindObject("Player");
	Model::SetAnimeStop(pPlayer->GetModelHandle(), true);
	Timer* pTimer = (Timer*)FindObject("Timer");
	pTimer->Stop();

	ObstacleManager* pObstacleManager = (ObstacleManager*)FindObject("ObstacleManager");
	pObstacleManager->SetAllObstacleActive(false);

	hPict_[0] = Image::Load("Png/Black.png");
	assert(hPict_[0] >= 0);
	hPict_[1] = Image::Load("Png/cross.png");
	assert(hPict_[1] >= 0);

	AudioManager::Release();
	AudioManager::Initialize(AudioManager::PLAYMENUE);

}

void PauseMenu::Update()
{
	if (pButtonFactory_->CheckButtonPressed() == "ReturnGame") {
		Aim* pAim = (Aim*)FindObject("Aim");
		pAim->SetAimMove(true);
		Player* pPlayer = (Player*)FindObject("Player");
		pPlayer->SetActive(true);
		Model::SetAnimeStop(pPlayer->GetModelHandle(), false);
		Timer* pTimer = (Timer*)FindObject("Timer");
		pTimer->Start();
		
		ObstacleManager* pObstacleManager = (ObstacleManager*)FindObject("ObstacleManager");
		pObstacleManager->SetAllObstacleActive(true);

		AudioManager::Release();
		AudioManager::Initialize(AudioManager::PLAY);

		KillMe();

	}
	else if (pButtonFactory_->CheckButtonPressed() == "ReturnTitle"){
		AudioManager::PlaySoundMa(AUDIO_ENTERCURSOR);
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //待機
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);

	}
	else if (pButtonFactory_->CheckButtonPressed() == "Quit") {
		Instantiate<ExitMenu>(this);

		GameObject* gs2 = GetParent()->FindObject("ButtonFactory");
		ButtonFactory* pB = (ButtonFactory*)gs2;
		pB->SetActive(false);
		pB->SetAlpha(10);
		pB->SetFrameAlpha(10);
	
	}

}

void PauseMenu::Draw()
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

void PauseMenu::Release()
{
}