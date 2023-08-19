#include "PlayScene.h"
#include "Timer.h"
#include "Player.h"
#include "Stage.h"
#include "PauseMenu.h"
#include "Engine/Input.h"
#include "ExitMenu.h"
#include "ObstacleManager.h"

//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), pText_(nullptr), clearTime_(0), count_(0)
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

	//※テキスト類はモデルよりも前面に描画(後に呼ぶ)
	Timer* pTimer = Instantiate<Timer>(this);
	pTimer->SetLimit(5);		//タイマーを設定
	pText_ = new Text;			//テキスト
	pText_->Initialize();
}

//更新
void PlayScene::Update()
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
	//描画
	pText_->SetScale(3.0f);
	pText_->Draw(300, 200, clearTime_);
}

//開放
void PlayScene ::Release()
{
}

//タイマー
void PlayScene::TimeProcess()
{
	Timer* pTimer = (Timer*)FindObject("Timer");

	//タイマーを起動(初回押下時のみ)
	if (Input::IsKeyDown(DIK_W)&& count_==0)
	{
		pTimer->Start();
		count_++;
	}

	//ゲームをクリアした場合
	if (Input::IsKeyDown(DIK_Y))
	{
		//タイマー停止
		pTimer->Stop();
		clearTime_ = pTimer->GetTime();////////クリアタイム取得確認用｡ここじゃなくてオーバーシーンで使うと思う
	}
	
	//タイマーが終了した場合
	if (pTimer->IsFinished())
	{
		////////(ゲームオーバー)ダウンアニメ再生して､シーン切り替えとかやるところ
		//Player* pPlayer = (Player*)FindObject("Player");
		//pPlayer->KillMe();
	}
}