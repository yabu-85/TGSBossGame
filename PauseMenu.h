#pragma once
#include "Engine/GameObject.h"

class ButtonFactory;

class PauseMenu :
    public GameObject
{
    int hPict_[2];
    ButtonFactory* pButtonFactory_;

public:
    PauseMenu(GameObject* parent);
    ~PauseMenu();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

