#pragma once
#include "Obstacle.h"

class LazerObstacle :
    public Obstacle
{
    int hModelHead_;

public:
    LazerObstacle(GameObject* parent);
    ~LazerObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

