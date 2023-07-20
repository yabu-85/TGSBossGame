#pragma once
#include "Engine/GameObject.h"

class Player;

class Aim :
    public GameObject
{
    int hPict_;                     //画像番号
    float mouseSensitivity;         //マウス感度
    float perspectiveDistance_;     //どのくらい後ろから映すか
    float heightDistance_;          //焦点の高さ
    float cameraSpeed_;             //カメラの速度
    XMFLOAT3 cameraTarget_;         //カメラの焦点目標
    XMFLOAT3 cameraPos_;            //カメラの場所目標
    XMFLOAT3 aimDirection_;         //現在の視点に基づいた進行方向ベクトル
    XMFLOAT3 plaPos_;               //プレイヤー位置
    Player* pPlayer_;

public:
    Aim(GameObject* parent);
    ~Aim();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //進行方向ベクトルのAim情報を取得
    XMFLOAT3 GetAimDirection() { return aimDirection_; }
};

