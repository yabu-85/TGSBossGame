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
	Instantiate<SkyBox>(this);
	Instantiate<ObstacleManager>(this);

	pTimer_ = Instantiate<Timer>(this);
	pTimer_->SetLimit(30);
	pTimer_->Start();

	//描画順調整のためここで（他のも/Drawで優先度決めれるようにしたい
	Instantiate<HpGauge>(this);
	Instantiate<Aim>(this);
	Instantiate<PlayerSpeedController>(this);

	pPlayer_->SetPosition(pStage->GetPlaPos());
	pPlayer_->SetActiveWithDelay(true, 10); //クリックの動作を入れないように遅延
}

//更新
void PlayScene::Update()
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

	if (Input::IsKeyDown(DIK_LEFTARROW)) {
		pTimer_->Stop();
	}

	if (Input::IsKeyDown(DIK_RIGHTARROW)) {
		pTimer_->Start();
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

