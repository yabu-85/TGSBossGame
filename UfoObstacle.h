#pragma once
#include "Obstacle.h"

class UfoObstacle : public Obstacle
{
    int hModelLa_;
    float firstMoveZ_;
    bool first_;

    float EaseOutExpo(float x) { return 1 - pow(1 - x, 5); };

public:
    UfoObstacle(GameObject* parent);
    ~UfoObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // è’ìÀîªíËÇïtó^
    void OnCollision(GameObject* pTarget) override;
};

