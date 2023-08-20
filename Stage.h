#pragma once
#include "Engine/GameObject.h"
#include "Player.h"

enum {
    TYPE_FLOOR,
    TYPE_WALL,
    TYPE_WALL2,
    TYPE_HIGHTWALL,
    TYPE_SLOPE,
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

    float GetFloorHeight(int x, int z);
    bool IsWall(int x, int z);
    int GetModelHandle(int i) { return hModel_[i]; }; //���f���ԍ��擾
    XMFLOAT3 GetPlaPos(); //CSV�ł̃v���C���[���W

    int GetHeight() { return height_; };

};

