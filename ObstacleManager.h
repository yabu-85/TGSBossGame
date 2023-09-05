#pragma once
#include "Engine/GameObject.h"
#include "Obstacle.h"
#include <vector>
#include "Engine/CsvReader.h"

class Player;

class RobotObstacle;
class UfoObstacle;

class ObstacleManager : public GameObject
{
    // 障害物番号
    enum ObstacleType
    {
        OBSTACLE_NORMAL = 0,
        OBSTACLE_WALL,
        OBSTACLE_UFO,
        OBSTACLE_ROBOT,
        OBSTACLE_RASER,
        OBSTACLE_BOSS,
        OBSTACLE_MAX,
    };

    std::vector<Obstacle*> obstacles_;
    std::vector<Obstacle*> inactiveObstacles_;
    CsvReader csv_;
    int width_;
    int height_;
    int activationZone_;
    int activationZoneSub_;
    bool isActive_;
    Player* pPlayer_;

public:
    ObstacleManager(GameObject* parent);
    ~ObstacleManager();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //障害物初期配置
    void InitCsv(std::string mp);

    //リストから削除
    void removeObstacle(Obstacle* _obstacle);
    //リストから削除
    void removeInActiveObstacle(Obstacle* _obstacle);

    // 障害物を生成して追加する
    void createAndAddObstacle(XMFLOAT3 _position, ObstacleType _type);

    //範囲内の障害物をActiveに
    void LoadCsv();

    //Obstaclesのlistを取得
    std::vector<Obstacle*> GetObstacleList() { return obstacles_; }

    void a();

    void SetAllObstacleActive(bool b);

};

