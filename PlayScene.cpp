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

#define SAFE_DELETE(p) if(p != nullptr){ p = nullptr; delete p;}

namespace {
	int goal;
	int stopTime;	//resultシーン移行までの待機時間
	int result;		//1=クリア 2=ゲームオーバー

	//ステージごとの設定
	std::string stageName[] = { "Map1", "Map2" };
	int stageTime[] = { 80, 50 };

}

//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), pTimer_(nullptr), pPlayer_(nullptr), hPict_(-1)
{
}

//初期化
void PlayScene::Initialize()
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	int stage = pSceneManager->GetPlayStage() - 1;

	Stage* pStage = Instantiate<Stage>(this);
 	pStage->InitMapData(stageName[stage]);
	goal = pStage->GetHeight();
	stopTime = 0;
	result = 0;

	pPlayer_ = Instantiate<Player>(this);
	Instantiate<SkyBox>(this);
	ObstacleManager* pObstacleManager = Instantiate<ObstacleManager>(this);
	pObstacleManager->InitCsv(stageName[stage]);

	pTimer_ = Instantiate<Timer>(this);
	pTimer_->SetLimit(stageTime[stage]);
	pTimer_->Start();

	//描画順調整のためここで（他のも/Drawで優先度決めれるようにしたい
	Instantiate<HpGauge>(this);
	Instantiate<Aim>(this);
	Instantiate<PlayerSpeedController>(this);

	pPlayer_->SetPosition(pStage->GetPlaPos());
	pPlayer_->SetActiveWithDelay(true, 10); //クリックの動作を入れないように遅延

	hPict_ = Image::Load("Png/Black.png");
	assert(hPict_ >= 0);

	AudioManager::Release();
	AudioManager::Initialize(AudioManager::PLAY);
}

//更新
void PlayScene::Update()
{
	if (Input::IsKeyDown(DIK_E) && !FindObject("PauseMenu")) {
		Instantiate<PauseMenu>(this);
	}

	//ゲームオーバー
	if (pTimer_->IsFinished() || pPlayer_->GetHp() <= 0) result = 1;
	//ゲームクリア
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

//描画
void PlayScene::Draw()
{
}

//開放
void PlayScene::Release()
{
	SAFE_DELETE(pTimer_);
	SAFE_DELETE(pPlayer_);
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