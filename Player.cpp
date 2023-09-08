#include "Player.h"
#include "Engine/Input.h"
#include "Engine/VFX.h"
#include "Engine/Model.h"
#include "Aim.h"
#include "Stage.h"
#include "HpGauge.h"
#include <thread>
#include <chrono>
#include "ObstacleManager.h"
#include "Engine/Image.h"
#include "AudioManager.h"

#define SAFE_DELETE(p) if(p != nullptr){ p = nullptr; delete p;}

namespace {
    const float airMoveSpeed = 0.002f;
    const float crouchSpeed = 0.0005f;

    const float gravity_ = 0.0075f;     //重力の値
    const float initVy_ = 0.17f;        //初期ジャンプ力

    const float noDe = 0.8f;            //減速をやめる時間
    const float deTime = 0.0055f;       //時間の速さ

    //バレットジャンプの強さ
    const float buJumpY = 1.5f;
    const float buJumpXZ = 0.2f;

}

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_(-1), targetRotation_(0), firstJump_(false), secondJump_(false), prevYHeight_(0),
    isCrouching_(false), graY_(0), fMove_{ 0,0,0 }, state_(S_IDLE), anime_(false), pAim_(nullptr), cameraHeight_(1.0f),
    playerMovement_{ 0,0,0 }, bulletJump_(false), pStage_(nullptr), maxMoveSpeed_(1.0f), isActive_(false),stateEnter_(true),
    hp_(0), maxHp_(0), pText(nullptr), hPict_(-1), pSpeedCtrl_(nullptr), pState_(nullptr)
{
    moveSpeed_ = 1.5f;
}

Player::~Player()
{
}

void Player::Initialize()
{
    pText = new Text;
    pText->Initialize();

    transform_.scale_.x = 0.2f;
    transform_.scale_.y = 0.2f;
    transform_.scale_.z = 0.2f;
    transform_.rotate_.y = 0.0f;

    //モデルデータのロード
    hModel_ = Model::Load("Model/huu.fbx");
    assert(hModel_ >= 0);

    hp_ = 50;
    maxHp_ = 50;
    Model::SetAnimFrame(hModel_, 0, 0, 1);

    pStage_ = (Stage*)FindObject("Stage");

    //画像データのロード
    hPict_ = Image::Load("Png/ColorDamage.png");
    assert(hPict_ >= 0);

}

void Player::Update()
{
    if (!isActive_) return;

    switch (state_) {
    case STATE::S_IDLE:
        UpdateIdle();
        break;
    case STATE::S_MOVE:
        UpdateMove();
        break;
    case STATE::S_CROUCH:
        UpdateCrouch();
        break;
    case STATE::S_DEAD:
        UpdateDead();
        break;
    }

    //移動させる
    transform_.position_.x += ((playerMovement_.x * moveSpeed_) * pSpeedCtrl_->GetMoveSpeed_()); // 移動！
    transform_.position_.z += ((playerMovement_.z * moveSpeed_) * pSpeedCtrl_->GetMoveSpeed_()); // z

    //埋まった時の対処（できるなら元のバグをなくしたい
    if (transform_.position_.y < 0.0f && prevYHeight_ == transform_.position_.y) {
        transform_.position_.y = pStage_->GetFloorHeight((int)transform_.position_.x, (int)transform_.position_.z);
        if (!IsMovementKeyPressed()) playerMovement_ = { 0,0,0 };
    }
    prevYHeight_ = transform_.position_.y;

    //壁との当たり判定
    IsInWall();

    //ポジションの制限
    if (transform_.position_.x <= 0.0f) transform_.position_.x = 0.0f;
    if (transform_.position_.x >= 6.9f) transform_.position_.x = 6.9f;

    //落下した時の処理
    if (transform_.position_.y <= -9.0f) {
        transform_.position_ = pStage_->NearestFloorLocation(transform_.position_);
        PlayerFallReset();
    }

    //移動するなら向きを変える
    if (IsMovementKeyPressed()) Rotate(playerMovement_.x, playerMovement_.z, 13.0f);

    //重力
    if (!IsPlayerOnGround()) Gravity();

    //jump
    if (Input::IsKeyDown(DIK_SPACE))
    {
        AudioManager::PlaySoundMa(AUDIO_JUNPING_START);
        Jump();
    }

    //しゃがんでない時カメラの高さリセット
    if (state_ != S_CROUCH) {
        if (cameraHeight_ < 1.0f)cameraHeight_ += 0.02f;

        if (Input::IsKey(DIK_F) && !IsPlayerOnGround()) isCrouching_ = true;
        else isCrouching_ = false;
    }

    //反射
    if (Input::IsMouseButtonDown(1)) {
        ObstacleManager* pObsM = (ObstacleManager*)FindObject("ObstacleManager");
        pObsM->Reflect();
        
        AudioManager::PlaySoundMa(AUDIO_REFLECTION);

        //エフェクト
        EmitterData data;
        data.textureFileName = "Particle/defaultParticle.png";
        data.position = transform_.position_;
        data.position.y += 0.9f;
        data.positionRnd = XMFLOAT3(0.1f, 0.0f, 0.1f);
        data.direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
        data.directionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
        data.speed = 0.1f;
        data.speedRnd = 0.0f;
        data.accel = 1.0f;
        data.delay = 0;
        data.number = 5;
        data.gravity = 0.0f;
        data.lifeTime = 5;
        data.color = XMFLOAT4(0.0f, 0.2f, 1.0f, 1.0f);
        data.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.1f, 0.0f);
        data.size = XMFLOAT2(0.2f, 0.2f);
        data.sizeRnd = XMFLOAT2(0.1f, 0.1f);
        data.scale = XMFLOAT2(1.7f, 1.7f);
        data.isBillBoard = true;
        VFX::Start(data);
    }

    //SpeedCtrl
    if (IsMovementKeyPressed() && IsPlayerOnGround()) {
        pSpeedCtrl_->AddRunTime();
        AudioManager::PlaySoundMa(AUDIO_RUNNING);
    }
    else {
        AudioManager::StopSoundMa(AUDIO_RUNNING);

        //地上かつ止まっていたら速度リセット
        if(IsPlayerOnGround()) pSpeedCtrl_->ResetSpeed();
    }

    //テスト用
    if (Input::IsKey(DIK_UPARROW)) {
        DecreaseHp(1);
    }
}

void Player::Draw()
{
    pText->Draw(30, 30, (int)transform_.position_.x);
    pText->Draw(30, 70, (int)transform_.position_.z);
    pText->Draw(30, 150, (int)pStage_->GetFloorHeight((int)transform_.position_.x, (int)transform_.position_.z));

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

}

void Player::Release()
{
    SAFE_DELETE(pAim_);
    SAFE_DELETE(pStage_);
}

void Player::SetActiveWithDelay(bool isActive, int time)
{
    //1秒後に実際のアクティブ状態を設定するタイマーをセットアップ
    std::thread([this, isActive, time]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
        isActive_ = isActive;
        pAim_ = (Aim*)FindObject("Aim");
        pAim_->SetAimMove(true);
        pSpeedCtrl_ = (PlayerSpeedController*)FindObject("PlayerSpeedController");

    }).detach();
}

void Player::DecreaseHp(int i)
{
    hp_ -= i;
    pAim_->TriggerCameraShake(10, 1.0f);

}

/*--------------------------------State------------------------*/

void Player::ChangeState(STATE s)
{
    state_ = s;
    stateEnter_ = true;
}

void Player::UpdateIdle()
{
    if (IsMovementKeyPressed()) {
        if (stateEnter_) {
            Model::SetAnimFrame(hModel_, 30, 60, 1);
            stateEnter_ = false;
        }

        ChangeState(S_MOVE);
    }
}

void Player::UpdateMove()
{
    CalcMove();

    if (!IsMovementKeyPressed()) {
        Model::SetAnimFrame(hModel_, 0, 0, 1);
        ChangeState(S_IDLE);
    }
    if (Input::IsKeyDown(DIK_F) && IsPlayerOnGround()) {
        ChangeState(S_CROUCH);
    }
}

void Player::UpdateCrouch()
{
    if (stateEnter_) {
        Model::SetAnimFrame(hModel_, 200, 220, 1);
        stateEnter_ = false;
        isCrouching_ = true;
    }

    if (isCrouching_) {
        if (cameraHeight_ > 0.8f)
            cameraHeight_ -= 0.02f;

        GetMoveDirection();

        fMove_ = { ((fMove_.x - playerMovement_.x) * crouchSpeed) , 0.0f , ((fMove_.z - playerMovement_.z) * crouchSpeed) };
        playerMovement_ = { playerMovement_.x + fMove_.x , 0.0f , playerMovement_.z + fMove_.z };
        playerMovement_ = { playerMovement_.x - (playerMovement_.x * 0.01f) , 0.0f , playerMovement_.z - (playerMovement_.z * 0.01f) };
        
        XMVECTOR vMax = XMLoadFloat3(&playerMovement_);
        float max = XMVectorGetX(XMVector3Length(vMax));
        if (max > maxMoveSpeed_) {
            XMVECTOR vMove = XMLoadFloat3(&playerMovement_);
            vMove = XMVector3Normalize(vMove) * maxMoveSpeed_;
            XMStoreFloat3(&playerMovement_, vMove);
        }

        Rotate(playerMovement_.x, playerMovement_.z, 300.0f);

        vMax = XMLoadFloat3(&playerMovement_);
        float a = XMVectorGetX(XMVector3Length(vMax));
        if (a <= 0.04) {
            isCrouching_ = false;
            playerMovement_ = XMFLOAT3(0, 0, 0);
        }
        if (Input::IsKeyUp(DIK_F)) {
            isCrouching_ = false;
        }
        if (Input::IsKeyDown(DIK_SPACE)) {
            Model::SetAnimFrame(hModel_, 0, 0, 1);

            ChangeState(S_IDLE);
        }

    }
    else {
        if (IsMovementKeyPressed()) Model::SetAnimFrame(hModel_, 30, 60, 1);
        else {
            Model::SetAnimFrame(hModel_, 0, 0, 1);
            playerMovement_ = XMFLOAT3(0, 0, 0);
        }

        ChangeState(S_IDLE);
    }
}

void Player::UpdateDead()
{
}

/*--------------------------------------private-----------------------------------------*/

void Player::GetMoveDirection(float value)
{
    fMove_ = { 0,0,0 };
    XMFLOAT3 aimDirection = pAim_->GetAimDirectionY();

    // PlayerクラスのMove関数内の一部
    if (Input::IsKey(DIK_W)) {
        fMove_.x += aimDirection.x;
        fMove_.z += aimDirection.z;
    }
    if (Input::IsKey(DIK_A)) {
        fMove_.x -= aimDirection.z;
        fMove_.z += aimDirection.x;
    }
    if (Input::IsKey(DIK_S)) {
        fMove_.x -= aimDirection.x;
        fMove_.z -= aimDirection.z;
    }
    if (Input::IsKey(DIK_D)) {
        fMove_.x += aimDirection.z;
        fMove_.z -= aimDirection.x;
    }

    XMVECTOR vMove = XMLoadFloat3(&fMove_);
    vMove = XMVector3Normalize(vMove) * value;
    XMStoreFloat3(&fMove_, vMove);

}

void Player::CalcMove()
{
    if (IsPlayerOnGround()) {
        GetMoveDirection(0.1f);
        playerMovement_ = fMove_;

    }
    else {
        GetMoveDirection();
        
        fMove_ = { ((fMove_.x - playerMovement_.x) * airMoveSpeed) , 0.0f , ((fMove_.z - playerMovement_.z) * airMoveSpeed) };
        playerMovement_ = { playerMovement_.x + fMove_.x , 0.0f , playerMovement_.z + fMove_.z };

        XMVECTOR vMax = XMLoadFloat3(&playerMovement_);
        float max = XMVectorGetX(XMVector3Length(vMax));

        if (max > maxMoveSpeed_) {
            XMVECTOR vMove;
            vMove = XMLoadFloat3(&playerMovement_);
            vMove = XMVector3Normalize(vMove);
            vMove *= maxMoveSpeed_;

            XMStoreFloat3(&playerMovement_, vMove);

        }
    }
}

void Player::Rotate(float x, float z, float _rotateSpeed)
{
    float tx = transform_.position_.x + x;
    float tz = transform_.position_.z + z;

    const XMVECTOR vFront{ 0, 0, 1, 0 };
    XMFLOAT3 fAimPos = XMFLOAT3(transform_.position_.x - tx, 0, transform_.position_.z - tz);
    XMVECTOR vAimPos = XMLoadFloat3(&fAimPos);
    vAimPos = XMVector3Normalize(vAimPos);
    XMVECTOR vDot = XMVector3Dot(vFront, vAimPos);
    float dot = XMVectorGetX(vDot);
    float angle = (float)acos(dot);

    // 外積を求めて半回転だったら angle に -1 を掛ける
    XMVECTOR vCross = XMVector3Cross(vFront, vAimPos);
    if (XMVectorGetY(vCross) < 0) {
        angle *= -1;
    }

    // 目標の回転角度を設定
    targetRotation_ = XMConvertToDegrees(angle);

    // 回転角度をスムーズに変更
    float rotationDiff = NormalizeAngle(targetRotation_ - (transform_.rotate_.y - 180.0f));  //Blender
    if (rotationDiff != 0) {
        if (_rotateSpeed > abs(rotationDiff)) {
            transform_.rotate_.y = targetRotation_;
            transform_.rotate_.y += 180.0f; //Blender
        }
        else {
            transform_.rotate_.y += _rotateSpeed * (rotationDiff > 0 ? 1 : -1);
        }
    }
}

float Player::NormalizeAngle(float angle) {
    while (angle > 180.0f) {
        angle -= 360.0f;
    }
    while (angle < -180.0f) {
        angle += 360.0f;
    }
    return angle;
}

void Player::Gravity()
{
    graY_ -= gravity_;

    const float maxGra = -1.0f;
    if (graY_ < maxGra) graY_ = maxGra;

    transform_.position_.y += graY_;

    //空中にいるなら一回目のジャンプは不可に
    firstJump_ = true;

    if (IsPlayerOnGround()) PlayerFallReset();

}

void Player::Jump()
{
    //BulletJump
    if (isCrouching_ == true && !bulletJump_ && (!firstJump_ || !secondJump_)) {

        //flag制御
        if (!firstJump_) firstJump_ = true;
        else secondJump_ = true;
        bulletJump_ = true;

        //XZ,Y軸の移動量計算
        float aimDirectionY = 1.0f + pAim_->GetAimDirectionXY().y;

        XMFLOAT3 aimDirection = pAim_->GetAimDirectionXY();
        XMVECTOR v;

        if (aimDirectionY < 1.0f) {
            if (aimDirectionY <= 0.01f) aimDirectionY = 1.99f * buJumpY;
            else {
                aimDirectionY = 1.0f * buJumpY;

                aimDirection = { aimDirection.x, 0, aimDirection.z };
                v = XMLoadFloat3(&aimDirection);
                v = XMVector3Normalize(v);
                XMStoreFloat3(&aimDirection, v);
            }
        }
        else  aimDirectionY *= buJumpY;

        graY_ = initVy_ * (aimDirectionY - 1.0f);
        graY_ += gravity_;
        transform_.position_.y += gravity_;

        playerMovement_ = { aimDirection.x * buJumpXZ, 0.0f , aimDirection.z * buJumpXZ };

        //maxSpeed
        XMVECTOR vMax = XMLoadFloat3(&playerMovement_);
        float a = XMVectorGetX(XMVector3Length(vMax));
        maxMoveSpeed_ = a;
        if (maxMoveSpeed_ < 0.1f) maxMoveSpeed_ = 0.1f;

        Rotate(playerMovement_.x, playerMovement_.z, 300.0f);

        return;
    }

    //SecondJump
    if (!IsPlayerOnGround() && firstJump_ && !secondJump_) {
        graY_ = initVy_ * 0.8f;
        graY_ += gravity_;
        secondJump_ = true;
        transform_.position_.y += gravity_;

        //maxSpeed
        XMVECTOR vMax = XMLoadFloat3(&playerMovement_);
        float a = XMVectorGetX(XMVector3Length(vMax));
        maxMoveSpeed_ = a;
        if (maxMoveSpeed_ < 0.1f) maxMoveSpeed_ = 0.1f;

        //移動キーを押しているときは値をその方向へmaxMoveSpeedの分入れる
        if (IsMovementKeyPressed()) {
            GetMoveDirection(maxMoveSpeed_);
            playerMovement_ = fMove_;

        }

        return;
    }

    //FirstJump
    if (IsPlayerOnGround()) {
        graY_ = initVy_;
        graY_ += gravity_;
        firstJump_ = true;
        transform_.position_.y += gravity_;

        //maxSpeed
        XMVECTOR vMax = XMLoadFloat3(&playerMovement_);
        float a = XMVectorGetX(XMVector3Length(vMax));
        maxMoveSpeed_ = a;
        if (maxMoveSpeed_ < 0.1f) maxMoveSpeed_ = 0.1f;

        return;

    }
}

void Player::PlayerFallReset()
{
    AudioManager::PlaySoundMa(AUDIO_JUNPING_START);
    pSpeedCtrl_->ResetSpeed();
    firstJump_ = false;
    secondJump_ = false;
    bulletJump_ = false;
    playerMovement_ = { 0.0f , 0.0f , 0.0f };
    transform_.position_.y = pStage_->GetFloorHeight((int)transform_.position_.x, (int)transform_.position_.z);
    graY_ = 0.0f;

}

bool Player::IsPlayerOnGround() {
    if (transform_.position_.y <= pStage_->GetFloorHeight((int)transform_.position_.x, (int)transform_.position_.z))
        return true;

    return false;
}

bool Player::IsMovementKeyPressed()
{
    if (Input::IsKey(DIK_W) || Input::IsKey(DIK_A) || Input::IsKey(DIK_S) || Input::IsKey(DIK_D))
        return true;

    return false;
}

void Player::IsInWall()
{
    //壁との判定  ここ壁だったとき
    int checkX1, checkX2;
    int checkZ1, checkZ2;
    float ground1, ground2;

    float speedUp = pSpeedCtrl_->GetMoveSpeed_() + 0.5f;

    checkX1 = (int)(transform_.position_.x + (0.15f * speedUp)); //前
    checkZ1 = (int)(transform_.position_.z + (0.3f * speedUp));
    checkX2 = (int)(transform_.position_.x - (0.15f * speedUp));
    checkZ2 = (int)(transform_.position_.z + (0.3f * speedUp));

    ground1 = pStage_->GetFloorHeight(checkX1, checkZ1);
    ground2 = pStage_->GetFloorHeight(checkX2, checkZ2);
    if (transform_.position_.y < ground1 || transform_.position_.y < ground2)

        if (pStage_->IsWall(checkX1, checkZ1) == true || pStage_->IsWall(checkX2, checkZ2) == true) { //床やけやったら
            transform_.position_.z = (float)((int)transform_.position_.z) + (1.0f - (0.3f * speedUp));
        }

    checkX1 = (int)(transform_.position_.x + (0.3f * speedUp)); //右
    checkZ1 = (int)(transform_.position_.z + (0.15f * speedUp));
    checkX2 = (int)(transform_.position_.x + (0.3f * speedUp));
    checkZ2 = (int)(transform_.position_.z - (0.15f * speedUp));

    ground1 = pStage_->GetFloorHeight(checkX1, checkZ1);
    ground2 = pStage_->GetFloorHeight(checkX2, checkZ2);
    if (transform_.position_.y < ground1 || transform_.position_.y < ground2)

        if (pStage_->IsWall(checkX1, checkZ1) == true || pStage_->IsWall(checkX2, checkZ2) == true) {
            transform_.position_.x = (float)((int)transform_.position_.x + 1) - (0.3f * speedUp);  // x　だけ戻すことで斜め移動ができるようになる     
        }

    checkX1 = (int)(transform_.position_.x + (0.15f * speedUp)); //後ろ
    checkZ1 = (int)(transform_.position_.z - (0.3f * speedUp));
    checkX2 = (int)(transform_.position_.x - (0.15f * speedUp));
    checkZ2 = (int)(transform_.position_.z - (0.3f * speedUp));

    ground1 = pStage_->GetFloorHeight(checkX1, checkZ1);
    ground2 = pStage_->GetFloorHeight(checkX2, checkZ2);
    if (transform_.position_.y < ground1 || transform_.position_.y < ground2)

        if (pStage_->IsWall(checkX1, checkZ1) == true || pStage_->IsWall(checkX2, checkZ2) == true) {
            transform_.position_.z = (float)((int)transform_.position_.z) + (0.3f * speedUp);
        }

    checkX1 = (int)(transform_.position_.x - (0.3f * speedUp)); //左
    checkZ1 = (int)(transform_.position_.z + (0.15f * speedUp));
    checkX2 = (int)(transform_.position_.x - (0.3f * speedUp));
    checkZ2 = (int)(transform_.position_.z - (0.15f * speedUp));

    ground1 = pStage_->GetFloorHeight(checkX1, checkZ1);
    ground2 = pStage_->GetFloorHeight(checkX2, checkZ2);
    if (transform_.position_.y < ground1 || transform_.position_.y < ground2)

        if (pStage_->IsWall(checkX1, checkZ1) == true || pStage_->IsWall(checkX2, checkZ2) == true) {
            transform_.position_.x = (float)((int)transform_.position_.x) + (0.3f * speedUp);
        }
}