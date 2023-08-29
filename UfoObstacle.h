#pragma once
#include "Obstacle.h"




class UfoObstacle : public Obstacle
{
    enum STATE {
        S_ENTER = 0,    //登場
        S_DETECT,       //プレイヤーを検知する
        S_PREARATION,   //撃つ準備
        S_SHOT,         //ここで打つ
        S_LEAVING,      //去る
    }
    state_;
    bool stateEnter_;
    void UpdateEnter();
    void UpdateDetection();
    void UpdatePreparation();
    void UpdateShot();
    void UpdateLeaving();
    void ChangeState(STATE s);

    bool attack_;           //攻撃するかどうか
    int hModelSub_[3];      //レイザーと検知用
    int time_;              //チャージ時間とかに使う
    float moveSpeed_;       //移動スピード
    float moveDist_;        //移動する距離（上限値みたいなもの？
    float leavYmoveSpeed_;  //去るときの上昇値
    XMVECTOR leavVec_;      //去るときの方向
    XMFLOAT3 targetPos_;    //ビームを放つ場所

    struct Cone {
        double h = 1.0;
        double r = 1.0;
        double min_h = 0.0;
        double max_h = 1.0;
    };

    bool Intersect_sphere_cone(XMVECTOR sphereCenter, float sphereRadius, XMVECTOR coneBaseCenter, XMVECTOR coneVertex);

public:
    UfoObstacle(GameObject* parent);
    ~UfoObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

