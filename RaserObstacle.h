#pragma once
#include "Obstacle.h"

class Beam;
class Player;

class RaserObstacle :
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
    int count_;
    bool aliveBeam_;    //これBeamのポインタがnullが出来なかった時用の
    Player* pPlayer_;
    Beam* pBeam_;

    //回転する
    void Rotate();
    //ミサイル撃つ&リストに登録
    void ShotBeam();

public:
    RaserObstacle(GameObject* parent);
    ~RaserObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void KillMeSub() override;
    void SetBeamActive(bool b);

};

