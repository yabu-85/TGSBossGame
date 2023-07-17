#include "Player.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Aim.h"

float NormalizeAngle(float angle);

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_(-1), targetRotation_(0), pAim_(nullptr), jump_(false), graY_(0), fMove_{ 0,0,0 },
    previousPosition_{ 0,0,0 }
{
    moveSpeed_ = 0.75f;
    rotationSpeed_ = 17.0f;
    gravity_ = 0.008f;
    initVy_ = 0.175f;

}

Player::~Player()
{
}

void Player::Initialize()
{
    transform_.scale_.x = 0.2f;
    transform_.scale_.y = 0.2f;
    transform_.scale_.z = 0.2f;

    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Human.fbx");
    assert(hModel_ >= 0);

    pAim_ = Instantiate<Aim>(this);
}

void Player::CalcMoveVec()
{
    fMove_ = { 0,0,0 };
    XMFLOAT3 aimDirection = pAim_->GetAimDirection();

    // Player�N���X��Move�֐����̈ꕔ
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
}

//angle�̒l�𒲐�����֐��i�U��������������߂����Ō����Ă���邽�߂�
float NormalizeAngle(float angle) {
    while (angle > 180.0f) {
        angle -= 360.0f;
    }
    while (angle < -180.0f) {
        angle += 360.0f;
    }
    return angle;
}

void Player::Update()
{
    previousPosition_ = transform_.position_;

    static bool hook = false;
    static XMFLOAT3 targetLocat = { 0,0,0 };
    static int waa = 0;
    static bool anime = false;


    if (!hook) CalcMoveVec();

    //fMove_�̐��K���Ɛ��l����
    XMVECTOR vMove = XMLoadFloat3(&fMove_);
    vMove = XMVector3Normalize(vMove) * 0.1;
    XMStoreFloat3(&fMove_, vMove);

    if (IsPlayerMove()) { // �ړ��L�[�L�[��������Ă���Ό�����ς���

        if (anime == false) { //��񂾂��A�j���[�V�����Ă�
            anime = true;
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

        // �O�ς����߂Ĕ���]�������� angle �� -1 ���|����
        XMVECTOR vCross = XMVector3Cross(vFront, vAimPos_);
        if (XMVectorGetY(vCross) < 0) {
            angle *= -1;
        }

        //�ړ�
        transform_.position_.x += (fMove_.x * moveSpeed_); // �ړ��I
        transform_.position_.z += (fMove_.z * moveSpeed_); // z

        // �ڕW�̉�]�p�x��ݒ�
        targetRotation_ = XMConvertToDegrees(angle);

        // ��]�p�x���X���[�Y�ɕύX
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
    else {
        if (anime == true) { //����[���~�܂�����
            anime = false;
            Model::SetAnimFrame(hModel_, 0, 10, 1);
        }
    }

    //�W�����v�[�[�[�[�[�[�[�[�[�[�[�[�[
    if (Input::IsKey(DIK_SPACE) && IsPlayerOnGround() && !jump_) {
        graY_ = initVy_;
        graY_ += gravity_;
        jump_ = true;
        transform_.position_.y += gravity_;
    }

    //�[�[�[�[�[�[�[�[�[�[�[�[�[�d�͗���
    if (!IsPlayerOnGround() && !hook) {
        transform_.position_.y += graY_;
        graY_ -= gravity_;

        if (IsPlayerOnGround()) {
            jump_ = false;
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

