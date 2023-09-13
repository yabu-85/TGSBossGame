#include "PlayerSpeedController.h"
#include "Engine/Image.h"
#include "Engine/Model.h"

namespace {
    const int drawSpeed = 25;
    const int speedUpTime = 300;
    int speedUpPngDraw = 0;
    int drawTime = 0;
    float upSpeed = 0.2f;
}

PlayerSpeedController::PlayerSpeedController(GameObject* parent)
    :GameObject(parent, "PlayerSpeedController"), hPict_{ -1, -1, -1 ,-1}, moveSpeedUp_(1.0f), runTime_(0), isUp_(true)
{
}

PlayerSpeedController::~PlayerSpeedController()
{
}

void PlayerSpeedController::Initialize()
{
    std::string fileName[] = { "SpeedUp", "SpeedDown", "SpeedGauge", "SpeedGaugeFrame" };
    for (int i = 0; i < 4; i++) {
        hPict_[i] = Image::Load("Png/" + fileName[i] + ".png");
        assert(hPict_[i] >= 0);
    }

}

void PlayerSpeedController::Update()
{
    if (runTime_ > (int)(moveSpeedUp_ * speedUpTime)) {
        runTime_ = 0;
        moveSpeedUp_ += upSpeed;
        speedUpPngDraw = 60;
        isUp_ = true;
    }

}

void PlayerSpeedController::Draw()
{
    Transform gauge;
    gauge.scale_.y = 0.6f;
    gauge.position_.x = 0.1f;
    gauge.position_.y = -0.8f;
    float hpGauge = (float)((runTime_ * 100) / (moveSpeedUp_ * speedUpTime)) * 0.01f;  //hp50 = 1
    gauge.scale_.x = hpGauge;
    Image::SetTransform(hPict_[2], gauge);
    Image::Draw(hPict_[2]);

    gauge.scale_.x = 1.0f;
    Image::SetTransform(hPict_[3], gauge);
    Image::Draw(hPict_[3]);

    //スピードアップ時の画面効果
    if (speedUpPngDraw > 0) {
        Transform pict;

        if (isUp_)
        {
            pict.scale_.x = GetSystemMetrics(SM_CXSCREEN) / Image::GetTextureSize(hPict_[0]).x;
            pict.scale_.y = GetSystemMetrics(SM_CYSCREEN) / Image::GetTextureSize(hPict_[0]).y;
            Image::SetRect(hPict_[0], 0, drawTime, 1920, 1280);
            Image::SetAlpha(hPict_[0], (int)(255 * (float)(speedUpPngDraw / 60.0f)));
            Image::SetTransform(hPict_[0], pict);
            Image::Draw(hPict_[0]);
            drawTime += drawSpeed;
        }
        else
        {
            pict.scale_.x = GetSystemMetrics(SM_CXSCREEN) / Image::GetTextureSize(hPict_[1]).x;
            pict.scale_.y = GetSystemMetrics(SM_CYSCREEN) / Image::GetTextureSize(hPict_[1]).y;
            Image::SetRect(hPict_[1], 0, drawTime, 1920, 1280);
            Image::SetAlpha(hPict_[1], (int)(255 * (float)(speedUpPngDraw / 60.0f)));
            Image::SetTransform(hPict_[1], pict);
            Image::Draw(hPict_[1]);
            drawTime -= drawSpeed;
        }

        speedUpPngDraw--;
    }
    Model::DrawOrder();

}

void PlayerSpeedController::Release()
{
}

void PlayerSpeedController::ResetSpeed(bool b)
{
    moveSpeedUp_ = 1.0f;
    runTime_ = 0;
    isUp_ = false;
    if (b) speedUpPngDraw = 60;
    else speedUpPngDraw = 0;

}