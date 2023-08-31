#pragma once
#include "Engine/GameObject.h"

class ButtonFactory;

//タイマー管理するクラス
class StageSelect : public GameObject
{
    int hPict_;
    int time_;
    ButtonFactory* pButtonFactory_;
    bool standBy_;        //待機フラグ
    int stateCount_;        //待機時間

public:
    StageSelect(GameObject* parent);
    ~StageSelect();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};