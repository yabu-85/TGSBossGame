#include "ObstacleManager.h"
#include "WallObstacle.h"
#include "UfoObstacle.h"
#include "RobotObstacle.h"
#include "Engine/CsvReader.h"
#include "Player.h"

ObstacleManager::ObstacleManager(GameObject* parent)
    :GameObject(parent, "ObstacleManager"), width_(0), height_(0), activationZone_(0), activationZoneSub_(0),
    pPlayer_(nullptr), pText_(nullptr)
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

    activationZone_ = 70;
    activationZoneSub_ = activationZone_;

    //CSVデータをテーブルに格納
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (csv_.GetValue(x, y) != 0)
            {
                XMFLOAT3 position((float)x + 0.5f, 50,(float)(height_ - y) - 0.5f );
                int intValue = csv_.GetValue(x, y);
                ObstacleType a = static_cast<ObstacleType>(intValue);
                createAndAddObstacle(position, a);
            }
        }
    }

    pText_ = new Text;
    pText_->Initialize();

}

void ObstacleManager::Update()
{
    int plaPosZ = (int)pPlayer_->GetPosition().z;
    if (activationZoneSub_ < activationZone_ + plaPosZ) {
        activationZoneSub_ = activationZone_ + plaPosZ;
        LoadCsv();

    }
    
}

void ObstacleManager::Draw()
{
    pText_->Draw(30, 240, activationZoneSub_);

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
    case ObstacleType::OBSTACLE_WALL:
        pObstacle = Instantiate<WallObstacle>(this);
        break;
    case ObstacleType::OBSTACLE_UFO:
        pObstacle = Instantiate<UfoObstacle>(this);
        break;
    case ObstacleType::OBSTACLE_ROBOT:
        pObstacle = Instantiate<RobotObstacle>(this);
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
    for (int x = 0; x < width_; x++) {

        for (Obstacle* obj : obstacles_) {
            Obstacle* pObstacle = (Obstacle*)obj;
            if (pObstacle->GetPosition().z <= activationZoneSub_) {
                pObstacle->SetActive(true);
            }
        }
        
    }
}
