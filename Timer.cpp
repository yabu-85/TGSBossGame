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
        std::string fileName = "Png/Time/Time";
        fileName = fileName + std::to_string(i);
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

    Transform pic1;
    pic1.scale_.x = 1.2f;
    pic1.scale_.y = 1.2f;
    Image::SetTransform(hPict_[firstDigit], pic1);
    Image::Draw(hPict_[firstDigit]);

    Transform pic2 = pic1;
    pic2.position_.x = -0.2f;
    Image::SetTransform(hPict_[secondDigit], pic2);
    Image::Draw(hPict_[secondDigit]);


    static int time = 100;                              //１の位、２の位下のね
    time -= (100.0 / FPS);
    if (time < 0) time = 100;
    pNum_->Draw(drawX_ + 500, drawY_, time);

    firstDigit = time % 10;
    secondDigit = (time / 10) % 10; // 2桁目の数字を取得

    pic1.position_.x = 0.7f;
    Image::SetTransform(hPict_[firstDigit], pic1);
    Image::Draw(hPict_[firstDigit]);

    pic2.position_.x = 0.5f;
    Image::SetTransform(hPict_[secondDigit], pic2);
    Image::Draw(hPict_[secondDigit]);

}

//開放
void Timer::Release()
{
}
