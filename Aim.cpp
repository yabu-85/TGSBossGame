#include "Aim.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Player.h"
#include "Engine/Image.h"
#include "Engine/Global.h"
#include "Stage.h"
#include <map>
#include "Engine/Direct3D.h"

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirectionXY_{ 0,0,0 }, aimDirectionY_{ 0,0,0 },
    plaPos_{ 0,0,0 }, pPlayer_(nullptr), hPict_(-1), aimDraw_(true), aimMove_(false), isShaking_(false), shakeTimer_(0),
    shakeAmplitude_(1.0f), shakeStrength_(0.0f), pStage_(nullptr), shakeTimerSub_(0)
{
    mouseSensitivity = 2.5f;
    perspectiveDistance_ = 3.2f;
    heightDistance_ = 1.5f;
    cross_.scale_ = { 0.5f, 0.5f, 0.5f };

    transform_.rotate_.y = -180.0f;
}

Aim::~Aim()
{
}

void Aim::Initialize()
{
    pStage_ = (Stage*)FindObject("Stage");

    //�摜�f�[�^�̃��[�h
    hPict_ = Image::Load("Png/cross.png");
    assert(hPict_ >= 0);

}

void Aim::Update()
{
    if (pPlayer_ == nullptr) {
        pPlayer_ = (Player*)FindObject("Player");
        return;
    }

    //�}�E�X�ړ���
    if (aimMove_) {
        XMFLOAT3 mouseMove = Input::GetMouseMove(); //�}�E�X�̈ړ��ʂ��擾

        //�ړ��ʂ��v�Z
        transform_.rotate_.y += (mouseMove.x * 0.05f) * mouseSensitivity; //�������̉�]
        transform_.rotate_.x -= (mouseMove.y * 0.05f) * mouseSensitivity; //�c�����̉�]
        if (transform_.rotate_.x <= -89.0f) transform_.rotate_.x = -89.0f;
        if (transform_.rotate_.x >= 89.0f) transform_.rotate_.x = 89.0f;

    //    if (transform_.rotate_.y <= -270.0f) transform_.rotate_.y = -270.0f;
    //    if (transform_.rotate_.y >= -90.0f) transform_.rotate_.y = -90.0f;
    }

    //�J�����̉�]
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

    //�J�����̈ʒu�Ɖ�]������
    XMMATRIX mView = mRotX * mRotY; //�J�����p
    XMMATRIX mPlaMove = mRotY;     //�v���C���[�̈ړ��p   

    //�v���C���[�N���X�ɐi�s�����x�N�g��(float3)��`����p
    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR caDire = XMVector3TransformNormal(forwardVector, mPlaMove); //XMVector3TransformNormal���g�p���邱�Ƃŉ�]�݂̂�K�p���܂�
    XMVector3Normalize(caDire);
    XMStoreFloat3(&aimDirectionY_, -caDire);

    //�v���C���[�̈ʒu���J�����̈ʒu�Ƃ���
    plaPos_ = pPlayer_->GetPosition();
    cameraPos_.x = plaPos_.x + (aimDirectionY_.z * 0.5f);
    cameraPos_.y = plaPos_.y + heightDistance_ * pPlayer_->GetCameraHeight(); //�ڐ�����
    cameraPos_.z = plaPos_.z - (aimDirectionY_.x * 0.5f);

    //�J�����œ_
    XMVECTOR caTarget = XMLoadFloat3(&cameraPos_);

    // �J�����|�W�V����
    XMVECTOR camPos = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormal���g�p���邱�Ƃŉ�]�݂̂�K�p���܂�
    XMVector3Normalize(camPos);
    XMStoreFloat3(&aimDirectionXY_, -camPos);

    if (isShaking_) {
        float shakeStr = shakeStrength_ / (float)shakeTimerSub_;
        shakeAmplitude_ -= shakeStr;
        shakeTimer_--;

        //�V�F�C�N���I�������猳�̈ʒu�ɖ߂��A�j���[�V�������J�n
        if (shakeTimer_ <= 0) {
            shakeAmplitude_ += shakeStr * 2;

            //�����
            if (shakeAmplitude_ >= 1.0f) {
                isShaking_ = false;
                shakeAmplitude_ = 1.0f;
            }
        }
    }

    //�v���C���[�̔��a���l�����ĉ�]��K�p���Ă���
    //����Aim�̋߂��̒l���v���C���[����擾���Čv�Z�����Ă�
    camPos = caTarget + (camPos * (perspectiveDistance_ * shakeAmplitude_));

    XMStoreFloat3(&cameraPos_, camPos);
    XMStoreFloat3(&cameraTarget_, caTarget);
    //if (cameraPos_.y <= 0.0f) cameraPos_.y = 0.0f;

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