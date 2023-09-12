#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"
#include "PlayerSpeedController.h"

class Aim;
class Stage;

class Player : public GameObject
{
    Text* pText;

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

    int hModel_;                    //���f���ԍ�
    int hPict_;
    int hp_;                        //����HP
    int maxHp_;                     //�ő�HP
    float moveSpeed_;               //�ړ�
    float targetRotation_;          //�ڕW�̉�]�p�x
    float graY_;                    //�W�����v���̏d�͌v�Z�p
    float cameraHeight_;            //�J�����̍��� (���Ⴊ�݂Ƃ��ɂ����g��Ȃ��H)
    float maxMoveSpeed_;            //�ő�ړ����x
    float prevYHeight_;             //���܂������p��
    bool firstJump_;                //�W�����v���Ă��邩
    bool secondJump_;               //�W�����v���Ă��邩2
    bool bulletJump_;               //�o���b�g�W�����v���Ă��邩
    bool isCrouching_;              //���Ⴊ��ł��邩
    bool anime_;                    //�A�j���[�V�������Ă�H
    bool isActive_;                 //������󂯕t���邩
    XMFLOAT3 fMove_;                //�ړ�����
    XMFLOAT3 playerMovement_;       //�v���C���[�̈ړ���

    Stage* pStage_;
    Aim* pAim_;
    PlayerSpeedController* pSpeedCtrl_;

    //����value�F���K�����鎞�ɂ�����l
    void GetMoveDirection(float value = 1.0f);

    void CalcMove();                                    //�ړ������v�Z�Enormalize
    void Rotate(float x, float z, float _rotateSpeed);  //��]����
    void Gravity();                                     //�d�͂����܂�
    void PlayerFallReset();                             //�����������̃t���O�Ȃǂ̃��Z�b�g
    void Jump();                                        //�W�����v�����
    float NormalizeAngle(float angle);                  //angle�̒l�𒲐�����֐��i�U��������������߂����Ō����Ă���邽�߂�
    bool IsMovementKeyPressed();                        //�ړ��L�[�������Ă��邩
    bool IsPlayerOnGround();                            //�n�ʂɂ��Ă��邩
    void IsInWall();

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

    bool IsisCrouc() { return isCrouching_; };
    void SetPlayerMovement(XMFLOAT3 _move) { playerMovement_ = _move; };

    void ResetPlayer();

};