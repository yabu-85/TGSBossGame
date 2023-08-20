#pragma once
#include "Engine/GameObject.h"

//�^�C�}�[�Ǘ�����N���X
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