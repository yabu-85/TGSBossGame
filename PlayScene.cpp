#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"
#include "PauseMenu.h"
#include "Engine/Input.h"
#include "ExitMenu.h"
#include "ObstacleManager.h"

//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
}

//初期化
void PlayScene::Initialize()
{
	Stage* pStage = Instantiate<Stage>(this);
	Player* pPlayer = Instantiate<Player>(this);
	Instantiate<ObstacleManager>(this);

	pPlayer->SetPosition(pStage->GetPlaPos());
	pPlayer->SetActiveWithDelay(true);
}

//更新
void PlayScene ::Update()
{
	if (Input::IsKeyDown(DIK_E) && !FindObject("PauseMenu")) {
		Instantiate<PauseMenu>(this);
		Player* pPlayer = (Player*)FindObject("Player");
		pPlayer->SetActive(false);
	}
}

//描画
void PlayScene ::Draw()
{
}

//開放
void PlayScene ::Release()
{
}

