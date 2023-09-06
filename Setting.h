#pragma once
#include "Engine/GameObject.h"

class ButtonFactory;

class Setting :
    public GameObject
{
    int hPict_[2];
    ButtonFactory* pButtonFactory_;

public:
    Setting(GameObject* parent);
    ~Setting();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

