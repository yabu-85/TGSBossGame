#pragma once
#include "Obstacle.h"

class UfoObstacle : public Obstacle
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

    int hModelLa_;
    int time_;              //チャージ時間とかに使う
    float moveSpeed_;       //移動スピード
    XMFLOAT3 targetPos_;    //ビームを放つ場所

public:
    UfoObstacle(GameObject* parent);
    ~UfoObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

