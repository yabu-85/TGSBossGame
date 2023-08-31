#pragma once
#include "Obstacle.h"

class Player;

class WallObstacle : public Obstacle
{
    Player* pPlayer_;

public:
    WallObstacle(GameObject* parent);
    ~WallObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    
};

