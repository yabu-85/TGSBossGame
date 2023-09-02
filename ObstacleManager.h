#pragma once
#include "Engine/GameObject.h"
#include "Obstacle.h"
#include <vector>
#include "Engine/CsvReader.h"

class Player;

class ObstacleManager : public GameObject
{
    // ��Q���ԍ�
    enum ObstacleType
    {
        OBSTACLE_NORMAL = 0,
        OBSTACLE_WALL,
        OBSTACLE_UFO,
        OBSTACLE_ROBOT,
        OBSTACLE_RASER,
        OBSTACLE_MAX,
    };

    std::vector<Obstacle*> obstacles_;
    CsvReader csv_;
    int width_;
    int height_;
    int activationZone_;
    int activationZoneSub_;
    bool isActive_;
    Player* pPlayer_;

    void InitCsv();

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

    //�͈͓��̏�Q����Active��
    void LoadCsv();

    //Obstacles��list���擾
    std::vector<Obstacle*> GetObstacleList() { return obstacles_; }

    void a();

    void SetAllObstacleActive(bool b);

};

