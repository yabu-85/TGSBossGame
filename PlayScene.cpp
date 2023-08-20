#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"
#include "PauseMenu.h"
#include "Timer.h"
#include "Engine/Input.h"
#include "ExitMenu.h"
#include "ObstacleManager.h"
#include "Engine/SceneManager.h"

static int goal;

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), pTimer_(nullptr), pPlayer_(nullptr)
{
}

//������
void PlayScene::Initialize()
{
	Stage* pStage = Instantiate<Stage>(this);
	goal = pStage->GetHeight();

	pPlayer_ = Instantiate<Player>(this);
	Instantiate<ObstacleManager>(this);

	pTimer_ = Instantiate<Timer>(this);
	pTimer_->SetLimit(10);
	pTimer_->Start();

	pPlayer_->SetPosition(pStage->GetPlaPos());
	pPlayer_->SetActiveWithDelay(true);
}

//�X�V
void PlayScene ::Update()
{
	if (Input::IsKeyDown(DIK_E) && !FindObject("PauseMenu")) {
		Instantiate<PauseMenu>(this);
		Player* pPlayer = (Player*)FindObject("Player");
		pPlayer->SetActive(false);
	}

	//�Q�[���I�[�o�[
	if (pTimer_->IsFinished() || pPlayer_->GetHp() <= 0) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->SetResult(false);
		pSceneManager->ChangeScene(SCENE_ID_RESULT);
	}

	//�Q�[���N���A
	if (goal <= pPlayer_->GetPosition().z) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->SetResult(true);
		pSceneManager->ChangeScene(SCENE_ID_RESULT);
	}
}

//�`��
void PlayScene ::Draw()
{
}

//�J��
void PlayScene ::Release()
{
}

