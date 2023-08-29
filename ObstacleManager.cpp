#include "ObstacleManager.h"
#include "WallObstacle.h"
#include "UfoObstacle.h"
#include "RobotObstacle.h"
#include "Engine/CsvReader.h"
#include "Player.h"
#include "Missile.h"

//Ufo�������[�h�͈͂����߂�
static float ufoLoadRange = -40.0f;

ObstacleManager::ObstacleManager(GameObject* parent)
    :GameObject(parent, "ObstacleManager"), width_(0), height_(0), activationZone_(0), activationZoneSub_(0),
    pPlayer_(nullptr)
{
}

ObstacleManager::~ObstacleManager()
{
}

void ObstacleManager::Initialize()
{
    // CSV�t�@�C���ǂݍ���
    csv_.Load("Obstacle.csv");

    //�X�e�[�W�̕��ƍ�����ݒ�
    width_ = (int)csv_.GetWidth();
    height_ = (int)csv_.GetHeight();

    activationZone_ = 70;
    activationZoneSub_ = activationZone_;

}

void ObstacleManager::Update()
{
    //�v���C���[���܂��̑҂�
    if (pPlayer_ == nullptr) {
        if (FindObject("Player")) {
            pPlayer_ = (Player*)FindObject("Player");
            InitCsv();
        }
        return;
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

    //nullptr����Ȃ����
    if (pObstacle)
    {
        pObstacle->SetPosition(_position);
        addObstacle(pObstacle);
    }

}

void ObstacleManager::LoadCsv()
{
    //CSV�f�[�^���e�[�u���Ɋi�[
    for (Obstacle* obj : obstacles_) {
        Obstacle* pObstacle = (Obstacle*)obj;

        // UFO�̏ꍇ�A���[�h�͈͂𐧌�����
        if (pObstacle->GetObjectName() == "UfoObstacle") {
            if (pObstacle->GetPosition().z <= activationZoneSub_ + ufoLoadRange) {
                pObstacle->SetActive(true);
            }
        }
        // ���̏�Q���̏���
        else {
            if (pObstacle->GetPosition().z <= activationZoneSub_) {
                pObstacle->SetActive(true);
            }
        }
    }
}

//���˂Ŏg������O�ς��Ƃ��C����������
void ObstacleManager::a()
{
    for (GameObject* e : obstacles_) {
        if (e->GetObjectName() == "RobotObstacle") {
            RobotObstacle* pRobot = dynamic_cast<RobotObstacle*>(e);
            if (!pRobot) continue;

            std::list<Missile*> mis = pRobot->GetMissiles();
            for (Missile* missile : mis) {
                missile->Reflect(XMFLOAT3 (1,1,1) );
            }
        }
    }


}


void ObstacleManager::InitCsv()
{

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
