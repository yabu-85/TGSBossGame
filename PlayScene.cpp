#include "PlayScene.h"
#include "Timer.h"
#include "Player.h"
#include "Stage.h"
#include "PauseMenu.h"
#include "Engine/Input.h"
#include "ExitMenu.h"
#include "ObstacleManager.h"
#include "Engine/SceneManager.h"
#include "SkyBox.h"
#include "Aim.h"
#include "HpGauge.h"
#include "PlayerSpeedController.h"
#include "AudioManager.h"
#include "Engine/Model.h"

namespace {
	int goal;
	int stopTime;	//result�V�[���ڍs�܂ł̑ҋ@����
	bool result;	//1=�N���A 2=�Q�[���I�[�o�[

}

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), pTimer_(nullptr), pPlayer_(nullptr), hPict_(-1)
{
}

//������
void PlayScene::Initialize()
{
	Stage* pStage = Instantiate<Stage>(this);
	goal = pStage->GetHeight();
	stopTime = 100;
	result = 0;

	pPlayer_ = Instantiate<Player>(this);
	Instantiate<SkyBox>(this);
	Instantiate<ObstacleManager>(this);

	pTimer_ = Instantiate<Timer>(this);
	pTimer_->SetLimit(100);
	pTimer_->Start();

	//�`�揇�����̂��߂����Łi���̂�/Draw�ŗD��x���߂��悤�ɂ�����
	Instantiate<HpGauge>(this);
	Instantiate<Aim>(this);
	Instantiate<PlayerSpeedController>(this);

	pPlayer_->SetPosition(pStage->GetPlaPos());
	pPlayer_->SetActiveWithDelay(true, 10); //�N���b�N�̓�������Ȃ��悤�ɒx��

	hPict_ = Image::Load("Png/Black.png");
	assert(hPict_ >= 0);

	AudioManager::Release();
	AudioManager::Initialize(AudioManager::PLAY);
}

//�X�V
void PlayScene::Update()
{
	if (Input::IsKeyDown(DIK_E) && !FindObject("PauseMenu")) {
		Instantiate<PauseMenu>(this);
	}

	//�Q�[���I�[�o�[
	if (pTimer_->IsFinished() || pPlayer_->GetHp() <= 0) result = 1;
	//�Q�[���N���A
	if (goal <= pPlayer_->GetPosition().z) result = 2;

	if (result >= 1) {
		stopTime--;
		SetObjectActive(false);

		if (stopTime <= 0) {
			if (result == 1) {
				SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
				pSceneManager->SetResult(false);
				pSceneManager->ChangeScene(SCENE_ID_RESULT);
			}

			if (result == 2) {
				SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
				pSceneManager->SetResult(true);
				pSceneManager->ChangeScene(SCENE_ID_RESULT);
			}
		}
	}

	if (Input::IsKeyDown(DIK_LEFTARROW)) pTimer_->Stop();
	if (Input::IsKeyDown(DIK_RIGHTARROW)) pTimer_->Start();

}

//�`��
void PlayScene::Draw()
{
	Image::SetAlpha(hPict_, 255);
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);

}

//�J��
void PlayScene::Release()
{
}

void PlayScene::SetObjectActive(bool _active) {
	Aim* pAim = (Aim*)FindObject("Aim");
	pAim->SetAimMove(_active);

	pPlayer_->SetActive(_active);
	Model::SetAnimeStop(pPlayer_->GetModelHandle(), !_active);

	if (_active) pTimer_->Start();
	else pTimer_->Stop();

	ObstacleManager* pObstacleManager = (ObstacleManager*)FindObject("ObstacleManager");
	pObstacleManager->SetAllObstacleActive(_active);

}