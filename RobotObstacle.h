#pragma once
#include "Obstacle.h"

class Player;

class RobotObstacle :
    public Obstacle
{
    bool backMove_;
    int nearestLocation_;
    Player* pPlayer_;

    void Rotate();

public:
    RobotObstacle(GameObject* parent);
    ~RobotObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // è’ìÀîªíËÇïtó^
    void OnCollision(GameObject* pTarget) override;

};

