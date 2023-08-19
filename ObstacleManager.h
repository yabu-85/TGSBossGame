#pragma once
#include "Engine/GameObject.h"
#include "Obstacle.h"
#include <vector>

enum ObstacleType {
    OBSTACLE_NORMAL = 0,
    OBSTACLE_SPIKE,
    OBSTACLE_WALL

};

class ObstacleManager :
    public GameObject
{
    std::vector<Obstacle*> obstacles_;  //Obstacle クラスのポインタのベクター

public:
    ObstacleManager(GameObject* parent);
    ~ObstacleManager();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void addObstacle(Obstacle* _obstacle);

    // Obstacle クラスのインスタンスを ObstacleManager から生成して追加
    void createAndAddObstacle(int _x, int _y, int _z, ObstacleType _type);

};

