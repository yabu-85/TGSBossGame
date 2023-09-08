#pragma once
#include "Obstacle.h"

class Player;

class BossObstacle : public Obstacle
{
    enum STATE {
        S_ENTER = 0, //登場

        S_DEAD,
    }
    state_;
    bool stateEnter_;
    void UpdateEnter();
    void UpdateDead();
    void ChangeState(STATE s);

    float targetRotation_;       //向く方向
    float moveSpeed_;            //移動速度
    XMFLOAT3 targetPosition_;    //移動する目標地点
    XMVECTOR MoveDirection_;     //移動方向

    Player* pPlayer_;

    void Rotate(float x, float z, float _rotateSpeed);  //回転します
    float NormalizeAngle(float angle);                  //angleの値を調整する関数
    void Move();                                        //Targetへの移動
    bool IsInTargetPosition();                          //Targetの位置に到着したか

public:
    BossObstacle(GameObject* parent);
    ~BossObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;


};

