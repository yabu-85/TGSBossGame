#pragma once
#include "Engine/Text.h"
#include "Engine/GameObject.h"

//フレームレート
static const int FPS = 60;

//タイマー管理するクラス
class Timer : public GameObject
{
    bool active_;        //起動しているかどうか
    int frame_;          //一秒間に何回更新されるか
    Text* pNum_;         //秒数テキスト
    float drawX_;        //描画位置(左右)
    float drawY_;        //描画位置(上下)

public:
    Timer(GameObject* parent);
    ~Timer();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //タイマー設定
    void SetLimit(float _sec) { frame_ = (int)(_sec * FPS); }

    //タイマー開始
    void Start() { active_ = true; }

    //タイマー停止
    void Stop() { active_ = false; }

    //タイマー終了判定
    bool IsFinished() { return (frame_ <= 0); }

    //タイム取得
    int GetTime() { return frame_ / FPS; }
};