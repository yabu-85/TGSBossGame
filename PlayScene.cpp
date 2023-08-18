#include "PlayScene.h"
#include "Timer.h"
#include "Player.h"
#include "Stage.h"
#include "PauseMenu.h"
#include "Engine/Input.h"
#include "ExitMenu.h"

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
	pPlayer->SetActiveWithDelay(true);

	//※テキストは前面になるように描画
	//タイマー設定
	Timer* pTimer = Instantiate<Timer>(this);
	pTimer->SetLimit(15);
}

//更新
void PlayScene ::Update()
{
	if (Input::IsKeyDown(DIK_E)) {
		Instantiate<PauseMenu>(this);
		Player* pPlayer = (Player*)FindObject("Player");
		pPlayer->SetActive(false);
	}

	//タイマー処理
	TimeProcess();
}

//描画
void PlayScene ::Draw()
{
}

//開放
void PlayScene ::Release()
{
}

//タイマー
void PlayScene::TimeProcess()
{
	//タイマー起動
	if (Input::IsKeyDown(DIK_W))
	{
		Timer* t = (Timer*)FindObject("Timer");
		t->Start();
	}
	//タイマー停止
	if (Input::IsKeyDown(DIK_Y))
	{
		Timer* t = (Timer*)FindObject("Timer");
		t->Stop();
	}
}