#include "ObstacleManager.h"
#include "MissileObstacle.h"
#include "WallObstacle.h"
#include "UfoObstacle.h"
#include "Engine/CsvReader.h"

ObstacleManager::ObstacleManager(GameObject* parent)
{
}

ObstacleManager::~ObstacleManager()
{
}

void ObstacleManager::Initialize()
{
#if 0
    // CSVファイル読み込み
    CsvReader csv;
    csv.Load("Obstacle.csv");

    //ステージの幅と高さを設定
    int w = (int)csv.GetWidth();
    int h = (int)csv.GetHeight();

    //CSVデータをテーブルに格納
    for (int x = 0; x < w; x++)
    {
        for (int z = 0; z < h; z++)
        {
            if (csv.GetValue(x,z) != 0) 
            {
                XMFLOAT3 position(static_cast<float>(x), 1, static_cast<float>(z));
                int intValue = csv.GetValue(x, z);
                ObstacleType a = static_cast<ObstacleType>(intValue);
                createAndAddObstacle(position, a);
            }
        }
    }
#endif

    createAndAddObstacle(XMFLOAT3(0, 0, 0), OBSTACLE_NORMAL);
	createAndAddObstacle(XMFLOAT3(4, 1, 30), OBSTACLE_MISSILE);
	createAndAddObstacle(XMFLOAT3(3, 1, 30), OBSTACLE_WALL);
	createAndAddObstacle(XMFLOAT3(7, 1, 30), OBSTACLE_UFO);
}

void ObstacleManager::Update()
{
}

void ObstacleManager::Draw()
{
}

void ObstacleManager::Release()
{
}

void ObstacleManager::addObstacle(Obstacle* _obstacle)
{
	obstacles_.push_back(_obstacle);
}

void ObstacleManager::createAndAddObstacle(XMFLOAT3 _position, ObstacleType _type)
{
    Obstacle* pObstacle = nullptr;

    switch (_type) 
    {
    case ObstacleType::OBSTACLE_NORMAL:
        pObstacle = Instantiate<Obstacle>(this);
        break;
    case ObstacleType::OBSTACLE_MISSILE:
        pObstacle = Instantiate<MissileObstacle>(this);
        break;
    case ObstacleType::OBSTACLE_WALL:
        pObstacle = Instantiate<WallObstacle>(this);
        break;
    case ObstacleType::OBSTACLE_UFO:
        pObstacle = Instantiate<UfoObstacle>(this);
        break;
    }

    //nullptrじゃなければ
    if (pObstacle) 
    {
        pObstacle->SetPosition(_position);
        addObstacle(pObstacle);
    }

}
