#pragma once
#include "Engine/GameObject.h"
#include "Obstacle.h"
#include <vector>
#include "Engine/CsvReader.h"
#include "Engine/Text.h"

class Player;

// è·äQï®î‘çÜ
enum ObstacleType
{
    OBSTACLE_NORMAL = 0,
    OBSTACLE_MISSILE,
    OBSTACLE_WALL,
    OBSTACLE_UFO
};

class ObstacleManager : public GameObject
{
    std::vector<Obstacle*> obstacles_;
    CsvReader csv_;
    int width_;
    int height_;
    int loadPosZ_;
    int loadPosZSub_;
    Player* pPlayer_;
    Text* pText_;


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

    void LoadCsv();
};

