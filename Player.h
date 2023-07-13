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
    bool jump_;                     //�W�����v���Ă��邩
    XMFLOAT3 fMove_;                //�ړ�����
    XMFLOAT3 previousPosition_;     //�O�̍��W
    Aim* pAim_;

    void CalcMoveVec();

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //�Q�b�^�[
    XMFLOAT3 GetPlaPos() { return transform_.position_; };
    XMFLOAT3 GetPlaRotate() { return transform_.rotate_; };
    bool IsPlayerOnGround();
    bool IsPlayerMove();
    XMVECTOR GetPlaVector();

};