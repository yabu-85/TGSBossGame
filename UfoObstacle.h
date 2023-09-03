#pragma once
#include "Obstacle.h"

class UfoObstacle : public Obstacle
{
    enum STATE {
        S_ENTER = 0,    //�o��
        S_DETECT,       //�v���C���[�����m����
        S_PREARATION,   //������
        S_SHOT,         //�����őł�
        S_LEAVING,      //����
    }
    state_;
    bool stateEnter_;
    void UpdateEnter();
    void UpdateDetection();
    void UpdatePreparation();
    void UpdateShot();
    void UpdateLeaving();
    void ChangeState(STATE s);

    bool attack_;           //�U�����邩�ǂ���
    bool attackDon_;        //��񂾂��U����������悤��
    int hModelSub_[3];      //���C�U�[�ƌ��m�p
    int time_;              //�`���[�W���ԂƂ��Ɏg��
    float moveSpeed_;       //�ړ��X�s�[�h
    float moveDist_;        //�ړ����鋗���i����l�݂����Ȃ��́H
    float leavYmoveSpeed_;  //����Ƃ��̏㏸�l
    XMVECTOR leavVec_;      //����Ƃ��̕���
    XMFLOAT3 targetPos_;    //�r�[������ꏊ

public:
    UfoObstacle(GameObject* parent);
    ~UfoObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

