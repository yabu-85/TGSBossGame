#pragma once
#include "Engine/GameObject.h"

class Player;

class Aim :
    public GameObject
{
    int hPict_;                     //�摜�ԍ�
    float mouseSensitivity;         //�}�E�X���x
    float perspectiveDistance_;     //�ǂ̂��炢��납��f����
    float heightDistance_;          //�œ_�̍���
    bool aimDraw_;                  //�G�C����\�����邩
    bool aimMove_;                  //�G�C���𓮂������ǂ���
    XMFLOAT3 cameraTarget_;         //�J�����̏œ_�ڕW
    XMFLOAT3 cameraPos_;            //�J�����̏ꏊ�ڕW
    XMFLOAT3 aimDirectionXY_;       //���݂̎��_�Ɋ�Â����i�s�����x�N�g��
    XMFLOAT3 aimDirectionY_;        //���݂̎��_�Ɋ�Â����i�s�����x�N�g��
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
    XMFLOAT3 GetAimDirectionY() { return aimDirectionY_; }
    XMFLOAT3 GetAimDirectionXY() { return aimDirectionXY_; }
    void SetAimDraw(bool b) { aimDraw_ = b; };
    void SetAimMove(bool b) { aimMove_ = b; };

};

