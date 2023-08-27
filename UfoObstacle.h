#pragma once
#include "Obstacle.h"

class UfoObstacle : public Obstacle
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

    int hModelLa_;
    int time_;              //�`���[�W���ԂƂ��Ɏg��
    float moveSpeed_;       //�ړ��X�s�[�h
    XMFLOAT3 targetPos_;    //�r�[������ꏊ

public:
    UfoObstacle(GameObject* parent);
    ~UfoObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

