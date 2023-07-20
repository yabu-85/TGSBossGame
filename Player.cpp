#include "Player.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Aim.h"

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_(-1), targetRotation_(0), pAim_(nullptr), firstJump_(false), secondJump_(false),
    graY_(0), fMove_{ 0,0,0 }, previousPosition_{ 0,0,0 }, state_(S_IDLE), anime_(false)
{
    moveSpeed_ = 0.75f;
    rotationSpeed_ = 13.0f;
    gravity_ = 0.0075f;
    initVy_ = 0.17f;

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
}

Player::~Player()
{
}

void Player::UpdateIdle()
{
    if (IsPlayerMove()) state_ = S_MOVE;
    if (Input::IsKeyDown(DIK_SPACE)) state_ = S_JUMP;

}

void Player::UpdateMove()
{
    if (anime_ == false) { //一回だけアニメーション呼ぶ
        anime_ = true;
        Model::SetAnimFrame(hModel_, 20, 100, 1);
    }

    float tx = transform_.position_.x + fMove_.x;
    float tz = transform_.position_.z + fMove_.z;

    XMVECTOR vFront{ 0, 0, 1, 0 };
    XMFLOAT3 fAimPos = XMFLOAT3(transform_.position_.x - tx, 0, transform_.position_.z - tz);
    XMVECTOR vAimPos_ = XMLoadFloat3(&fAimPos);
    vAimPos_ = XMVector3Normalize(vAimPos_);
    XMVECTOR vDot = XMVector3Dot(vFront, vAimPos_);
    float dot = XMVectorGetX(vDot);
    float angle = acos(dot);

    // 外積を求めて半回転だったら angle に -1 を掛ける
    XMVECTOR vCross = XMVector3Cross(vFront, vAimPos_);
    if (XMVectorGetY(vCross) < 0) {
        angle *= -1;
    }

    //移動
    transform_.position_.x += (fMove_.x * moveSpeed_); // 移動！
    transform_.position_.z += (fMove_.z * moveSpeed_); // z

    // 目標の回転角度を設定
    targetRotation_ = XMConvertToDegrees(angle);

    // 回転角度をスムーズに変更
    float rotationDiff = NormalizeAngle(targetRotation_ - transform_.rotate_.y);
    if (rotationSpeed_ > abs(rotationDiff)) {
        transform_.rotate_.y = targetRotation_;
    }
    else {
        transform_.rotate_.y += rotationSpeed_ * (rotationDiff > 0 ? 1 : -1);
    }

    if (!IsPlayerMove()) {
        anime_ = false;
        Model::SetAnimFrame(hModel_, 0, 10, 1);
        state_ = S_IDLE;

    }
    if (Input::IsKeyDown(DIK_SPACE)) state_ = S_JUMP;
}

void Player::UpdateJump()
{
    if (!IsPlayerOnGround() && firstJump_ && !secondJump_) {
        graY_ = initVy_ * 0.8;
        graY_ += gravity_;
        secondJump_ = true;
        transform_.position_.y += gravity_;
        state_ = S_IDLE;

    }

    if (IsPlayerOnGround() && !firstJump_) {
        graY_ = initVy_;
        graY_ += gravity_;
        firstJump_ = true;
        transform_.position_.y += gravity_;

    }

    state_ = S_IDLE;
}

void Player::UpdateDead()
{
}

void Player::Update()
{
    switch (state_) {
    case STATE::S_IDLE:
        UpdateIdle();
        break;
    case STATE::S_MOVE:
        UpdateMove();
        break;
    case STATE::S_JUMP:
        UpdateJump();
        break;
    case STATE::S_DEAD:
        UpdateDead();
        break;
    }

    previousPosition_ = transform_.position_;
    CalcMoveVec();

    //ーーーー重力落下ーーーーーー
    if (!IsPlayerOnGround()) {
        transform_.position_.y += graY_;
        graY_ -= gravity_;
        firstJump_ = true;

        if (IsPlayerOnGround()) {
            firstJump_ = false;
            secondJump_ = false;
            transform_.position_.y = 0.0f;
            graY_ = 0.0f;
        }
    }
}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Player::Release()
{
}

bool Player::IsPlayerOnGround() {
    if (transform_.position_.y <= 0.0f)
        return true;

    return false;
}

bool Player::IsPlayerMove() {
    if (fMove_.x != 0 || fMove_.z)
        return true;

    return false;
}

XMVECTOR Player::GetPlaVector() {
    XMVECTOR v = previousPosition_ - transform_.position_;

    return v;
}

/*--------------------------------------private-----------------------------------------*/

void Player::CalcMoveVec()
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
    vMove = XMVector3Normalize(vMove) * 0.1;
    XMStoreFloat3(&fMove_, vMove);
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
