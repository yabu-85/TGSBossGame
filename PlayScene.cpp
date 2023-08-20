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

//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), pTimer_(nullptr), pPlayer_(nullptr)
{
}

//初期化
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

//更新
void PlayScene ::Update()
{
	if (Input::IsKeyDown(DIK_E) && !FindObject("PauseMenu")) {
		Instantiate<PauseMenu>(this);
		Player* pPlayer = (Player*)FindObject("Player");
		pPlayer->SetActive(false);
	}

	//ゲームオーバー
	if (pTimer_->IsFinished() || pPlayer_->GetHp() <= 0) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->SetResult(false);
		pSceneManager->ChangeScene(SCENE_ID_RESULT);
	}

	//ゲームクリア
	if (goal <= pPlayer_->GetPosition().z) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->SetResult(true);
		pSceneManager->ChangeScene(SCENE_ID_RESULT);
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

