#pragma once
#include "Engine/GameObject.h"

class ButtonFactory;
class SliderButton;

class Setting :
    public GameObject
{
    int hPict_[4];
    ButtonFactory* pButtonFactory_;
    SliderButton* pSlider_;
    SliderButton* pSlider2_;

public:
    Setting(GameObject* parent);
    ~Setting();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

