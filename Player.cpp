#include "Player.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Aim.h"

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_(-1), targetRotation_(0), firstJump_(false), secondJump_(false), isCrouching_(false),
    graY_(0), fMove_{ 0,0,0 }, previousPosition_{ 0,0,0 }, state_(S_IDLE), anime_(false), pAim_(nullptr), cameraHeight_(1.0f),
    playerMovement_{0,0,0}, movementRatio_(0.0f), pText_(nullptr), bulletJump_(false)
{
    moveSpeed_ = 0.75f;
    rotationSpeed_ = 13.0f;
    gravity_ = 0.0075f;
    initVy_ = 0.17f;
}

Player::~Player()
{
}

void Player::Initialize()
{
    transform_.scale_.x = 0.2f;
    transform_.scale_.y = 0.2f;
    transform_.scale_.z = 0.2f;

    //モデルデータのロード
    hModel_ = Model::Load("Human.fbx");
    assert(hModel_ >= 0);

    pAim_ = Instantiate<Aim>(this);

    pText_ = new Text;
    pText_->Initialize();
}

void Player::Update()
{
    previousPosition_ = transform_.position_;

    switch (state_) {
    case STATE::S_IDLE:
        UpdateIdle();
        break;
    case STATE::S_MOVE:
        UpdateMove();
        break;
    case STATE::S_DEAD:
        UpdateDead();
        break;
    }

    if(state_ != S_MOVE)
        CalcMoveRatio(false);

    transform_.position_.x += (playerMovement_.x * moveSpeed_) * movementRatio_; // 移動！
    transform_.position_.z += (playerMovement_.z * moveSpeed_) * movementRatio_; // z

    //移動するなら向きを変える
    if (IsPlayerMove()) GradualRotate();

    //重力
    if (!IsPlayerOnGround()) Gravity();

    //しゃがみ
    Crouch();

    //jump
    if (Input::IsKeyDown(DIK_SPACE)) Jump();

    //攻撃
    //フルオートはKey、単発||近接はKeyDown
    if (Input::IsMouseButton(0)) {
        transform_.rotate_.y += 1.0f;
    }

    //覗き込み||近接ガード||slow（落下速度低下）
    if (Input::IsMouseButton(1)) {
        transform_.rotate_.y -= 1.0f;
    }

    //デバック用こまんど
    if (Input::IsKey(DIK_UPARROW)) {
        transform_.position_.y += 1.0f;
    }
}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

    pText_->Draw(30, 30, (int)(movementRatio_ * 100.0f));
    pText_->Draw(30, 70, (int)(pAim_->GetAimDirection().y * 100.0f));

}

void Player::Release()
{
}

XMVECTOR Player::GetPlaVector() {
    XMVECTOR v = previousPosition_ - transform_.position_;

    return v;

}

/*--------------------------------State------------------------*/

void Player::UpdateIdle()
{


    //--------state----------
    if (IsMovementKeyPressed()) {
        if (anime_ == false) {
            anime_ = true;
            Model::SetAnimFrame(hModel_, 20, 100, 1);
        }
        state_ = S_MOVE;
    }

}

void Player::UpdateMove()
{
    CalcMoveRatio(true);
    CalcMove();


    //--------state----------
    if (!IsMovementKeyPressed()) {
        anime_ = false;
        Model::SetAnimFrame(hModel_, 0, 10, 1);
        state_ = S_IDLE;
    }

}

void Player::UpdateDead()
{
}

/*--------------------------------------private-----------------------------------------*/

void Player::CalcMove()
{
    fMove_ = { 0,0,0 };
    XMFLOAT3 aimDirection = pAim_->GetAimDirection();

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

    //fMove_の正規化と数値調整
    XMVECTOR vMove = XMLoadFloat3(&fMove_);
    vMove = XMVector3Normalize(vMove) * 0.1f;
    XMStoreFloat3(&fMove_, vMove);

    if (IsPlayerOnGround()) {
        playerMovement_ = fMove_;

    }
    else if (fMove_.x != 0.0f || fMove_.z != 0.0f) {
        //空中での移動、今のMoveから徐々に目標へ値を近づけている
        fMove_ = { ((fMove_.x - playerMovement_.x) * 0.04f) , 0.0f , ((fMove_.z - playerMovement_.z) * 0.04f ) };
        playerMovement_ = { playerMovement_.x + fMove_.x , 0.0f , playerMovement_.z + fMove_.z};

        if (abs(playerMovement_.x) + abs(playerMovement_.z) >= 1.0f) {
            vMove = XMLoadFloat3(&playerMovement_);
            XMVector3Normalize(vMove);
            XMStoreFloat3(&playerMovement_, vMove);
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

void Player::InstantRotate() {
    float tx = transform_.position_.x + playerMovement_.x;
    float tz = transform_.position_.z + playerMovement_.z;

    const XMVECTOR vFront{ 0, 0, 1, 0 };
    XMFLOAT3 fAimPos = XMFLOAT3(transform_.position_.x - tx, 0, transform_.position_.z - tz);
    XMVECTOR vAimPos_ = XMLoadFloat3(&fAimPos);
    vAimPos_ = XMVector3Normalize(vAimPos_);
    XMVECTOR vDot = XMVector3Dot(vFront, vAimPos_);
    float dot = XMVectorGetX(vDot);
    float angle = (float)acos(dot);

    // 外積を求めて半回転だったら angle に -1 を掛ける
    XMVECTOR vCross = XMVector3Cross(vFront, vAimPos_);
    if (XMVectorGetY(vCross) < 0) {
        angle *= -1;
    }

    transform_.rotate_.y = XMConvertToDegrees(angle);

}

void Player::GradualRotate()
{
    float tx = transform_.position_.x + playerMovement_.x;
    float tz = transform_.position_.z + playerMovement_.z;

    const XMVECTOR vFront{ 0, 0, 1, 0 };
    XMFLOAT3 fAimPos = XMFLOAT3(transform_.position_.x - tx, 0, transform_.position_.z - tz);
    XMVECTOR vAimPos_ = XMLoadFloat3(&fAimPos);
    vAimPos_ = XMVector3Normalize(vAimPos_);
    XMVECTOR vDot = XMVector3Dot(vFront, vAimPos_);
    float dot = XMVectorGetX(vDot);
    float angle = (float)acos(dot);

    // 外積を求めて半回転だったら angle に -1 を掛ける
    XMVECTOR vCross = XMVector3Cross(vFront, vAimPos_);
    if (XMVectorGetY(vCross) < 0) {
        angle *= -1;
    }

    // 目標の回転角度を設定
    targetRotation_ = XMConvertToDegrees(angle);

    // 回転角度をスムーズに変更
    float rotationDiff = NormalizeAngle(targetRotation_ - transform_.rotate_.y);
    if (rotationDiff != 0) {
        if (rotationSpeed_ > abs(rotationDiff)) {
            transform_.rotate_.y = targetRotation_;
        }
        else {
            transform_.rotate_.y += rotationSpeed_ * (rotationDiff > 0 ? 1 : -1);
        }
    }
}

void Player::Gravity()
{
    transform_.position_.y += graY_;
    graY_ -= gravity_;
    if (graY_ < -1.0f) graY_ = -1.0f;
    
    firstJump_ = true;

    if (IsPlayerOnGround()) {
        firstJump_ = false;
        secondJump_ = false;
        bulletJump_ = false;
        transform_.position_.y = 0.0f;
        graY_ = 0.0f;
    }
}

void Player::Crouch()
{
    if (Input::IsKey(DIK_F)) {
        transform_.scale_.y = 0.12f;
        isCrouching_ = true;

        if (cameraHeight_ > 0.8f)
            cameraHeight_ -= 0.02f;

    }
    else {
        transform_.scale_.y = 0.2f;
        isCrouching_ = false;

        if (cameraHeight_ < 1.0f)
            cameraHeight_ += 0.02f;

    }
}

void Player::Jump()
{
    //BulletJump
    if (isCrouching_ == true && !bulletJump_ && (!firstJump_ || !secondJump_)) {
        if (!firstJump_) firstJump_ = true;
        else secondJump_ = true;
        bulletJump_ = true;

        const float bulletJump = 1.8f;
        float aimDirectionY = 1.0f + pAim_->GetAimDirection().y;

        XMFLOAT3 aimDirection = pAim_->GetAimDirection();
        XMVECTOR v;

        if (aimDirectionY < 1.0f) {
            if (aimDirectionY <= 0.01f) aimDirectionY = 1.99f * bulletJump;
            else {
                aimDirectionY = 1.0f * bulletJump;

                aimDirection = { aimDirection.x, 0, aimDirection.z };
                v = XMLoadFloat3(&aimDirection);
                v = XMVector3Normalize(v);
                XMStoreFloat3(&aimDirection, v);
            }
        }
        else  aimDirectionY *= bulletJump;
        
        graY_ = initVy_ * (aimDirectionY - 1.0f);
        graY_ += gravity_;
        transform_.position_.y += gravity_;

        playerMovement_ = { aimDirection.x * 0.6f , 0.0f , aimDirection.z * 0.6f };
        movementRatio_ = 1.0f;

        InstantRotate();

        return;
    }

    //SecondJump
    if (!IsPlayerOnGround() && firstJump_ && !secondJump_) {
        graY_ = initVy_ * 0.8f;
        graY_ += gravity_;
        secondJump_ = true;
        transform_.position_.y += gravity_;
        return;
    }
    
    //FirstJump
    if (IsPlayerOnGround()) {
        graY_ = initVy_;
        graY_ += gravity_;
        firstJump_ = true;
        transform_.position_.y += gravity_;
        return;

    }

}

void Player::CalcMoveRatio(bool type)
{
    //加速
    if (type == true) {
        if (IsPlayerOnGround()) movementRatio_ += 0.075f;
        else movementRatio_ += 0.05f;
        if (movementRatio_ > 1.0f) movementRatio_ = 1.0f;
        return;
    }
    //減衰
    else {
        if (IsPlayerOnGround()) movementRatio_ -= 0.15f;
        else movementRatio_ -= 0.008f;
        if (movementRatio_ < 0.0f) movementRatio_ = 0.0f;
        return;
    }
}

bool Player::IsPlayerOnGround() {
    if (transform_.position_.y <= 0.0f)
        return true;

    return false;
}

bool Player::IsMovementKeyPressed()
{
    if (Input::IsKey(DIK_W) || Input::IsKey(DIK_A) || Input::IsKey(DIK_S) || Input::IsKey(DIK_D))
        return true;

    return false;
}

bool Player::IsPlayerMove() {
    if (graY_ != 0.0f || playerMovement_.x != 0.0f || playerMovement_.z != 0.0f)
        return true;

    return false;
}
