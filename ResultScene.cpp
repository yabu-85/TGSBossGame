#include "ResultScene.h"
#include "ExitMenu.h"	
#include "Engine/SceneManager.h"
#include "ButtonFactory.h"
#include "Button.h"
#include "Engine/Input.h"
#include "AudioManager.h"

ResultScene::ResultScene(GameObject* parent)
	:GameObject(parent, "ResultScene"), hPict_{ -1,-1,-1 }, pButtonFactory_(nullptr)
{
}

//初期化
void ResultScene::Initialize()
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager->GetResult()) {
		hPict_[0] = Image::Load("Png/ClearBG.png");
		assert(hPict_[0] >= 0);

		hPict_[1] = Image::Load("Png/Clear.png");
		assert(hPict_[1] >= 0);
	}
	else {
		hPict_[0] = Image::Load("Png/OverBG.png");
		assert(hPict_[0] >= 0);

		hPict_[1] = Image::Load("Png/Over.png");
		assert(hPict_[1] >= 0);
	}

	hPict_[2] = Image::Load("Png/cross.png");
	assert(hPict_[2] >= 0);

	pButtonFactory_ = Instantiate<ButtonFactory>(this);
	if (pSceneManager->GetResult()) {
		//クリアした場合のボタン
		pButtonFactory_->ButtonCreate(0.0f, -300.0f, 1.0f, 1.0f, "ReturnTitle");
		pButtonFactory_->ButtonCreate(0.0f, -600.0f, 1.0f, 1.0f, "Exit");
	}
	else {
		pButtonFactory_->ButtonCreate(0.0f, -200.0f, 1.0f, 1.0f, "Retry");
		pButtonFactory_->ButtonCreate(0.0f, -450.0f, 1.0f, 1.0f, "ReturnTitle");
		pButtonFactory_->ButtonCreate(0.0f, -700.0f, 1.0f, 1.0f, "Exit");

	}
	
	pButtonFactory_->SetAlpha(200);
	pButtonFactory_->SetFrameAlpha(200);
	pButtonFactory_->SetBlendMode(0);

	AudioManager::Release();
	AudioManager::Initialize(AudioManager::RESULT);

}

//更新
void ResultScene::Update()
{
	if (pButtonFactory_->CheckButtonPressed() == "Retry") {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);

	}
	if (pButtonFactory_->CheckButtonPressed() == "ReturnTitle") {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);

	}
	else if (pButtonFactory_->CheckButtonPressed() == "Exit") {
		Instantiate<ExitMenu>(this);
		GameObject* gs2 = GetParent()->FindObject("ButtonFactory");
		ButtonFactory* pB = (ButtonFactory*)gs2;
		pB->SetActive(false);
		pB->SetAlpha(10);
		pB->SetFrameAlpha(10);
		
	}

}

//描画
void ResultScene::Draw()
{
	Transform pos;
	pos.scale_ = XMFLOAT3(2.0f, 1.6f, 2.0f);
	Image::SetAlpha(hPict_[0], 150);
	Image::SetTransform(hPict_[0], pos);
	Image::Draw(hPict_[0]);

	pos.position_.y = 0.2f;
	pos.scale_ = XMFLOAT3(1.7f, 1.7f, 1.7f);
	Image::SetTransform(hPict_[1], pos);
	Image::Draw(hPict_[1]);

	DrawCursor();
}

//開放
void ResultScene::Release()
{
}



void ResultScene::DrawCursor()
{
	static float screenWidth = (float)GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	static float screenHeight = (float)GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ

	Transform aim;
	aim.scale_.x = 0.5f;
	aim.scale_.y = 0.5f;
	aim.scale_.z = 0.5f;
	aim.position_ = Input::GetMousePositionSub();
	aim.position_ = { aim.position_.x / screenWidth, -aim.position_.y / screenHeight , 0 };

	Image::SetTransform(hPict_[2], aim);
	Image::Draw(hPict_[2]);
}