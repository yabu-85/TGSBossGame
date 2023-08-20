#pragma once
#include "Engine/GameObject.h"
#include "Player.h"

class HpGauge :
    public GameObject
{
    int hPict_[2];    //�摜�ԍ�
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

