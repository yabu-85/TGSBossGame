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
    std::vector<Obstacle*> obstacles_;  //Obstacle �N���X�̃|�C���^�̃x�N�^�[

public:
    ObstacleManager(GameObject* parent);
    ~ObstacleManager();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void addObstacle(Obstacle* _obstacle);

    // Obstacle �N���X�̃C���X�^���X�� ObstacleManager ���琶�����Ēǉ�
    void createAndAddObstacle(int _x, int _y, int _z, ObstacleType _type);

};

