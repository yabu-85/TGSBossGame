#pragma once
#include "Obstacle.h"

class UfoObstacle : public Obstacle
{
    int hModelLa_;
    float firstMoveZ_;
    bool first_;
    float move_;
    float maxMoveX_;
    bool x_;
    bool firstAct_;

    float EaseOutExpo(float x) { return 1.0f - pow(1 - x, 5); };

public:
    UfoObstacle(GameObject* parent);
    ~UfoObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // �Փ˔����t�^
    void OnCollision(GameObject* pTarget) override;
};

