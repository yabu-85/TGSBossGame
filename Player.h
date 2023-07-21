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
    float cameraHeight_;            //�J�����̍��� (���Ⴊ�݂Ƃ��ɂ����g��Ȃ��H)
    float movementRatio_;           //�v���C���[�̈ړ����x�̔䗦0�`1
    XMFLOAT3 fMove_;                //�ړ�����
    XMFLOAT3 previousPosition_;     //�O�̍��W
    XMFLOAT3 playerMovement_;       //�v���C���[�̈ړ���
    bool firstJump_;                //�W�����v���Ă��邩
    bool secondJump_;               //�W�����v���Ă��邩
    bool isCrouching_;              //���Ⴊ��ł��邩
    bool anime_;                    
    Aim* pAim_;

    void CalcMoveVec();                 //�ړ������v�Z�Enormalize
    float NormalizeAngle(float angle);  //angle�̒l�𒲐�����֐��i�U��������������߂����Ō����Ă���邽�߂�
    void Rotate();                      //
    void Gravity();                     
    void Crouch();
    void Jump();
    void CalcMoveRatio(bool type);      //true=�v���X�P false=�}�C�i�X�P

    enum STATE {
        S_IDLE,
        S_MOVE,
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
    void UpdateDead();

    //�[�[�[�[�[�[�Q�b�^�[�[�[�[�[�[�[�[
    bool IsPlayerOnGround();                            //�n�ʂɂ��Ă��邩
    bool IsPlayerMove();                                //�ړ��L�[�������Ă��邩
    XMVECTOR GetPlaVector();                            //�ړ������擾
    float GetCameraHeight() { return cameraHeight_; };  //�J�����̍���0.8f �` 1.0f
    bool IsCrouching();

};