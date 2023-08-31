#pragma once
#include "Engine/GameObject.h"

//Obstacleの基底クラス
class Obstacle : public GameObject
{
protected:
    int hModel_;
    bool active_;
    bool isDraw_;

public:
    Obstacle(GameObject* parent);
    ~Obstacle();
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void Release() override;
    
    void SetActive(bool b) { active_ = b; };
    void SetDraw(bool b) { isDraw_ = b; };
    bool IsActive() { return active_; };

};

