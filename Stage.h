#pragma once
#include "Engine/GameObject.h"
#include "Player.h"

enum {
    TYPE_FLOOR,
    TYPE_WALL,
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

    int IsWall(int x, int z); //１壁、０床 それ以外ー１
    int GetModelHandle() { return hModel_[TYPE_WALL]; }; //モデル番号取得
    XMFLOAT3 GetPlaPos(); //CSVでのプレイヤー座標

};

