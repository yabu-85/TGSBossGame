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

//◆◆◆を管理するクラス
class Stage : public GameObject
{
    int hModel_[TYPE_MAX];    //モデル番号
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
    int GetModelHandle(int i) { return hModel_[i]; }; //モデル番号取得
    XMFLOAT3 GetPlaPos(); //CSVでのプレイヤー座標

    int GetHeight() { return height_; };

};

