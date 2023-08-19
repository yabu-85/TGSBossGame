#pragma once
#include "Engine/GameObject.h"

//Obstacleの基底クラス
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

    //障害物の位置をセットする
    void SetPosition(XMFLOAT3 _position);
};

