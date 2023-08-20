#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"
#include "PauseMenu.h"
#include "Timer.h"
#include "Engine/Input.h"
#include "ExitMenu.h"
#include "ObstacleManager.h"
#include "Engine/SceneManager.h"

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), pTimer_(nullptr)
{
}

//������
void PlayScene::Initialize()
{
	Stage* pStage = Instantiate<Stage>(this);
	Player* pPlayer = Instantiate<Player>(this);
	Instantiate<ObstacleManager>(this);
	pTimer_ = Instantiate<Timer>(this);
	pTimer_->SetLimit(10);
	pTimer_->Start();

	pPlayer->SetPosition(pStage->GetPlaPos());
	pPlayer->SetActiveWithDelay(true);
}

//�X�V
void PlayScene ::Update()
{
	if (Input::IsKeyDown(DIK_E) && !FindObject("PauseMenu")) {
		Instantiate<PauseMenu>(this);
		Player* pPlayer = (Player*)FindObject("Player");
		pPlayer->SetActive(false);
	}

	if (pTimer_->IsFinished()) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
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

