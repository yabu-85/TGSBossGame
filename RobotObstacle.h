#pragma once
#include "Obstacle.h"

class Player;

class RobotObstacle :
    public Obstacle
{
    enum STATE {
        S_ENTER = 0, //�o��
        S_CHARGING,  //�`���[�W��
        S_SHOT,      //�����őł�
        S_IDLE,      //�������Ȃ�
        S_LEAVING,   //����
    }
    state_;
    bool stateEnter_;
    void UpdateEnter();
    void UpdateCharging();
    void UpdateShot();
    void UpdateIdle();
    void UpdateLeaving();
    void ChangeState(STATE s);

    int hModelHead_;
    int nearestLocation_;
    float count_;
    Player* pPlayer_;

    void Rotate();
    void ShotMissile();

public:
    RobotObstacle(GameObject* parent);
    ~RobotObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // �Փ˔����t�^
    void OnCollision(GameObject* pTarget) override;

    void SetLearestLocation();

};

