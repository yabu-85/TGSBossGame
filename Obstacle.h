#pragma once
#include "Engine/GameObject.h"

//Obstacle�̊��N���X
class Obstacle : public GameObject
{
protected:
    XMFLOAT3 position_;
    int hModel_;

public:
    Obstacle(GameObject* parent);
    ~Obstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //��Q���̈ʒu���Z�b�g����
    void SetPosition(XMFLOAT3 _position);
};

