#pragma once
#include "Engine/GameObject.h"
#include "Aim.h"

#define SAFE_DELETE(p) if(p != nullptr){ p = nullptr; delete p;}

class Player : public GameObject
{
    int hModel_;                    //���f���ԍ�
    float moveSpeed_;               //�ړ�
    float targetRotation_;          //�ڕW�̉�]�p�x
    float rotationSpeed_;           //��]���x
    float graY_;                    //�W�����v���̏d�͌v�Z�p
    float gravity_;                 //�d�͂̒l
    float initVy_;                  //�����W�����v��
    XMFLOAT3 fMove_;                //�ړ�����
    XMFLOAT3 previousPosition_;     //�O�̍��W
    bool firstJump_;                //�W�����v���Ă��邩
    bool secondJump_;               //�W�����v���Ă��邩
    bool anime_;
    Aim* pAim_;

    void CalcMoveVec();                 //�ړ������v�Z�Enormalize
    float NormalizeAngle(float angle);  //angle�̒l�𒲐�����֐��i�U��������������߂����Ō����Ă���邽�߂�

    enum STATE {
        S_IDLE,
        S_MOVE,
        S_JUMP,
        S_DEAD,
    };
    STATE state_;

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void UpdateIdle();
    void UpdateMove();
    void UpdateJump();
    void UpdateDead();

    //�Q�b�^�[
    XMFLOAT3 GetPlaPos() { return transform_.position_; };
    XMFLOAT3 GetPlaRotate() { return transform_.rotate_; };
    bool IsPlayerOnGround();
    bool IsPlayerMove();
    XMVECTOR GetPlaVector();

};