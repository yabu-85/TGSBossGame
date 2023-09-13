#include "ResultScene.h"
#include "ExitMenu.h"	
#include "Engine/SceneManager.h"
#include "ButtonFactory.h"
#include "Button.h"
#include "Engine/Input.h"
#include "AudioManager.h"

namespace {
	int lank = 3; //Cランク
	//左からS,A,B,Cでその数以上だったらそのランクになる
	int stageClearTime[2][3] = { { 60, 50, 30},
						         { 100, 10, 10} 
	};
	
}

ResultScene::ResultScene(GameObject* parent)
	:GameObject(parent, "ResultScene"), hPict_{ -1,-1,-1 }, pButtonFactory_(nullptr), clearTime_(0), clearTimeSma_(0), result_(false), hPictLank_(-1)
{
	for (int i = 0; i < 12; i++) { hPictTime_[i] = -1; };

}

//初期化
void ResultScene::Initialize()
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	result_ = pSceneManager->GetResult();

	if (result_) {
		hPict_[0] = Image::Load("Png/ClearBG.png");
		assert(hPict_[0] >= 0);

		hPict_[1] = Image::Load("Png/Clear.png");
		assert(hPict_[1] >= 0);

		//Timer
		for (int i = 0; i < 10; i++) {
			std::string fileName = "Png/Time/Time";
			fileName = fileName + std::to_string(i);
			fileName = fileName + ".png";
			hPictTime_[i] = Image::Load(fileName);
			assert(hPictTime_[i] >= 0);
		}
		hPictTime_[10] = Image::Load("Png/Time/Colon.png");
		assert(hPictTime_[10] >= 0);

		hPictTime_[11] = Image::Load("Png/Time/Period.png");
		assert(hPictTime_[11] >= 0);
		
		clearTime_ = pSceneManager->GetClearTime();
		clearTimeSma_ = pSceneManager->GetClearTimeSma();

		//ランク
		std::string fileNameLank[] = { "S", "A", "B", "C"};
		int playStage = pSceneManager->GetPlayStage() - 1;
		for (int i = 0; i < 3; i++) {
			if (clearTime_ > stageClearTime[playStage][i]) {
				lank = i;
				break;
			}
		}

		hPictLank_ = Image::Load("Png/" + fileNameLank[lank] + ".png");
		assert(hPictLank_ >= 0);


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
	AudioManager::Initialize(AudioManager::TITLE);
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

	//クリアタイム表示
	if (result_) {
		int firstDigit = clearTime_ % 10;                          //一の位
		int secondDigit = (clearTime_ / 10) % 10;                  //十の位の数字を取得
		int third = (clearTime_ / 100);

		//左の一の位
		Transform pic1 = transform_;
		pic1.scale_.x += 1.5f;
		pic1.scale_.y += 1.5f;
		pic1.position_ = { 0.0f, 0.6f, 0.0f };
		Image::SetTransform(hPictTime_[firstDigit], pic1);
		Image::Draw(hPictTime_[firstDigit]);

		//左の十の位
		Transform pic2 = pic1;
		pic2.position_.x -= 0.09f;
		Image::SetTransform(hPictTime_[secondDigit], pic2);
		Image::Draw(hPictTime_[secondDigit]);

		//左の百の位
		Transform pic3 = pic1;
		pic3 = pic1;
		pic3.position_.x -= 0.18f;
		Image::SetTransform(hPictTime_[third], pic3);
		Image::Draw(hPictTime_[third]);

		//ピリオド
		Transform picPeriod = pic1;
		picPeriod.scale_.x = 1.3f;
		picPeriod.scale_.y = 1.3f;
		picPeriod.position_.x += 0.06f;
		picPeriod.position_.y -= 0.03f;
		Image::SetTransform(hPictTime_[11], picPeriod);
		Image::Draw(hPictTime_[11]);

		firstDigit = clearTimeSma_ % 10;                             //１行目
		secondDigit = (clearTimeSma_ / 10) % 10;                     //2桁目の数字を取得

		//右の一の位
		pic2.position_.x += 0.18f;
		pic2.position_.y += -0.03f;
		pic2.scale_.x = 1.3f;
		pic2.scale_.y = 1.3f;
		Image::SetTransform(hPictTime_[secondDigit], pic2);
		Image::Draw(hPictTime_[secondDigit]);

		//右の十の位
		pic1.position_.x += 0.14f;
		pic1.position_.y += -0.03f;
		pic1.scale_.x = 1.3f;
		pic1.scale_.y = 1.3f;
		Image::SetTransform(hPictTime_[firstDigit], pic1);
		Image::Draw(hPictTime_[firstDigit]);

		Transform lank;
		lank.scale_ = { 0.2f, 0.2f, 0 };
		lank.position_ = { -0.35f, 0.6f, 0.0f };
		Image::SetTransform(hPictLank_, lank);
		Image::Draw(hPictLank_);

	}

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