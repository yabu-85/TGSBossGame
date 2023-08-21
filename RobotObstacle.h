#pragma once
#include "Obstacle.h"

class Player;

class RobotObstacle :
    public Obstacle
{
    bool backMove_;
    int hModelHead_;
    int nearestLocation_;
    float count_;
    Player* pPlayer_;

    void Rotate();

public:
    RobotObstacle(GameObject* parent);
    ~RobotObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // �Փ˔����t�^
    void OnCollision(GameObject* pTarget) override;

    void SetLearestLocation();

};

