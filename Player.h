#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"
#include "PlayerSpeedController.h"

class Aim;
class Stage;

class Player : public GameObject
{
    Text* pText;

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

    int hModel_;                    //モデル番号
    int hPict_;
    int hp_;                        //今のHP
    int maxHp_;                     //最大HP
    float moveSpeed_;               //移動
    float targetRotation_;          //目標の回転角度
    float graY_;                    //ジャンプ時の重力計算用
    float cameraHeight_;            //カメラの高さ (しゃがみとかにしか使わない？)
    float maxMoveSpeed_;            //最大移動速度
    float prevYHeight_;             //埋まった時用の
    bool firstJump_;                //ジャンプしているか
    bool secondJump_;               //ジャンプしているか2
    bool bulletJump_;               //バレットジャンプしているか
    bool isCrouching_;              //しゃがんでいるか
    bool anime_;                    //アニメーションしてる？
    bool isActive_;                 //操作を受け付けるか
    XMFLOAT3 fMove_;                //移動方向
    XMFLOAT3 playerMovement_;       //プレイヤーの移動量

    Stage* pStage_;
    Aim* pAim_;
    PlayerSpeedController* pSpeedCtrl_;

    //引数value：正規化する時にかける値
    void GetMoveDirection(float value = 1.0f);

    void CalcMove();                                    //移動方向計算・normalize
    void Rotate(float x, float z, float _rotateSpeed);  //回転する
    void Gravity();                                     //重力うけます
    void PlayerFallReset();                             //落下した時のフラグなどのリセット
    void Jump();                                        //ジャンプするよ
    float NormalizeAngle(float angle);                  //angleの値を調整する関数（振り向きが距離が近い方で向いてくれるための
    bool IsMovementKeyPressed();                        //移動キーを押しているか
    bool IsPlayerOnGround();                            //地面についているか
    void IsInWall();

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //Activeを遅延を入れてセット/1000=1秒
    void SetActiveWithDelay(bool isActive, int time);
    void SetActive(bool b) { isActive_ = b; };  //Activeをセット
    void DecreaseHp(int i);       //Hpを減らす関数

    float GetCameraHeight() { return cameraHeight_; };  //カメラの高さ0.8f ～ 1.0f
    int GetModelHandle() { return hModel_; }
    int GetHp() { return hp_; };
    int GetMaxHp() { return maxHp_; };

    bool IsisCrouc() { return isCrouching_; };
    void SetPlayerMovement(XMFLOAT3 _move) { playerMovement_ = _move; };

    void ResetPlayer();

};