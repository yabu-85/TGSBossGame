#pragma once
#include "Engine/GameObject.h"

//タイマー管理するクラス
class StageSelect : public GameObject
{
    int hPict_;
    int time_;

    void CheckButtonPressed();
    void ButtonInitializ();

public:
    StageSelect(GameObject* parent);
    ~StageSelect();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};