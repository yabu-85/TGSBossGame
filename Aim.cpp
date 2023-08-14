#include "Aim.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Player.h"
#include "Engine/Image.h"
#include "Engine/Global.h"
#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/VFX.h"
#include <map>

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirectionXY_{ 0,0,0 }, aimDirectionY_{ 0,0,0 }, plaPos_{ 0,0,0 },
    pPlayer_(nullptr), hPict_(-1), aimDraw_(true), aimMove_(true), isShaking_(false), shakeTimer_(0), shakeAmplitude_(1.0f), shakeStrength_(0.0f),
    pStage_(nullptr), shakeTimerSub_(0)

{
    mouseSensitivity = 2.5f;
    perspectiveDistance_ = 3.2f;
    heightDistance_ = 1.5f;
    cross_.scale_ = { 0.5f, 0.5f, 0.5f };

}

Aim::~Aim()
{
}

void Aim::Initialize()
{
    pPlayer_ = (Player*)FindObject("Player");
    pStage_ = (Stage*)FindObject("Stage");

    //画像データのロード
    hPict_ = Image::Load("cross.png");
    assert(hPict_ >= 0);

}

static int num = 0;

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
    cameraPos_.x = plaPos_.x + (aimDirectionY_.z * 0.5f);
    cameraPos_.y = plaPos_.y + heightDistance_ * pPlayer_->GetCameraHeight(); //目線高さ
    cameraPos_.z = plaPos_.z - (aimDirectionY_.x * 0.5f);

    //カメラ焦点
    XMVECTOR caTarget = XMLoadFloat3(&cameraPos_);

    // カメラポジション
    XMVECTOR camPos = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormalを使用することで回転のみを適用します
    XMVector3Normalize(camPos);
    XMStoreFloat3(&aimDirectionXY_, -camPos);

    if (isShaking_) {
        float shakeStr = shakeStrength_ / (float)shakeTimerSub_;
        shakeAmplitude_ -= shakeStr;
        shakeTimer_--;

        //シェイクが終了したら元の位置に戻すアニメーションを開始
        if (shakeTimer_ <= 0) {
            shakeAmplitude_ += shakeStr * 2;

            //おわり
            if (shakeAmplitude_ >= 1.0f) {
                isShaking_ = false;
                shakeAmplitude_ = 1.0f;
            }
        }
    }

    int plahModel = pPlayer_->GetModelHandle();
    XMFLOAT3 weapTop = Model::GetBoneAnimPosition(plahModel, "MeleeTop");
    XMFLOAT3 weapRoot = Model::GetBoneAnimPosition(plahModel, "Melee");

    XMVECTOR vTop = XMLoadFloat3(&weapTop);
    XMVECTOR vRoot = XMLoadFloat3(&weapRoot);
    XMVECTOR vMove = vTop - vRoot;
    vMove = XMVector3Normalize(vMove);
    vMove *= 0.3f;
    XMFLOAT3 move;
    XMStoreFloat3(&move, vMove);

    XMFLOAT3 emitPos2 = XMFLOAT3(weapRoot.x + move.x, weapRoot.y + move.y, weapRoot.z + move.z);
    XMFLOAT3 emitPos3 = XMFLOAT3(weapRoot.x + move.x + move.x, weapRoot.y + move.y + move.y, weapRoot.z + move.z + move.z);
    XMFLOAT3 emitPos4 = XMFLOAT3(weapRoot.x + move.x + move.x + move.x, weapRoot.y + move.y + move.y + move.y, weapRoot.z + move.z + move.z + move.z);

    EmitterData data1;
    data1.textureFileName = "cloudA.png";
    data1.position = XMFLOAT3(weapRoot.x, weapRoot.y, weapRoot.z);
    data1.positionRnd = XMFLOAT3(0.01f, 0.01f, 0.01f);
    data1.direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
    data1.delay = 0;
    data1.gravity = 0.0f;
    data1.number = 1;
    data1.lifeTime = 3;
    data1.size = XMFLOAT2(0.3f, 0.3f);
    data1.color = XMFLOAT4(0.0f, 0.7f, 1.0f, 1.0f);
    VFX::Start(data1);

    EmitterData data2 = data1;
    data2.position = emitPos2;
    VFX::Start(data2);

    EmitterData data3 = data1;
    data3.position = emitPos3;
    VFX::Start(data3);

    EmitterData data4 = data1;
    data4.position = emitPos4;
    VFX::Start(data4);
    
    //プレイヤーの半径を考慮して回転を適用します
    //ここAimの近さの値をプレイヤーから取得して計算もしてる
    camPos = caTarget + (camPos * (perspectiveDistance_ * pPlayer_->IsAiming() * shakeAmplitude_));

    XMStoreFloat3(&cameraPos_, camPos);
    XMStoreFloat3(&cameraTarget_, caTarget);

    Camera::SetPosition(cameraPos_);
    Camera::SetTarget(cameraTarget_);

}

void Aim::Draw()
{
    Image::SetTransform(hPict_, cross_);
    Image::Draw(hPict_);

}

void Aim::Release()
{
}

void Aim::TriggerCameraShake(int t, float s)
{
    isShaking_ = true;
    shakeTimer_ = t / 2;
    shakeTimerSub_ = shakeTimer_;
    if (shakeTimer_ == 0) {
        shakeTimer_ = 1;
        shakeTimerSub_ = 1;
    }

    shakeStrength_ = s;
    shakeAmplitude_ = 1.0f;
    
}