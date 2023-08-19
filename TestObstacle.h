#pragma once
#include "Obstacle.h"

class TestObstacle :
    public Obstacle
{
public:
    TestObstacle(GameObject* parent);
    ~TestObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

