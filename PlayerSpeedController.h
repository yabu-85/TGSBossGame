#pragma once
#include "Engine/GameObject.h"

class PlayerSpeedController :
    public GameObject
{
    int hPict_[3];    //画像番号
    int runTime_;
    float moveSpeedUp_;

public:
    PlayerSpeedController(GameObject* parent);
    ~PlayerSpeedController();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void AddRunTime() { runTime_++; };
    void ResetSpeed();

    float GetMoveSpeed_() { return moveSpeedUp_; };

};