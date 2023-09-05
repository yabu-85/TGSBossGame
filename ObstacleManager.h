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
    // è·äQï®î‘çÜ
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

    // è·äQï®Çí«â¡Ç∑ÇÈ
    void addObstacle(Obstacle* _obstacle);

    void removeObstacle(Obstacle* _obstacle);
    void removeObstacle(RobotObstacle* _obstacle);
    void removeObstacle(UfoObstacle* _obstacle);

    // è·äQï®Çê∂ê¨ÇµÇƒí«â¡Ç∑ÇÈ
    void createAndAddObstacle(XMFLOAT3 _position, ObstacleType _type);

    //îÕàÕì‡ÇÃè·äQï®ÇActiveÇ…
    void LoadCsv();

    //ObstaclesÇÃlistÇéÊìæ
    std::vector<Obstacle*> GetObstacleList() { return obstacles_; }

    void a();

    void SetAllObstacleActive(bool b);

};

