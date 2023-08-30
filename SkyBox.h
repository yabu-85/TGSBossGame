#pragma once
#include "Engine/GameObject.h"

class Player;

class SkyBox :
    public GameObject
{
    int hModel_;
    Player* pPlayer_;

public:
    SkyBox(GameObject* parent);
    ~SkyBox();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

