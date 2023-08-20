#include "Timer.h"
#include "Engine/Input.h"

Timer::Timer(GameObject* parent)
    :GameObject(parent, "Timer"), pNum_(nullptr), frame_(0), active_(false), drawX_(10), drawY_(20)
{
}

Timer::~Timer()
{
}

void Timer::Initialize()
{
    pNum_ = new Text;
    pNum_->Initialize();
}

void Timer::Update()
{
    //起動状態
    if (active_)
    {
        //カウントダウン
        if (frame_ > 0)
            frame_--;
    }
}

//描画
void Timer::Draw()
{
    //テキスト表示
    pNum_->SetScale(1.0f);                              //テキストのサイズ
    pNum_->Draw(drawX_, drawY_, "Time:");               //描画

    int frameCount = frame_ % FPS;                      //一秒ごとリセット
    //タイマー数値表示
    if (frameCount < 10)                                //躍動感
        pNum_->SetScale(frameCount * 0.2f + 1.0f);      //規定フレーム以下時に適応
    else
        pNum_->SetScale(1.0f);

    int sec = frame_ / FPS;                             //秒数
    pNum_->Draw(drawX_ + 100, drawY_, sec);
}

//開放
void Timer::Release()
{
}
