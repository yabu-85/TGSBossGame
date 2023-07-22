#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"
#include "PauseMenu.h"

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

	pPlayer->SetPosition(pStage->GetPlaPos());

	Instantiate<PauseMenu>(this);
}

//更新
void PlayScene ::Update()
{
}

//描画
void PlayScene ::Draw()
{
}

//開放
void PlayScene ::Release()
{
}