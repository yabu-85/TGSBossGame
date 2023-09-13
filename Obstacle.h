#pragma once
#include "Engine/GameObject.h"

//Obstacle‚ÌŠî’êƒNƒ‰ƒX
class Obstacle : public GameObject
{
protected:
    int hModel_;
    bool active_;
    bool isDraw_;
    XMFLOAT3 csvPos_;

public:
    Obstacle(GameObject* parent);
    ~Obstacle();
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void Release() override;
    
    void SetActive(bool b) { active_ = b; };
    void SetDraw(bool b) { isDraw_ = b; };

    void SetCsvPos(XMFLOAT3 pos) { csvPos_ = pos; };
    XMFLOAT3 GetCsvPos() { return csvPos_ ; };
    
    virtual void KillMeSub() = 0;

};

