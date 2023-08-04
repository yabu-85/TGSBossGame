#pragma once
#include "Engine/GameObject.h"

class PauseMenu :
    public GameObject
{
    int hPict_[2];

    void ButtonInitializ(); 
    void CheckButtonPressed();

public:
    PauseMenu(GameObject* parent);
    ~PauseMenu();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

