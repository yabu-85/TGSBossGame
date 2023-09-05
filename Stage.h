#pragma once
#include "Engine/GameObject.h"
#include "Player.h"

//オブジェクトの種類を判別する番号
enum OBJ
{
    TYPE_FLOOR,         //床
    TYPE_MAX            //最大値
};

enum CHARACTER
{
    TYPE_PLAYER = 10    //プレイヤー
};

//ステージを管理するクラス
class Stage : public GameObject
{    
    int width_;                 //ステージ幅
    int height_;                //ステージ高さ
    int hModel_[TYPE_MAX];      //モデル番号
    int** table_;               //ステージセルのデータ

public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    
    float GetFloorHeight(int x, int z);     //床の高さを取得する関数
    bool IsWall(int x, int z);              //壁であるか判定する関数
    int GetModelHandle(int i) { return hModel_[i]; } //モデル番号を取得する関数
    XMFLOAT3 GetPlaPos();                   //CSV上でのプレイヤー座標を取得する関数
    int GetHeight() { return height_; };    //マップの高さ（ゴールまで）
    
    //プレイヤーが落ちた時の一番近い床
    XMFLOAT3 NearestFloorLocation(XMFLOAT3 pos);

    //
    void InitMapData(std::string mp);

};

