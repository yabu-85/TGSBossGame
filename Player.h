#pragma once
#include "Engine/GameObject.h"
#include "Aim.h"
#include "Engine/Text.h"

class Stage;

class Player : public GameObject
{
    int hModel_;                    //モデル番号
    int hp_;                        //今のHP
    int maxHp_;                     //最大HP
    float moveSpeed_;               //移動
    float targetRotation_;          //目標の回転角度
    float rotationSpeed_;           //回転速度
    float graY_;                    //ジャンプ時の重力計算用
    float gravity_;                 //重力の値
    float initVy_;                  //初期ジャンプ力
    float cameraHeight_;            //カメラの高さ (しゃがみとかにしか使わない？)
    float maxMoveSpeed_;            //最大移動速度
    XMFLOAT3 fMove_;                //移動方向
    XMFLOAT3 previousPosition_;     //前の座標
    XMFLOAT3 playerMovement_;       //プレイヤーの移動量
    bool firstJump_;                //ジャンプしているか
    bool secondJump_;               //ジャンプしているか2
    bool bulletJump_;               //バレットジャンプしているか
    bool isCrouching_;              //しゃがんでいるか
    bool anime_;                    //アニメーションしてる？
    bool isActive_;                 //操作を受け付けるか

    Stage* pStage_;
    Text* pText_;
    Aim* pAim_;

    void CalcMove();                    //移動方向計算・normalize
    void InstantRotate(float x, float z);               //即座に回転
    void GradualRotate(float x, float z);               //徐々に振り向く
    void Gravity();                     //重力うけます
    void Jump();                        //ジャンプするよ
    float NormalizeAngle(float angle);  //angleの値を調整する関数（振り向きが距離が近い方で向いてくれるための

    bool IsMovementKeyPressed();        //移動キーを押しているか
    bool IsPlayerMove();                //移動キーを押しているか
    bool IsPlayerOnGround();            //地面についているか

    //------------------State------------------
    enum STATE {
        S_IDLE,
        S_MOVE,
        S_CROUCH,
        S_DEAD,
    };
    STATE state_;
    bool stateEnter_; //入った時だけ呼ぶのに使う

    void ChangeState(STATE s);
    void UpdateIdle();
    void UpdateMove();
    void UpdateCrouch();
    void UpdateDead();


public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetActiveWithDelay(bool isActive);
    void SetActive(bool b) { isActive_ = b; };
    void DecreaseHp(int i) { hp_ -= i; };

    XMVECTOR GetPlaVector();                            //移動方向取得
    float GetCameraHeight() { return cameraHeight_; };  //カメラの高さ0.8f 〜 1.0f
    int GetModelHandle() { return hModel_; }
    int GetHp() { return hp_; };
    int GetMaxHp() { return maxHp_; };
};