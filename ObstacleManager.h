#pragma once
#include "Engine/GameObject.h"
#include "Obstacle.h"
#include <vector>
#include "Engine/CsvReader.h"

class Player;

// áŠQ•¨”Ô†
enum ObstacleType
{
    OBSTACLE_NORMAL = 0,
    OBSTACLE_WALL,
    OBSTACLE_UFO,
    OBSTACLE_ROBOT,
    OBSTACLE_LASER,
    OBSTACLE_MAX,
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

    // áŠQ•¨‚ğ’Ç‰Á‚·‚é
    void addObstacle(Obstacle* _obstacle);

    // áŠQ•¨‚ğ¶¬‚µ‚Ä’Ç‰Á‚·‚é
    void createAndAddObstacle(XMFLOAT3 _position, ObstacleType _type);

    //”ÍˆÍ“à‚ÌáŠQ•¨‚ğActive‚É
    void LoadCsv();

    //Obstacles‚Ìlist‚ğæ“¾
    std::vector<Obstacle*> GetObstacleList() { return obstacles_; }

    void a();
};

