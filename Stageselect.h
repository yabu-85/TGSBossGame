#pragma once
#include "Engine/GameObject.h"

//�^�C�}�[�Ǘ�����N���X
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