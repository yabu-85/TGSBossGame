#pragma once
#include "Engine/GameObject.h"

class Player;
class Stage;

class Aim :
    public GameObject
{
    int hPict_;                     //画像番号
    int shakeTimer_;                //シェイクに使う時間
    float shakeAmplitude_;          //カメラシェイクの振幅（0～1）
    float shakeStrength_;           //シェイクの強さ
    float mouseSensitivity;         //マウス感度
    float perspectiveDistance_;     //どのくらい後ろから映すか
    float heightDistance_;          //焦点の高さ
    bool aimDraw_;                  //エイムを表示するか
    bool aimMove_;                  //エイムを動かすかどうか
    bool isShaking_;                //シェイクしているかどうか
    XMFLOAT3 cameraTarget_;         //カメラの焦点目標
    XMFLOAT3 cameraPos_;            //カメラの場所目標
    XMFLOAT3 aimDirectionXY_;       //現在の視点に基づいた進行方向ベクトル
    XMFLOAT3 aimDirectionY_;        //現在の視点に基づいた進行方向ベクトル
    XMFLOAT3 plaPos_;               //プレイヤー位置
    Transform cross_;               //AimのTransform
    Player* pPlayer_;
    Stage* pStage_;

public:
    Aim(GameObject* parent);
    ~Aim();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //進行方向ベクトルのAim情報を取得
    XMFLOAT3 GetAimDirectionY() { return aimDirectionY_; }
    XMFLOAT3 GetAimDirectionXY() { return aimDirectionXY_; }
    void SetAimDraw(bool b) { aimDraw_ = b; };
    void SetAimMove(bool b) { aimMove_ = b; };

    //シェイクに使う時間用
    //カメラシェイクの強さ
    //カメラシェイクの振幅（初期値：0～1）
    void TriggerCameraShake(int t, float s, float a);

};

