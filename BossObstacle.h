#pragma once
#include "Obstacle.h"

class Player;

class BossObstacle : public Obstacle
{
    enum STATE {
        S_ENTER = 0, //登場

        S_MISSILE,

        S_DEAD,
    }
    state_;
    bool stateEnter_;
    void UpdateEnter();
    void UpdateMissile();
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
    bool IsInTargetPosition(float _leng);               //Targetの位置に到着したか
    bool IsInTargetPosition(XMFLOAT3 _tar, float _leng);//ターゲット位置手動
    void ShotBeam();                                    //ビーム発射
    void AirStrike();                                   //空爆実行
    void ShotMissile();                                 //ミサイル発射    

public:
    BossObstacle(GameObject* parent);
    ~BossObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

