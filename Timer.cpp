#include "Timer.h"
#include "Engine/Input.h"

//フレームレート
static const int FPS = 60;

//コンストラクタ
Timer::Timer(GameObject* parent)
    :GameObject(parent, "Timer"), pNum(nullptr),
    Frame(0), Active(false), drawX(10), drawY(20)
{
}

//デストラクタ
Timer::~Timer()
{
}

//初期化
void Timer::Initialize()
{
    pNum = new Text;
    pNum->Initialize();
}

//更新
void Timer::Update()
{
    //起動状態
    if (Active)
    {
        //カウントダウン
        if (Frame > 0) Frame--;
    }
}

//描画
void Timer::Draw()
{

    pNum->SetScale(1.0f);                                   //テキストのサイズ
    pNum->Draw(drawX, drawY, "Time:");                      //描画内容

    if (Frame % FPS < 10)                                   //数字の躍動感
        pNum->SetScale((Frame % FPS) * 0.2f + 1.0f);        //規定フレーム以下時に適応
    else
        pNum->SetScale(1.0f);

    int sec = Frame / FPS;                                  //秒数
    pNum->Draw(drawX + 100, drawY, sec);
}

//開放
void Timer::Release()
{
}

//タイマー設定
void Timer::SetLimit(float seconds)
{
    Frame = (int)(seconds * FPS);
}

//タイマー開始
void Timer::Start()
{
    Active = true;
}

//タイマー終了
void Timer::Stop()
{
    Active = false;
}

//タイマー処理終了
bool Timer::IsFinished()
{
    return (Frame == 0);
}
