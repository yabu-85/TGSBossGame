#pragma once
#include "Obstacle.h"

class Player;
class Missile;

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
    float count_;
    Player* pPlayer_;
    std::list<Missile*> missiles_;

    void Rotate();
    void ShotMissile();

public:
    RobotObstacle(GameObject* parent);
    ~RobotObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //引数のオブジェクトをmissiles_から外す
    void NotifyMissileDestroyed(Missile* destMissile);

    std::list<Missile*> GetMissiles() { return missiles_; };
};

