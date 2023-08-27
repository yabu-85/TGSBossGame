#include "HpGauge.h"
#include "Engine/Image.h"
#include "Player.h"

HpGauge::HpGauge(GameObject* parent)
	:GameObject(parent, "HpGauge"), hPict_{ -1, -1 }, pPlayer(nullptr)
{
}

HpGauge::~HpGauge()
{
}

void HpGauge::Initialize()
{
    const char* fileName[] = { "Png/hpGaugeA0.png","Png/hpGaugeA1.png" };

    //モデルデータのロード
    for (int i = 0; i < 2; i++) { //壁と床のロード
        hPict_[i] = Image::Load(fileName[i]);
        assert(hPict_[i] >= 0);
    }

    pPlayer = (Player*)FindObject("Player");

    pic0Pos.scale_.y = 0.6f;
    pic0Pos.position_.x = -0.95f;
    pic0Pos.position_.y = -0.8f;

    pic1Pos = pic0Pos;
}

void HpGauge::Update()
{
}

void HpGauge::Draw()
{

    int hp = pPlayer->GetHp();
    int hpMax = pPlayer->GetMaxHp();

    float hpGauge = (float)((hp * 100) / hpMax) * 0.01f;  //hp50 = 1
    pic0Pos.scale_.x = hpGauge;

    Image::SetTransform(hPict_[0], pic0Pos);
    Image::Draw(hPict_[0]);

    Image::SetTransform(hPict_[1], pic1Pos);
    Image::Draw(hPict_[1]);
}

void HpGauge::Release()
{
}
