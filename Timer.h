#pragma once
#include "Engine/Text.h"
#include "Engine/GameObject.h"

//タイマー管理するクラス
class Timer : public GameObject
{
    bool Active;        //起動しているかどうか
    int Frame;          //一秒間に何回更新されるか
    Text* pNum;         //秒数テキスト
    float drawX;        //描画位置(左右)
    float drawY;        //描画高さ

public:
    Timer(GameObject* parent);      //コンストラクタ
    ~Timer();                       //デストラクタ
    void Initialize() override;     //初期化
    void Update() override;         //更新
    void Draw() override;           //描画
    void Release() override;        //開放

    //タイマー設定
    void SetLimit(float seconds);

    //タイマー開始
    void Start();

    //タイマー終了
    void Stop();

    //タイマー処理終了
    bool IsFinished();
};