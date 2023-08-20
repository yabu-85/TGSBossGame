#include "ObstacleManager.h"
#include "MissileObstacle.h"
#include "WallObstacle.h"
#include "UfoObstacle.h"
#include "Engine/CsvReader.h"
#include "Player.h"

ObstacleManager::ObstacleManager(GameObject* parent)
    :GameObject(parent, "ObstacleManager"), width_(0), height_(0), loadPosZ_(0), pPlayer_(nullptr)
{
}

ObstacleManager::~ObstacleManager()
{
}

void ObstacleManager::Initialize()
{
    // CSVファイル読み込み
    csv_.Load("Obstacle.csv");

    //ステージの幅と高さを設定
    width_ = (int)csv_.GetWidth();
    height_ = (int)csv_.GetHeight();

    pPlayer_ = (Player*)FindObject("Player");

    loadPosZ_ = 10;
    
}

void ObstacleManager::Update()
{
    int plaPosZ = pPlayer_->GetPosition().z;
    if (loadPosZ_ < plaPosZ) {
        loadPosZ_ = plaPosZ;

        LoadCsv();
    }


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

void ObstacleManager::LoadCsv()
{
    //CSVデータをテーブルに格納
    for (int x = 0; x < width_; x++)
    {
        if (csv_.GetValue(x, loadPosZ_) != 0)
        {
            XMFLOAT3 position(static_cast<float>(x), 1, static_cast<float>(loadPosZ_+loadPosZ_));
            int intValue = csv_.GetValue(x, loadPosZ_);
            ObstacleType a = static_cast<ObstacleType>(intValue);
            createAndAddObstacle(position, a);
        }
    }
}
