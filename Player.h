#pragma once
#include "Engine/GameObject.h"
#include "Aim.h"

#define SAFE_DELETE(p) if(p != nullptr){ p = nullptr; delete p;}

class Player : public GameObject
{
    int hModel_;                    //モデル番号
    float moveSpeed_;               //移動
    float targetRotation_;          //目標の回転角度
    float rotationSpeed_;           //回転速度
    float graY_;                    //ジャンプ時の重力計算用
    float gravity_;                 //重力の値
    float initVy_;                  //初期ジャンプ力
    bool jump_;                     //ジャンプしているか
    XMFLOAT3 fMove_;                //移動方向
    XMFLOAT3 previousPosition_;     //前の座標
    Aim* pAim_;

    void CalcMoveVec();

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //ゲッター
    XMFLOAT3 GetPlaPos() { return transform_.position_; };
    XMFLOAT3 GetPlaRotate() { return transform_.rotate_; };
    bool IsPlayerOnGround();
    bool IsPlayerMove();
    XMVECTOR GetPlaVector();

};