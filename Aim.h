#pragma once
#include "Engine/GameObject.h"

class Player;

class Aim :
    public GameObject
{
    float mouseSensitivity;         //�}�E�X���x
    float perspectiveDistance_;     //�ǂ̂��炢��납��f����
    float heightDistance_;          //�œ_�̍���
    float cameraSpeed_;             //�J�����̑��x
    float maxLeng_;                 //�ő�̗���鋗��

    XMFLOAT3 cameraTarget_;         //�J�����̏œ_�ڕW
    XMFLOAT3 cameraPos_;            //�J�����̏ꏊ�ڕW
    XMFLOAT3 currentCameraPos_;     //���̃J�����̏ꏊ
    XMFLOAT3 currentCameraTar_;     //���̃J�����̏œ_
    XMFLOAT3 aimDirection_;         //���݂̎��_�Ɋ�Â����i�s�����x�N�g��
    XMFLOAT3 plaPos_;               //�v���C���[�ʒu

    Player* pPlayer_;

public:
    Aim(GameObject* parent);
    ~Aim();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //�i�s�����x�N�g����Aim�����擾
    XMFLOAT3 GetAimDirection() { return aimDirection_; }
};

