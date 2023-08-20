#pragma once
#include "Obstacle.h"

class UfoObstacle : public Obstacle
{
public:
    UfoObstacle(GameObject* parent);
    ~UfoObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // Õ“Ë”»’è‚ğ•t—^
    void OnCollision(GameObject* pTarget) override;
};

