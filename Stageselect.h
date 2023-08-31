#pragma once
#include "Engine/GameObject.h"

class ButtonFactory;

//�^�C�}�[�Ǘ�����N���X
class StageSelect : public GameObject
{
    int hPict_;
    int hSound_;    //�T�E���h�ԍ�
    int hSound2_;    //�T�E���h�ԍ�
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