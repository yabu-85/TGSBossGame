#include "Timer.h"
#include "Engine/Input.h"

Timer::Timer(GameObject* parent)
    :GameObject(parent, "Timer"), frame_(0), active_(false), drawX_(10), drawY_(20),
    hPict_{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, calcTime_(0)
{
}

Timer::~Timer()
{
}

void Timer::Initialize()
{
    for (int i = 0; i < 10; i++) {
        std::string fileName = "Png/Time/Time";
        fileName = fileName + std::to_string(i);
        fileName = fileName + ".png";
        hPict_[i] = Image::Load(fileName);
        assert(hPict_[i] >= 0);
    }
    hPict_[10] = Image::Load("Png/Time/Colon.png");
    assert(hPict_[10] >= 0);

    hPict_[11] = Image::Load("Png/Time/Period.png");
    assert(hPict_[11] >= 0);
}

void Timer::Update()
{
    //起動状態
    if (active_)
    {
        //カウントダウン
        if (frame_ > 0)
            frame_--;

        calcTime_ -= (100.0 / FPS);
        if (calcTime_ < 0) calcTime_ = 100;

    }
}

//描画
void Timer::Draw()
{
    int frameCount = frame_ % FPS;                      //一秒ごとリセット
    float size = frameCount * 0.2f + 1.0f;
    //タイマー数値表示
    if (frameCount < 10)                                //躍動感
        transform_.scale_ = { size, size, size };       //規定フレーム以下時に適応
    else
        transform_.scale_ = { 1.0f, 1.0f, 1.0f};
    int sec = frame_ / FPS;                             //秒数

    int firstDigit = sec % 10;                          //一の位
    int secondDigit = (sec / 10) % 10;                  //十の位の数字を取得

    //左の一の位
    Transform pic1 = transform_;
    pic1.scale_.x += 1.2f;
    pic1.scale_.y += 1.2f;
    pic1.position_ = { -0.1f, 0.8, 0.0f };
    Image::SetTransform(hPict_[firstDigit], pic1);
    Image::Draw(hPict_[firstDigit]);
    
    //左の十の位
    Transform pic2 = pic1;
    pic2.position_.x -= 0.09f;
    Image::SetTransform(hPict_[secondDigit], pic2);
    Image::Draw(hPict_[secondDigit]);

    //ピリオド
    Transform picPeriod = pic1;
    picPeriod.scale_.x = 1.3f;
    picPeriod.scale_.y = 1.3f;
    picPeriod.position_.x += 0.06f;
    picPeriod.position_.y -= 0.03f;
    Image::SetTransform(hPict_[11], picPeriod);
    Image::Draw(hPict_[11]);

    firstDigit = calcTime_ % 10;                             //１行目
    secondDigit = (calcTime_ / 10) % 10;                     //2桁目の数字を取得

    //右の一の位
    pic2.position_.x += 0.18f;
    pic2.position_.y += -0.03f;
    pic2.scale_.x = 1.3f;
    pic2.scale_.y = 1.3f;
    Image::SetTransform(hPict_[secondDigit], pic2);
    Image::Draw(hPict_[secondDigit]);

    //右の十の位
    pic1.position_.x += 0.14f;
    pic1.position_.y += -0.03f;
    pic1.scale_.x = 1.3f;
    pic1.scale_.y = 1.3f;
    Image::SetTransform(hPict_[firstDigit], pic1);
    Image::Draw(hPict_[firstDigit]);



}

//開放
void Timer::Release()
{
}
