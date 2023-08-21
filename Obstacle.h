#pragma once
#include "Engine/GameObject.h"

//Obstacleの基底クラス
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

    //障害物の位置をセットする
    void SetPosition(XMFLOAT3 _position);
};

