#pragma once
#include "Obstacle.h"

class RaserObstacle :
    public Obstacle
{
    enum STATE {
        S_ENTER = 0, //ìoèÍ
        S_CHARGING,  //É`ÉÉÅ[ÉWíÜ
        S_SHOT,      //Ç±Ç±Ç≈ë≈Ç¬
        S_IDLE,      //âΩÇ‡ÇµÇ»Ç¢
        S_LEAVING,   //ãéÇÈ
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

public:
    RaserObstacle(GameObject* parent);
    ~RaserObstacle();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

