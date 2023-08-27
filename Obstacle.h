#pragma once
#include "Engine/GameObject.h"

//Obstacle‚ÌŠî’êƒNƒ‰ƒX
class Obstacle : public GameObject
{
protected:
    int hModel_;
    bool active_;

public:
    Obstacle(GameObject* parent);
    ~Obstacle();
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void Release() override;
    
    void SetActive(bool b) { active_ = b; };
    bool IsActive() { return active_; };

};

