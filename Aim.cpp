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

    //�}�E�X�����ʒu
    Input::SetMousePosition(800 / 2, 600 / 2);

}

void Aim::Update()
{
    //�v���C���[�̈ړ������ɂ���ĉ�]��������
    //�����������[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
    XMVECTOR v = pPlayer_->GetPlaVector();
    //if (aimDirection_.z < 0.0) transform_.rotate_.y += XMVectorGetX(v) * 7;
    //else transform_.rotate_.y -= XMVectorGetX(v) * 7;


    //�}�E�X�ړ���
    XMFLOAT3 mouseMove = Input::GetMouseMove(); //�}�E�X�̈ړ��ʂ��擾

    //�ړ��ʂ����Z
    transform_.rotate_.y += (mouseMove.x * 0.05f) * mouseSensitivity; //�������̉�]
    transform_.rotate_.x -= (mouseMove.y * 0.05f) * mouseSensitivity; //�c�����̉�]

    //�J�����̉�]
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    XMMATRIX matT = XMMatrixTranslation(0, 0, perspectiveDistance_);

    //�J�����̈ʒu�Ɖ�]������
    XMMATRIX mView = mRotX * mRotY * matT; //�J�����p
    XMMATRIX mPlaMove = mRotX * mRotY;     //�v���C���[�̈ړ��p

    //�v���C���[���W�擾
    plaPos_ = pPlayer_->GetPlaPos();

    //�v���C���[�L�����N�^�[�̈ʒu���J�����̈ʒu�Ƃ���
    cameraPos_.x = plaPos_.x;
    cameraPos_.y = plaPos_.y + heightDistance_; //�ڐ�����
    cameraPos_.z = plaPos_.z;

    //�v���C���[�N���X�ɐi�s�����x�N�g��(float3)��`����p   
    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR caDire = XMVector3TransformNormal(forwardVector, mPlaMove); //XMVector3TransformNormal���g�p���邱�Ƃŉ�]�݂̂�K�p���܂�
    XMStoreFloat3(&aimDirection_, -caDire);

    //�J�����œ_
    XMVECTOR caTarget = XMLoadFloat3(&cameraPos_);

    // �J�����|�W�V����
    XMVECTOR camPos = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormal���g�p���邱�Ƃŉ�]�݂̂�K�p���܂�
    camPos = caTarget + (camPos * perspectiveDistance_); // �v���C���[�̔��a���l�����ĉ�]��K�p���܂�

    XMStoreFloat3(&cameraPos_, camPos);
    XMStoreFloat3(&cameraTarget_, caTarget);


#if 1

    //�v���C���[�̈ړ��[���ړ��Ȃ��ɂȂ����ꍇ�̂Ȃ񂽂炪�K�v

    static bool inputKey = false;

    //-----------���񂾂񓮂����
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
