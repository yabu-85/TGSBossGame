#pragma once
#include "Obstacle.h"

class WallObstacle : public Obstacle
{
public:
    WallObstacle(GameObject* parent);
    ~WallObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // �Փ˔����t�^
    void OnCollision(GameObject* pTarget) override;
};

