#pragma once
#include "Engine/GameObject.h"
#include "Obstacle.h"
#include <vector>

// ��Q���ԍ�
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

    // ��Q����ǉ�����
    void addObstacle(Obstacle* _obstacle);

    // ��Q���𐶐����Ēǉ�����
    void createAndAddObstacle(XMFLOAT3 _position, ObstacleType _type);

};

