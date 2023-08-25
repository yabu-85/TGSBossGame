#pragma once
#include "Engine/GameObject.h"
#include "Aim.h"
#include "Engine/Text.h"

class Stage;

class Player : public GameObject
{
    int hModel_;                    //���f���ԍ�
    int hp_;                        //����HP
    int maxHp_;                     //�ő�HP
    float moveSpeed_;               //�ړ�
    float targetRotation_;          //�ڕW�̉�]�p�x
    float rotationSpeed_;           //��]���x
    float graY_;                    //�W�����v���̏d�͌v�Z�p
    float gravity_;                 //�d�͂̒l
    float initVy_;                  //�����W�����v��
    float cameraHeight_;            //�J�����̍��� (���Ⴊ�݂Ƃ��ɂ����g��Ȃ��H)
    float maxMoveSpeed_;            //�ő�ړ����x
    XMFLOAT3 fMove_;                //�ړ�����
    XMFLOAT3 playerMovement_;       //�v���C���[�̈ړ���
    bool firstJump_;                //�W�����v���Ă��邩
    bool secondJump_;               //�W�����v���Ă��邩2
    bool bulletJump_;               //�o���b�g�W�����v���Ă��邩
    bool isCrouching_;              //���Ⴊ��ł��邩
    bool anime_;                    //�A�j���[�V�������Ă�H
    bool isActive_;                 //������󂯕t���邩

    Stage* pStage_;
    Text* pText_;
    Aim* pAim_;

    void CalcMove();                    //�ړ������v�Z�Enormalize
    void InstantRotate(float x, float z);               //�����ɉ�]
    void GradualRotate(float x, float z);               //���X�ɐU�����
    void Gravity();                     //�d�͂����܂�
    void Jump();                        //�W�����v�����
    float NormalizeAngle(float angle);  //angle�̒l�𒲐�����֐��i�U��������������߂����Ō����Ă���邽�߂�

    bool IsMovementKeyPressed();        //�ړ��L�[�������Ă��邩
    bool IsPlayerOnGround();            //�n�ʂɂ��Ă��邩

    //------------------State------------------
    enum STATE {
        S_IDLE,
        S_MOVE,
        S_CROUCH,
        S_DEAD,
    };
    STATE state_;
    bool stateEnter_; //�������������ĂԂ̂Ɏg��

    void ChangeState(STATE s);
    void UpdateIdle();
    void UpdateMove();
    void UpdateCrouch();
    void UpdateDead();

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //Active��x�������ăZ�b�g/1000=1�b
    void SetActiveWithDelay(bool isActive, int time);    
    void SetActive(bool b) { isActive_ = b; };  //Active���Z�b�g
    void DecreaseHp(int i);       //Hp�����炷�֐�

    float GetCameraHeight() { return cameraHeight_; };  //�J�����̍���0.8f �` 1.0f
    int GetModelHandle() { return hModel_; }
    int GetHp() { return hp_; };
    int GetMaxHp() { return maxHp_; };
};