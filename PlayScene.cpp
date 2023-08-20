#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"
#include "PauseMenu.h"
#include "Engine/Input.h"
#include "ExitMenu.h"
#include "ObstacleManager.h"

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
}

//������
void PlayScene::Initialize()
{
	Stage* pStage = Instantiate<Stage>(this);
	Player* pPlayer = Instantiate<Player>(this);
	Instantiate<ObstacleManager>(this);

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
}

//�`��
void PlayScene ::Draw()
{
}

//�J��
void PlayScene ::Release()
{
}

