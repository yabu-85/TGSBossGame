#pragma once
#include "Engine/GameObject.h"

class PauseMenu :
    public GameObject
{
    int hModel_[5];                    //ƒ‚ƒfƒ‹”Ô†

public:
    PauseMenu(GameObject* parent);
    ~PauseMenu();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

