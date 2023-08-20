#pragma once
#include "Engine/GameObject.h"

//タイマー管理するクラス
class Stageselect : public GameObject
{
    void CheckButtonPressed();
    void ButtonInitializ();

public:
    Stageselect(GameObject* parent);
    ~Stageselect();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};