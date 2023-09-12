#pragma once
#include "Engine/GameObject.h"

class PlayerSpeedController :
    public GameObject
{
    int hPict_[3];    //画像番号
    int runTime_;
    float moveSpeedUp_;
    bool isUp_;         //スピード上げる=true, 下がる=false

public:
    PlayerSpeedController(GameObject* parent);
    ~PlayerSpeedController();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void AddRunTime() { runTime_++; };
    void ResetSpeed(bool b = false);

    float GetMoveSpeed_() { return moveSpeedUp_; };

};