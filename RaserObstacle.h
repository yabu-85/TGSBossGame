#pragma once
#include "Obstacle.h"

class RaserObstacle :
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

public:
    RaserObstacle(GameObject* parent);
    ~RaserObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

