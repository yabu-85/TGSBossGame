#include "ResultScene.h"
#include "ExitMenu.h"	
#include "Engine/SceneManager.h"
#include "ButtonFactory.h"
#include "Button.h"
#include "Engine/Input.h"
#include "AudioManager.h"

namespace {
	int lank = 3; //C�����N
	//������S,A,B,C�ł��̐��ȏゾ�����炻�̃����N�ɂȂ�
	int stageClearTime[2][3] = { { 60, 50, 30},
						         { 100, 10, 10} 
	};
	
}

ResultScene::ResultScene(GameObject* parent)
	:GameObject(parent, "ResultScene"), hPict_{ -1,-1,-1 }, pButtonFactory_(nullptr), clearTime_(0), clearTimeSma_(0), result_(false), hPictLank_(-1)
{
	for (int i = 0; i < 12; i++) { hPictTime_[i] = -1; };

}

//������
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

		//�����N
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
		//�N���A�����ꍇ�̃{�^��
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

//�X�V
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

//�`��
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

	//�N���A�^�C���\��
	if (result_) {
		int firstDigit = clearTime_ % 10;                          //��̈�
		int secondDigit = (clearTime_ / 10) % 10;                  //�\�̈ʂ̐������擾
		int third = (clearTime_ / 100);

		//���̈�̈�
		Transform pic1 = transform_;
		pic1.scale_.x += 1.5f;
		pic1.scale_.y += 1.5f;
		pic1.position_ = { 0.0f, 0.6f, 0.0f };
		Image::SetTransform(hPictTime_[firstDigit], pic1);
		Image::Draw(hPictTime_[firstDigit]);

		//���̏\�̈�
		Transform pic2 = pic1;
		pic2.position_.x -= 0.09f;
		Image::SetTransform(hPictTime_[secondDigit], pic2);
		Image::Draw(hPictTime_[secondDigit]);

		//���̕S�̈�
		Transform pic3 = pic1;
		pic3 = pic1;
		pic3.position_.x -= 0.18f;
		Image::SetTransform(hPictTime_[third], pic3);
		Image::Draw(hPictTime_[third]);

		//�s���I�h
		Transform picPeriod = pic1;
		picPeriod.scale_.x = 1.3f;
		picPeriod.scale_.y = 1.3f;
		picPeriod.position_.x += 0.06f;
		picPeriod.position_.y -= 0.03f;
		Image::SetTransform(hPictTime_[11], picPeriod);
		Image::Draw(hPictTime_[11]);

		firstDigit = clearTimeSma_ % 10;                             //�P�s��
		secondDigit = (clearTimeSma_ / 10) % 10;                     //2���ڂ̐������擾

		//�E�̈�̈�
		pic2.position_.x += 0.18f;
		pic2.position_.y += -0.03f;
		pic2.scale_.x = 1.3f;
		pic2.scale_.y = 1.3f;
		Image::SetTransform(hPictTime_[secondDigit], pic2);
		Image::Draw(hPictTime_[secondDigit]);

		//�E�̏\�̈�
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

//�J��
void ResultScene::Release()
{
}



void ResultScene::DrawCursor()
{
	static float screenWidth = (float)GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//�X�N���[���̕�
	static float screenHeight = (float)GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//�X�N���[���̍���

	Transform aim;
	aim.scale_.x = 0.5f;
	aim.scale_.y = 0.5f;
	aim.scale_.z = 0.5f;
	aim.position_ = Input::GetMousePositionSub();
	aim.position_ = { aim.position_.x / screenWidth, -aim.position_.y / screenHeight , 0 };

	Image::SetTransform(hPict_[2], aim);
	Image::Draw(hPict_[2]);
}