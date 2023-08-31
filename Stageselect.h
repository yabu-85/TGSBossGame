#pragma once
#include "Engine/GameObject.h"

class ButtonFactory;

//タイマー管理するクラス
class StageSelect : public GameObject
{
    int hPict_;
    int hSound_;    //サウンド番号
    int hSound2_;    //サウンド番号
    int time_;
    ButtonFactory* pButtonFactory_;

public:
    StageSelect(GameObject* parent);
    ~StageSelect();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};