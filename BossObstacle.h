#pragma once
#include "Obstacle.h"

class BossObstacle : public Obstacle
{
    enum STATE {
        S_ENTER = 0, //�o��

    }
    state_;
    bool stateEnter_;
    void UpdateEnter();
    void ChangeState(STATE s);


public:
    BossObstacle(GameObject* parent);
    ~BossObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};
