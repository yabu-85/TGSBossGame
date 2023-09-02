#include "PlayerSpeedController.h"
#include "Engine/Image.h"

namespace {
    static int speedUpPngDraw = 0;

}

PlayerSpeedController::PlayerSpeedController(GameObject* parent)
    :GameObject(parent, "PlayerSpeedController"), hPict_{ -1, -1, -1 }, moveSpeedUp_(1.0f), runTime_(0)
{
}

PlayerSpeedController::~PlayerSpeedController()
{
}

void PlayerSpeedController::Initialize()
{
    std::string fileName[] = { "ColorDamage", "SpeedGauge", "SpeedGaugeFrame" };
    for (int i = 0; i < 3; i++) {
        hPict_[i] = Image::Load("Png/" + fileName[i] + ".png");
        assert(hPict_[i] >= 0);
    }

}

void PlayerSpeedController::Update()
{
    if (runTime_ > (int)(moveSpeedUp_ * 300)) {
        runTime_ = 0;
        moveSpeedUp_ += 0.2f;
        speedUpPngDraw = 60;
    }


}

void PlayerSpeedController::Draw()
{
    Transform gauge;
    gauge.scale_.y = 0.6f;
    gauge.position_.x = 0.1f;
    gauge.position_.y = -0.8f;
    float hpGauge = (float)((runTime_ * 100) / (moveSpeedUp_ * 300)) * 0.01f;  //hp50 = 1
    gauge.scale_.x = hpGauge;
    Image::SetTransform(hPict_[1], gauge);
    Image::Draw(hPict_[1]);

    gauge.scale_.x = 1.0f;
    Image::SetTransform(hPict_[2], gauge);
    Image::Draw(hPict_[2]);

    //ƒ_ƒ[ƒW‰æ–ÊŒø‰Ê
    if (speedUpPngDraw > 0) {
        Transform pict;
        pict.scale_.x = GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini") / Image::GetTextureSize(hPict_[0]).x;
        pict.scale_.y = GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini") / Image::GetTextureSize(hPict_[0]).y;
        Image::SetAlpha(hPict_[0], (int)(255 * (float)(speedUpPngDraw / 60.0f)));
        Image::SetTransform(hPict_[0], pict);
        Image::Draw(hPict_[0]);

        speedUpPngDraw--;
    }
}

void PlayerSpeedController::Release()
{
}

void PlayerSpeedController::ResetSpeed()
{
    moveSpeedUp_ = 1.0f;
    runTime_ = 0;

}