#pragma once
#include "Obstacle.h"

class LaserObstacle :
    public Obstacle
{
    int hModelHead_;

public:
    LaserObstacle(GameObject* parent);
    ~LaserObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

