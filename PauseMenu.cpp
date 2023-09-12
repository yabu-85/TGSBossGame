#include "PauseMenu.h"
#include "Engine/Input.h"
#include "ButtonFactory.h"
#include "ExitMenu.h"
#include "Engine/SceneManager.h"
#include "AudioManager.h"
#include "PlayScene.h"
#include <future>
#include "Setting.h"

//���j���[�N���X�����Ƃ���
//Pause�ASetting�N���X���p�����č�邱�ƂɂȂ�@�Ǝv��

//Pause�̓v���C�V�[������ŏo��
//Settign��ʂ͂��ׂẴV�[���ŏo��
//��Ƃ�Play�V�[���̓������~�߂邱�Ƃ͊m��

//Setting��ʂɕK�v�ȋ@�\��΂Ȃ��
//�}�E�X���x�ƁA�L�[�R���t�B�O�̐ݒ肪�~����
//���Ɖ��ʂ̐ݒ�A


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
	pButtonFactory_->ButtonCreate(0.0f, 0.0f, 1.0f, 1.0f, "Setting");
	pButtonFactory_->ButtonCreate(0.0f, -300.0f, 1.0f, 1.0f, "ReturnTitle");
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
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //�ҋ@
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);

	}

}

void PauseMenu::Draw()
{

	Image::SetAlpha(hPict_[0], 100);
	Image::SetTransform(hPict_[0], transform_);
	Image::Draw(hPict_[0]);

	Transform abb;
	abb.position_ = Input::GetMousePositionSub();
	static float screenWidth = (float)GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//�X�N���[���̕�
	static float screenHeight = (float)GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//�X�N���[���̍���
	abb.position_ = { abb.position_.x / screenWidth, -abb.position_.y / screenHeight , 0 };

	Image::SetTransform(hPict_[1], abb);
	Image::Draw(hPict_[1]);
}

void PauseMenu::Release()
{
}