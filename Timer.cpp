#include "Timer.h"
#include "Engine/Input.h"

Timer::Timer(GameObject* parent)
    :GameObject(parent, "Timer"), pNum_(nullptr),
   frame_(0), active_(false), drawX_(10), drawY_(20), hPict_{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
{
}

Timer::~Timer()
{
}

void Timer::Initialize()
{
    pNum_ = new Text;
    pNum_->Initialize();

    for (int i = 0; i < 10; i++) {
        std::string fileName = "Time";
        fileName = fileName + std::to_string( (int)(i % 2 + 1) );
        fileName = fileName + ".png";
        hPict_[i] = Image::Load(fileName);
        assert(hPict_[i] >= 0);
    }

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

    //一桁目取得
    int firstDigit = sec % 10;
    int secondDigit = (sec / 10) % 10; // 2桁目の数字を取得
    int thirdDigit = sec / 100; // 3桁目の数字を取得

    Transform pic;
    pic.scale_.x = 0.2f;
    pic.scale_.y = 0.2f;
    Image::SetTransform(hPict_[firstDigit], pic);
    Image::Draw(hPict_[firstDigit]);

    static int time = 100;
    time -= (100.0 / FPS);
    if (time < 0) time = 100;

    pNum_->Draw(drawX_ + 500, drawY_, time );
}

//開放
void Timer::Release()
{
}
