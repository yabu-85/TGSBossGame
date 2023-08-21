#pragma once
#include "Engine/GameObject.h"

//Obstacle�̊��N���X
class Obstacle : public GameObject
{
protected:
    int hModel_;

public:
    Obstacle(GameObject* parent);
    ~Obstacle();
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void Release() override;

    //��Q���̈ʒu���Z�b�g����
    void SetPosition(XMFLOAT3 _position);
};

