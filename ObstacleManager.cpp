#include "ObstacleManager.h"
#include "TestObstacle.h"

ObstacleManager::ObstacleManager(GameObject* parent)
{
}

ObstacleManager::~ObstacleManager()
{
}

void ObstacleManager::Initialize()
{
	createAndAddObstacle(2, 1, 20, OBSTACLE_NORMAL);
	createAndAddObstacle(4, 1, 30, OBSTACLE_SPIKE);
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

void ObstacleManager::createAndAddObstacle(int _x, int _y, int _z, ObstacleType _type)
{
    Obstacle* pObstacle = nullptr;

    switch (_type) {
    case ObstacleType::OBSTACLE_NORMAL:
        pObstacle = Instantiate<Obstacle>(this);
        break;
    case ObstacleType::OBSTACLE_SPIKE:
        pObstacle = Instantiate<TestObstacle>(this);
        break;
    }

    //nullptr‚¶‚á‚È‚¯‚ê‚Î
    if (pObstacle) {
        pObstacle->SetPosition(_x, _y, _z);
        addObstacle(pObstacle);
    }

}
