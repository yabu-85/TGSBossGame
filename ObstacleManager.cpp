#include "ObstacleManager.h"
#include "WallObstacle.h"
#include "UfoObstacle.h"
#include "RobotObstacle.h"
#include "Engine/CsvReader.h"
#include "Player.h"
#include "Missile.h"
#include "RaserObstacle.h"
#include "Engine/Input.h"

#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}

//Ufoだけロード範囲を狭める
static float ufoLoadRange = -20.0f;

ObstacleManager::ObstacleManager(GameObject* parent)
    :GameObject(parent, "ObstacleManager"), width_(0), height_(0), activationZone_(0), activationZoneSub_(0),
    pPlayer_(nullptr), isActive_(true)
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

    activationZone_ = 70;
    activationZoneSub_ = activationZone_;

}

void ObstacleManager::Update()
{
    if (!isActive_) return;

    //プレイヤー生まれるの待つ
    if (pPlayer_ == nullptr) {
        if (FindObject("Player")) {
            pPlayer_ = (Player*)FindObject("Player");
            InitCsv();
        }

        return;
    }

    if (Input::IsKeyDown(DIK_3)) {
        int a = 0;
        std::vector <Obstacle*> b = obstacles_;
    }
    
    int plaPosZ = (int)pPlayer_->GetPosition().z;
    if (activationZoneSub_ < activationZone_ + plaPosZ) {
        activationZoneSub_ = activationZone_ + plaPosZ;
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

void ObstacleManager::removeObstacle(Obstacle* _obstacle)
{
    if (_obstacle != nullptr) {
        for (auto it = obstacles_.begin(); it != obstacles_.end();) {
            if (*it == _obstacle) {
                *it = nullptr;
                delete* it; // メモリを解放
                it = obstacles_.erase(it); // 要素を削除し、次の要素を指すイテレータを取得
            }
            else {
                it++;
            }
        }
    }
}

void ObstacleManager::removeObstacle(RobotObstacle* _obstacle)
{
    if (_obstacle != nullptr) {
        for (auto it = obstacles_.begin(); it != obstacles_.end();) {
            if (*it == _obstacle) {
                *it = nullptr;
                delete* it; // メモリを解放
                it = obstacles_.erase(it); // 要素を削除し、次の要素を指すイテレータを取得
            }
            else {
                it++;
            }
        }
    }
}

void ObstacleManager::removeObstacle(UfoObstacle* _obstacle)
{
    removeObstacle((RobotObstacle*)_obstacle);

}

void ObstacleManager::createAndAddObstacle(XMFLOAT3 _position, ObstacleType _type)
{
    Obstacle* pObstacle = nullptr;

    switch (_type)
    {
    case ObstacleType::OBSTACLE_WALL:
        pObstacle = (WallObstacle*)Instantiate<WallObstacle>(this);
        break;
    case ObstacleType::OBSTACLE_UFO:
        pObstacle = (UfoObstacle*)Instantiate<UfoObstacle>(this);
        break;
    case ObstacleType::OBSTACLE_ROBOT:
        pObstacle = (RobotObstacle*)Instantiate<RobotObstacle>(this);
        break;
    case ObstacleType::OBSTACLE_RASER:
        pObstacle = (RaserObstacle*)Instantiate<RaserObstacle>(this);
        break;
    }

    //nullptrじゃなければ
    if (pObstacle)
    {
        pObstacle->SetPosition(_position);
        pObstacle->SetCsvPos(_position);
        addObstacle(pObstacle);
    }

}

void ObstacleManager::LoadCsv()
{
    //CSVデータをテーブルに格納
    for (Obstacle* e : obstacles_) {
        if (!e) {
            removeObstacle(e);
            continue;
        }

        if (e->GetObjectName() == "UfoObstacle"){ 
            if (e->GetCsvPos().z <= activationZoneSub_ + ufoLoadRange) {
                e->SetDraw(true);
                e->SetActive(true);
            }
        }

        else {  //他の障害物の処理
            if (e->GetCsvPos().z <= activationZoneSub_) {
                e->SetDraw(true);
                e->SetActive(true);
            }
        }
    }
}

//反射で使うやつ名前変えとく気が向いたら
//これを読んだらManegerのobstacles_のRobotの子供Missile達にプレイヤーの衝突距離かを計算している
void ObstacleManager::a()
{
    for (Obstacle* e : obstacles_) {
        if (!e) {
            removeObstacle(e);
            continue;
        }
        
        if (e->GetObjectName() == "RobotObstacle") {
            RobotObstacle* pRobot = dynamic_cast<RobotObstacle*>(e);

            std::vector<Missile*> mis = pRobot->GetMissiles();
            for (Missile* missile : mis) {
                missile->Reflect();
            }
        }
    }
}

void ObstacleManager::SetAllObstacleActive(bool b)
{
    isActive_ = b;

    if (!b) for (auto e : obstacles_) {
        e->SetActive(b);

    }
    else {
        LoadCsv();

    }

    for (GameObject* e : obstacles_) {
        if (e->GetObjectName() == "RobotObstacle") {
            RobotObstacle* pRobot = dynamic_cast<RobotObstacle*>(e);
            if (!pRobot) continue;

            std::vector<Missile*> mis = pRobot->GetMissiles();
            for (Missile* missile : mis) {
                missile->SetActive(b);

            }
        }
    }

}

void ObstacleManager::InitCsv()
{
    //レーザーの描画の問題でUFOを先にInitializeしておく
    //CSVデータをテーブルに格納
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (csv_.GetValue(x, y) != 0)
            {
                XMFLOAT3 position((float)x + 0.5f, 50, (float)(height_ - y) - 0.5f);
                int intValue = csv_.GetValue(x, y);
                if (intValue != OBSTACLE_UFO) {
                    ObstacleType a = static_cast<ObstacleType>(intValue);
                    createAndAddObstacle(position, a);
                    
                }
            }
        }
    }

    //UFO以外のをInitializeする
    //CSVデータをテーブルに格納
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (csv_.GetValue(x, y) != 0)
            {
                XMFLOAT3 position((float)x + 0.5f, 50, (float)(height_ - y) - 0.5f);
                int intValue = csv_.GetValue(x, y);
                if (intValue == OBSTACLE_UFO) {
                    ObstacleType a = static_cast<ObstacleType>(intValue);
                    createAndAddObstacle(position, a);

                }
            }
        }
    }
}
