#pragma once
#include "Engine/GameObject.h"
#include "Player.h"

enum {
    TYPE_FLOOR,
    TYPE_WALL,
    TYPE_MAX
};

//���������Ǘ�����N���X
class Stage : public GameObject
{
    int hModel_[TYPE_MAX];    //���f���ԍ�
    int** table_;
    int width_;
    int height_;

public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    int IsWall(int x, int z); //�ǁH
    int GetModelHandle() { return hModel_[TYPE_WALL]; }; //���f���ԍ��擾
    XMFLOAT3 GetPlaPos();

};

