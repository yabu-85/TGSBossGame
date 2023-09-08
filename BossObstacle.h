#pragma once
#include "Obstacle.h"

class Player;

class BossObstacle : public Obstacle
{
    enum STATE {
        S_ENTER = 0, //ìoèÍ

        S_DEAD,
    }
    state_;
    bool stateEnter_;
    void UpdateEnter();
    void UpdateDead();
    void ChangeState(STATE s);

    Player* pPlayer_;

public:
    BossObstacle(GameObject* parent);
    ~BossObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

