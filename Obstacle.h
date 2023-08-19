#pragma once
#include "Engine/GameObject.h"
//Obstacle�̊��N���X

class Obstacle :
    public GameObject
{
protected:
    int x;
    int y;
    int z;
    int hModel_;

public:
    Obstacle(GameObject* parent);
    ~Obstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetPosition(int _x, int _y, int _z);
};

