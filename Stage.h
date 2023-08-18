#pragma once
#include "Engine/GameObject.h"
#include "Player.h"

//オブジェクトの種類を判別する番号
enum 
{
    TYPE_FLOOR,         //床
    TYPE_WALL,          //壁
    TYPE_WALL2,         //壁(大)
    TYPE_HIGHTWALL,     //壁(特大)
    TYPE_SLOPE,         //傾斜
    TYPE_MAX            //最大値
};

//ステージを管理するクラス
class Stage : public GameObject
{    
    int width_;                 //ステージ幅
    int height_;                //ステージ高さ
    int hModel_[TYPE_MAX];      //モデル番号
    int** table_;               //ステージセルのデータ

public:
    //コンストラクタ
    Stage(GameObject* parent);

    //デストラクタ
    ~Stage();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;


    //床の高さを取得する関数
    float GetFloorHeight(int x, int z);

    //セルが壁であるか判定する関数
    bool IsWall(int x, int z);

    //モデル番号を取得する関数
    int GetModelHandle(int i) { return hModel_[i]; };

    //CSV上でのプレイヤー座標を取得する関数
    XMFLOAT3 GetPlaPos(); 
};

