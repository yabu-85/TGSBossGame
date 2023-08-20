#pragma once
#include "Obstacle.h"

class MissileObstacle : public Obstacle
{
public:
    MissileObstacle(GameObject* parent);
    ~MissileObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // Õ“Ë”»’è‚ğ•t—^
    void OnCollision(GameObject* pTarget) override;
};

