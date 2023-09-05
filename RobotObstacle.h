#pragma once
#include "Obstacle.h"
#include <vector>

class Player;
class Missile;

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
    float count_;
    Player* pPlayer_;

    std::vector<Missile*> missiles_;

    //��]����
    void Rotate();
    //�~�T�C������&���X�g�ɓo�^
    void ShotMissile();

public:
    RobotObstacle(GameObject* parent);
    ~RobotObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //�����̃I�u�W�F�N�g��missiles_����O��
    void NotifyMissileDestroyed(Missile* destMissile);

    //�~�T�C���̃��X�g�擾
    std::vector<Missile*> GetMissiles() { return missiles_; };

    //
    void KillMeSub();

};

