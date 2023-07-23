#pragma once
#include "Engine/GameObject.h"
#include "Aim.h"
#include "Engine/Text.h"

#define SAFE_DELETE(p) if(p != nullptr){ p = nullptr; delete p;}

class Player : public GameObject
{
    Text* pText_;
    int hModel_;                    //モデル番号
    float moveSpeed_;               //移動
    float targetRotation_;          //目標の回転角度
    float rotationSpeed_;           //回転速度
    float graY_;                    //ジャンプ時の重力計算用
    float gravity_;                 //重力の値
    float initVy_;                  //初期ジャンプ力
    float cameraHeight_;            //カメラの高さ (しゃがみとかにしか使わない？)
    float movementRatio_;           //プレイヤーの移動速度の比率0～1
    XMFLOAT3 fMove_;                //移動方向
    XMFLOAT3 previousPosition_;     //前の座標
    XMFLOAT3 playerMovement_;       //プレイヤーの移動量
    bool firstJump_;                //ジャンプしているか
    bool secondJump_;               //ジャンプしているか
    bool bulletJump_;               //バレットジャンプしているか
    bool isCrouching_;              //しゃがんでいるか
    bool anime_;                    
    Aim* pAim_;

    //---------------privateメンバ関数---------------

    void CalcMove();                    //移動方向計算・normalize
    void InstantRotate();               //即座に回転
    void GradualRotate();               //徐々に振り向く
    void Gravity();                     //重力うけます
    void Crouch();                      //しゃがみ関係です
    void Jump();                        //ジャンプするよん
    void CalcMoveRatio(bool type);      //true=プラス１ false=マイナス１
    
    float NormalizeAngle(float angle);  //angleの値を調整する関数（振り向きが距離が近い方で向いてくれるための

    bool IsMovementKeyPressed();        //移動キーを押しているか
    bool IsPlayerMove();                //移動キーを押しているか
    bool IsPlayerOnGround();            //地面についているか

    //------------------State------------------
    enum STATE {
        S_IDLE,
        S_MOVE,
        S_DEAD,
    };
    STATE state_;

    void UpdateIdle();
    void UpdateMove();
    void UpdateDead();


public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //ーーーーーーゲッターーーーーーーー
    XMVECTOR GetPlaVector();                            //移動方向取得
    float GetCameraHeight() { return cameraHeight_; };  //カメラの高さ0.8f ～ 1.0f

};