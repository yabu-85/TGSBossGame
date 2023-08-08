#include "Aim.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Player.h"
#include "Engine/Image.h"
#include "Engine/Global.h"

#define SAFE_DELETE(p) if(p != nullptr){ p = nullptr; delete p;}

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirectionXY_{ 0,0,0 }, aimDirectionY_{ 0,0,0 }
    , plaPos_{ 0,0,0 }, pPlayer_(nullptr), hPict_(-1), aimDraw_(true), aimMove_(true)
{
    mouseSensitivity = 2.5f;
    perspectiveDistance_ = 3.2f;
    heightDistance_ = 1.5f;
}

Aim::~Aim()
{
    //SAFE_DELETE(pPlayer_);
}

void Aim::Initialize()
{
    pPlayer_ = (Player*)FindObject("Player");

    //画像データのロード
    hPict_ = Image::Load("cross.png");
    assert(hPict_ >= 0);
}

void Aim::Update()
{
    //マウス移動量
    if (aimMove_) {
        XMFLOAT3 mouseMove = Input::GetMouseMove(); //マウスの移動量を取得

        //移動量を計算
        transform_.rotate_.y += (mouseMove.x * 0.05f) * mouseSensitivity; //横方向の回転
        transform_.rotate_.x -= (mouseMove.y * 0.05f) * mouseSensitivity; //縦方向の回転
        if (transform_.rotate_.x <= -89.0f) transform_.rotate_.x = -89.0f;
        if (transform_.rotate_.x >= 89.0f) transform_.rotate_.x = 89.0f;
    
    }

    //カメラの回転
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

    //カメラの位置と回転を合成
    XMMATRIX mView = mRotX * mRotY; //カメラ用
    XMMATRIX mPlaMove = mRotY;     //プレイヤーの移動用   

    //プレイヤークラスに進行方向ベクトル(float3)を伝える用
    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR caDire = XMVector3TransformNormal(forwardVector, mPlaMove); //XMVector3TransformNormalを使用することで回転のみを適用します
    XMVector3Normalize(caDire);
    XMStoreFloat3(&aimDirectionY_, -caDire);

    //プレイヤーの位置をカメラの位置とする
    plaPos_ = pPlayer_->GetPosition();
    cameraPos_.x = plaPos_.x + (aimDirectionY_.z * 0.5);
    cameraPos_.y = plaPos_.y + heightDistance_ * pPlayer_->GetCameraHeight(); //目線高さ
    cameraPos_.z = plaPos_.z - (aimDirectionY_.x * 0.5);

    //カメラ焦点
    XMVECTOR caTarget = XMLoadFloat3(&cameraPos_);

    // カメラポジション
    XMVECTOR camPos = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormalを使用することで回転のみを適用します
    XMVector3Normalize(camPos);
    XMStoreFloat3(&aimDirectionXY_, -camPos);
    
    camPos = caTarget + (camPos * (perspectiveDistance_ * pPlayer_->IsAiming())); // プレイヤーの半径を考慮して回転を適用します

    XMStoreFloat3(&cameraPos_, camPos);
    XMStoreFloat3(&cameraTarget_, caTarget);

    Camera::SetPosition(cameraPos_);
    Camera::SetTarget(cameraTarget_);

}

void Aim::Draw()
{
    Transform cross;
    cross.rotate_.x = 0;
    cross.rotate_.y = 0;
    cross.rotate_.z = 0;
    cross.scale_.x = 0.5;
    cross.scale_.y = 0.5;
    cross.scale_.z = 0.5;

    Image::SetTransform(hPict_, cross);
    Image::Draw(hPict_);
}

void Aim::Release()
{
}
