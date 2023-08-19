#pragma once
#include "Engine/GameObject.h"
#include "Obstacle.h"
#include <vector>

// 障害物番号
enum ObstacleType 
{
    OBSTACLE_NORMAL = 0,
    OBSTACLE_SPIKE,
    OBSTACLE_WALL,
    OBSTACLE_MAX
};

class ObstacleManager : public GameObject
{
    std::vector<Obstacle*> obstacles_;

public:
    ObstacleManager(GameObject* parent);
    ~ObstacleManager();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // 障害物を追加する
    void addObstacle(Obstacle* _obstacle);

    // 障害物を生成して追加する
    void createAndAddObstacle(XMFLOAT3 _position, ObstacleType _type);

};

