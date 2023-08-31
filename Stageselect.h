#pragma once
#include "Engine/GameObject.h"

class ButtonFactory;

//�^�C�}�[�Ǘ�����N���X
class StageSelect : public GameObject
{
    int hPict_;
    int time_;
    ButtonFactory* pButtonFactory_;
    bool standBy_;        //�ҋ@�t���O
    int stateCount_;        //�ҋ@����

public:
    StageSelect(GameObject* parent);
    ~StageSelect();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};