#pragma once
#include "Engine/GameObject.h"
#include "Obstacle.h"
#include <vector>

// è·äQï®î‘çÜ
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

    // è·äQï®Çí«â¡Ç∑ÇÈ
    void addObstacle(Obstacle* _obstacle);

    // è·äQï®Çê∂ê¨ÇµÇƒí«â¡Ç∑ÇÈ
    void createAndAddObstacle(XMFLOAT3 _position, ObstacleType _type);

};

