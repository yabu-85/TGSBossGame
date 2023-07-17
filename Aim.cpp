#include "Aim.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Player.h"

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 }, plaPos_{ 0,0,0 }, pPlayer_(nullptr),
    currentCameraPos_{ 0,0,0 }, currentCameraTar_{ 0,0,1 }
{
    mouseSensitivity = 2.5f;
    perspectiveDistance_ = 4.3f;
    heightDistance_ = 2.0f;
    maxLeng_ = 5.0f;
    cameraSpeed_ = 0.08f;
}

Aim::~Aim()
{
}

void Aim::Initialize()
{
    pPlayer_ = (Player*)FindObject("Player");

    //マウス初期位置
    Input::SetMousePosition(800 / 2, 600 / 2);

}

void Aim::Update()
{
    //プレイヤーの移動方向によって回転させるやつ
    //未完成＜＝ーーーーーーーーーーーーーーーーーー
    XMVECTOR v = pPlayer_->GetPlaVector();
    //if (aimDirection_.z < 0.0) transform_.rotate_.y += XMVectorGetX(v) * 7;
    //else transform_.rotate_.y -= XMVectorGetX(v) * 7;


    //マウス移動量
    XMFLOAT3 mouseMove = Input::GetMouseMove(); //マウスの移動量を取得

    //移動量を加算
    transform_.rotate_.y += (mouseMove.x * 0.05f) * mouseSensitivity; //横方向の回転
    transform_.rotate_.x -= (mouseMove.y * 0.05f) * mouseSensitivity; //縦方向の回転

    //カメラの回転
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    XMMATRIX matT = XMMatrixTranslation(0, 0, perspectiveDistance_);

    //カメラの位置と回転を合成
    XMMATRIX mView = mRotX * mRotY * matT; //カメラ用
    XMMATRIX mPlaMove = mRotX * mRotY;     //プレイヤーの移動用

    //プレイヤー座標取得
    plaPos_ = pPlayer_->GetPlaPos();

    //プレイヤーキャラクターの位置をカメラの位置とする
    cameraPos_.x = plaPos_.x;
    cameraPos_.y = plaPos_.y + heightDistance_; //目線高さ
    cameraPos_.z = plaPos_.z;

    //プレイヤークラスに進行方向ベクトル(float3)を伝える用   
    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR caDire = XMVector3TransformNormal(forwardVector, mPlaMove); //XMVector3TransformNormalを使用することで回転のみを適用します
    XMStoreFloat3(&aimDirection_, -caDire);

    //カメラ焦点
    XMVECTOR caTarget = XMLoadFloat3(&cameraPos_);

    // カメラポジション
    XMVECTOR camPos = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormalを使用することで回転のみを適用します
    camPos = caTarget + (camPos * perspectiveDistance_); // プレイヤーの半径を考慮して回転を適用します

    XMStoreFloat3(&cameraPos_, camPos);
    XMStoreFloat3(&cameraTarget_, caTarget);


#if 1

    //プレイヤーの移動ー＞移動なしになった場合のなんたらが必要

    static bool inputKey = false;

    //-----------だんだん動くやつ
    if (pPlayer_->IsPlayerMove()) {
        XMVECTOR vCameraPosDist, vCameraTarDist;
        float posDx, posDy, posDz;

        do {
            vCameraPosDist = cameraPos_ - currentCameraPos_;
            vCameraTarDist = cameraTarget_ - currentCameraTar_;
            currentCameraPos_ += (vCameraPosDist * cameraSpeed_);
            currentCameraTar_ += (vCameraTarDist * cameraSpeed_);

            posDx = XMVectorGetX(XMVector3Length(vCameraPosDist));
            posDy = XMVectorGetY(XMVector3Length(vCameraPosDist));
            posDz = XMVectorGetZ(XMVector3Length(vCameraPosDist));

        } while ((posDx + posDy + posDz) > maxLeng_);

        Camera::SetPosition(currentCameraPos_);
        Camera::SetTarget(currentCameraTar_);

        inputKey = true;

    }
    else if (!pPlayer_->IsPlayerMove() && inputKey) {
        XMVECTOR vCameraPosDist, vCameraTarDist;
        float posDx, posDy, posDz;

        do {
            vCameraPosDist = cameraPos_ - currentCameraPos_;
            vCameraTarDist = cameraTarget_ - currentCameraTar_;
            currentCameraPos_ += (vCameraPosDist * cameraSpeed_);
            currentCameraTar_ += (vCameraTarDist * cameraSpeed_);

            posDx = XMVectorGetX(XMVector3Length(vCameraPosDist));
            posDy = XMVectorGetY(XMVector3Length(vCameraPosDist));
            posDz = XMVectorGetZ(XMVector3Length(vCameraPosDist));

        } while ((posDx + posDy + posDz) > maxLeng_);

        Camera::SetPosition(currentCameraPos_);
        Camera::SetTarget(currentCameraTar_);

        if (abs(posDx + posDy + posDz) <= 0.01f) inputKey = false;

    }
    else
    {
        Camera::SetPosition(cameraPos_);
        Camera::SetTarget(cameraTarget_);

        currentCameraPos_ = cameraPos_;
        currentCameraTar_ = cameraTarget_;

        inputKey = false;
    }

#else

    XMVECTOR vCameraPosDist, vCameraTarDist;
    int posDx, posDy, posDz;

    do {
        vCameraPosDist = cameraPos_ - currentCameraPos_;
        vCameraTarDist = cameraTarget_ - currentCameraTar_;
        currentCameraPos_ += (vCameraPosDist * cameraSpeed_);
        currentCameraTar_ += (vCameraTarDist * cameraSpeed_);

        posDx = XMVectorGetX(XMVector3Length(vCameraPosDist));
        posDy = XMVectorGetY(XMVector3Length(vCameraPosDist));
        posDz = XMVectorGetZ(XMVector3Length(vCameraPosDist));

    } while ((posDx + posDy + posDz) > maxLeng_);

    Camera::SetPosition(currentCameraPos_);
    Camera::SetTarget(currentCameraTar_);

#endif


}

void Aim::Draw()
{
}

void Aim::Release()
{
}
