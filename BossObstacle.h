#pragma once
#include "Obstacle.h"

class Player;

class BossObstacle : public Obstacle
{
    enum STATE {
        S_ENTER = 0, //�o��

        S_MISSILE,

        S_DEAD,
    }
    state_;
    bool stateEnter_;
    void UpdateEnter();
    void UpdateMissile();
    void UpdateDead();
    void ChangeState(STATE s);

    float targetRotation_;       //��������
    float moveSpeed_;            //�ړ����x
    XMFLOAT3 targetPosition_;    //�ړ�����ڕW�n�_
    XMVECTOR MoveDirection_;     //�ړ�����

    Player* pPlayer_;

    void Rotate(float x, float z, float _rotateSpeed);  //��]���܂�
    float NormalizeAngle(float angle);                  //angle�̒l�𒲐�����֐�
    void Move();                                        //Target�ւ̈ړ�
    bool IsInTargetPosition(float _leng);               //Target�̈ʒu�ɓ���������
    bool IsInTargetPosition(XMFLOAT3 _tar, float _leng);//�^�[�Q�b�g�ʒu�蓮
    void ShotBeam();                                    //�r�[������
    void AirStrike();                                   //�󔚎��s
    void ShotMissile();                                 //�~�T�C������    

public:
    BossObstacle(GameObject* parent);
    ~BossObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

