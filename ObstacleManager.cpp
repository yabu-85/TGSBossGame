#include "ObstacleManager.h"
#include "WallObstacle.h"
#include "UfoObstacle.h"
#include "RobotObstacle.h"
#include "Engine/CsvReader.h"
#include "Player.h"
#include "Missile.h"
#include "RaserObstacle.h"
#include "Engine/Input.h"
#include "BossObstacle.h"

namespace {
    //Uf�������[�h�͈͂����߂�o
    const float ufoLoadRange = -20.0f;

}

ObstacleManager::ObstacleManager(GameObject* parent)
    :GameObject(parent, "ObstacleManager"), width_(0), height_(0), activationZone_(0), activationZoneSub_(0),
    pPlayer_(nullptr), isActive_(true), csvInit_(false)
{
}

ObstacleManager::~ObstacleManager()
{
}

void ObstacleManager::Initialize()
{
    activationZone_ = 70;
    activationZoneSub_ = activationZone_;

}

void ObstacleManager::Update()
{
    if (!isActive_) return;

    //�v���C���[���܂��̑҂�
    if (pPlayer_ == nullptr) {
        if (FindObject("Player")) {
            pPlayer_ = (Player*)FindObject("Player");
        }

        return;
    }

    //�f�o�b�O�p�u���C�N�|�C���g
    if (Input::IsKeyDown(DIK_3)) {
        std::vector <Obstacle*> e = obstacles_;
        std::vector <Obstacle*> bi = inactiveObstacles_;
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

void ObstacleManager::InitCsv(std::string mp)
{
    if (csvInit_ == false) {
        // CSV�t�@�C���ǂݍ���
        csv_.Load(mp + ".csv");

        csvInit_ = true;
    }

    //�X�e�[�W�̕��ƍ�����ݒ�
    width_ = (int)csv_.GetWidth();
    height_ = (int)csv_.GetHeight();

    //���[�U�[�̕`��̖���UFO����Initialize���Ă���
    //CSV�f�[�^���e�[�u���Ɋi�[
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

    //UFO�ȊO�̂�Initialize����
    //CSV�f�[�^���e�[�u���Ɋi�[
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

void ObstacleManager::removeObstacle(Obstacle* _obstacle)
{
    if (_obstacle != nullptr) {
        for (auto it = obstacles_.begin(); it != obstacles_.end();) {
            if (*it == _obstacle) {
                it = obstacles_.erase(it);
            }
            else {
                it++;
            }
        }
    }
}

void ObstacleManager::removeInActiveObstacle(Obstacle* _obstacle)
{
    if (_obstacle != nullptr) {
        for (auto it = inactiveObstacles_.begin(); it != inactiveObstacles_.end();) {
            if (*it == _obstacle) {
                it = inactiveObstacles_.erase(it);
            }
            else {
                it++;
            }
        }
    }
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
    case ObstacleType::OBSTACLE_BOSS:
        pObstacle = (BossObstacle*)Instantiate<BossObstacle>(this);
        break;
    }

    //nullptr����Ȃ����
    if (pObstacle)
    {
        pObstacle->SetPosition(_position);
        pObstacle->SetCsvPos(_position);
        obstacles_.push_back(pObstacle);
        inactiveObstacles_.push_back(pObstacle);
    }
}

void ObstacleManager::LoadCsv()
{
    for (Obstacle* e : inactiveObstacles_) {
        if (!e) {
            continue;
        }

        if (e->GetObjectName() == "UfoObstacle"){ 
            if (e->GetCsvPos().z <= activationZoneSub_ + ufoLoadRange) {
                e->SetDraw(true);
                e->SetActive(true);
                removeInActiveObstacle(e);
            }
        }

        else {  //���̏�Q���̏���
            if (e->GetCsvPos().z <= activationZoneSub_) {
                e->SetDraw(true);
                e->SetActive(true);
                removeInActiveObstacle(e);
            }
        }
    }
}

//���˂Ŏg������O�ς��Ƃ��C����������
//�����ǂ񂾂�Maneger��obstacles_��Robot�̎q��Missile�B�Ƀv���C���[�̏Փˋ��������v�Z���Ă���
void ObstacleManager::Reflect()
{
    for (Obstacle* e : obstacles_) {
        if (!e) {
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

    for (Obstacle* e : obstacles_) {
        if (!e) {
            continue;
        }

        if (e->GetObjectName() == "UfoObstacle") {
            if (e->GetCsvPos().z <= activationZoneSub_ + ufoLoadRange) {
                e->SetActive(b);
            }
        }
        else {  //���̏�Q���̏���
            if (e->GetCsvPos().z <= activationZoneSub_) {
                e->SetActive(b);
            }
        }

        if (e->GetObjectName() == "RobotObstacle") {
            RobotObstacle* pRobot = dynamic_cast<RobotObstacle*>(e);
            std::vector<Missile*> mis = pRobot->GetMissiles();
            for (Missile* missile : mis) {
                missile->SetActive(b);
            }
        }

        if (e->GetObjectName() == "RaserObstacle") {
            RaserObstacle* pRaser = dynamic_cast<RaserObstacle*>(e);
            pRaser->SetBeamActive(b);
        }
    }
}

void ObstacleManager::KillObstacles()
{   
    for (Obstacle* obj : obstacles_) {
        obj->KillMeSub();
    }

    for (Obstacle* obj : obstacles_) {
        removeObstacle(obj);
    }
    obstacles_.clear();

    for (Obstacle* obj : inactiveObstacles_) {
        removeInActiveObstacle(obj);
    }
    inactiveObstacles_.clear();

}
