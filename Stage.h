#pragma once
#include "Engine/GameObject.h"

class Player;

//オブジェクトの種類を判別する番号
enum OBJ
{
    TYPE_FLOOR,         //床
    TYPE_MAX,            //最大値
    
    TYPE_PLAYER = 10
};

//ステージを管理するクラス
class Stage : public GameObject
{    
    int width_;                 //ステージ幅
    int height_;                //ステージ高さ
    int hModel_;      //モデル番号
    int** table_;               //ステージセルのデータ
    XMFLOAT3 plaPos_;
    Player* pPlayer_;

public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    float GetFloorHeight(int x, int z);         //床の高さを取得する関数
    bool IsWall(int x, int z);                  //壁であるか判定する関数
    int GetModelHandle(int i) { return hModel_; } //モデル番号を取得する関数
    XMFLOAT3 GetPlaPos() { return plaPos_; };   //CSV上でのプレイヤー座標を取得する関数
    int GetHeight() { return height_; };        //マップの高さ（ゴールまで）
    
    //プレイヤーが落ちた時の一番近い床
    XMFLOAT3 NearestFloorLocation(XMFLOAT3 pos);

    //マップデータ初期化
    void InitMapData(std::string mp);

};

