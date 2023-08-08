#pragma once
#include "Engine/GameObject.h"
#include "Aim.h"
#include "Engine/Text.h"

class Stage;

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
    float decelerationTime_;        //�����̌�������(def=1.0->0.0
    float maxMoveSpeed_;            //�ő�ړ����x
    XMFLOAT3 fMove_;                //�ړ�����
    XMFLOAT3 previousPosition_;     //�O�̍��W
    XMFLOAT3 playerMovement_;       //�v���C���[�̈ړ���
    bool firstJump_;                //�W�����v���Ă��邩
    bool secondJump_;               //�W�����v���Ă��邩
    bool bulletJump_;               //�o���b�g�W�����v���Ă��邩
    bool isCrouching_;              //���Ⴊ��ł��邩
    bool anime_;                    //�A�j���[�V�������Ă�H
    bool isDecelerated_;            //���Ɍ������Ă��邩
    bool isDecelerating_;           //������Ԃ�
    bool active_;                   //������󂯕t���邩
    Stage* pStage_;
    Text* pText_;
    Aim* pAim_;

    //---------------private�����o�֐�---------------

    void CalcMove();                    //�ړ������v�Z�Enormalize
    void InstantRotate();               //�����ɉ�]
    void GradualRotate();               //���X�ɐU�����
    void Gravity();                     //�d�͂����܂�
    void Crouch();                      //���Ⴊ�݊֌W�ł�
    void Jump();                        //�W�����v������
    float NormalizeAngle(float angle);  //angle�̒l�𒲐�����֐��i�U��������������߂����Ō����Ă���邽�߂�

    bool IsMovementKeyPressed();        //�ړ��L�[�������Ă��邩
    bool IsPlayerMove();                //�ړ��L�[�������Ă��邩
    bool IsPlayerOnGround();            //�n�ʂɂ��Ă��邩

    //------------------State------------------
    enum STATE {
        S_IDLE,
        S_MOVE,
        S_DEAD,
    };
    STATE state_;

    void UpdateIdle();
    void UpdateMove();
    void UpdateDead();


public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetActive(bool b) { active_ = b; };

    //�[�[�[�[�[�[�Q�b�^�[�[�[�[�[�[�[�[
    XMVECTOR GetPlaVector();                            //�ړ������擾
    float GetCameraHeight() { return cameraHeight_; };  //�J�����̍���0.8f �` 1.0f
    float IsAiming();                                   //�Y�[���l�F�n��i�߂��I�j,�󒆁i�����炢�j,�`���ĂȂ��i1.0�j
    bool IsDecelerating() { return isDecelerating_; };

};