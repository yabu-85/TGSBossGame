#pragma once
#include "Engine/GameObject.h"
#include "Player.h"

class HpGauge :
    public GameObject
{
    int hPict_[2];    //‰æ‘œ”Ô†
    Transform pic0Pos;
    Transform pic1Pos;
    Player* pPlayer;

public:
    HpGauge(GameObject* parent);
    ~HpGauge();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

