#pragma once
#include "Engine/GameObject.h"
#include "Obstacle.h"
#include <vector>
#include "Engine/CsvReader.h"

class Player;

// 障害物番号
enum ObstacleType
{
    OBSTACLE_NORMAL = 0,
    OBSTACLE_WALL,
    OBSTACLE_UFO,
    OBSTACLE_ROBOT,
};

class ObstacleManager : public GameObject
{
    std::vector<Obstacle*> obstacles_;
    CsvReader csv_;
    int width_;
    int height_;
    int activationZone_;
    int activationZoneSub_;
    Player* pPlayer_;

    void InitCsv();

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

    //範囲内の障害物をActiveに
    void LoadCsv();

    //Obstaclesのlistを取得
    std::vector<Obstacle*> GetObstacleList() { return obstacles_; }

    void a();
};

