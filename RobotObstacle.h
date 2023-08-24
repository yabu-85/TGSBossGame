#pragma once
#include "Obstacle.h"

class Player;

class RobotObstacle :
    public Obstacle
{
    enum STATE {
        S_ENTER = 0, //登場
        S_CHARGING,  //チャージ中
        S_SHOT,      //ここで打つ
        S_IDLE,      //何もしない
        S_LEAVING,   //去る
    }
    state_;
    bool stateEnter_;
    void UpdateEnter();
    void UpdateCharging();
    void UpdateShot();
    void UpdateIdle();
    void UpdateLeaving();
    void ChangeState(STATE s);

    int hModelHead_;
    int nearestLocation_;
    float count_;
    Player* pPlayer_;

    void Rotate();
    void ShotMissile();

public:
    RobotObstacle(GameObject* parent);
    ~RobotObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // 衝突判定を付与
    void OnCollision(GameObject* pTarget) override;

    void SetLearestLocation();

};

