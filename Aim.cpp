#include "Aim.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Player.h"
#include "Engine/Image.h"

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPos_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 }, plaPos_{ 0,0,0 }, pPlayer_(nullptr),
    hPict_(-1)
{
    mouseSensitivity = 2.5f;
    perspectiveDistance_ = 3.2f;
    heightDistance_ = 1.5f;
    cameraSpeed_ = 0.4f;
}

Aim::~Aim()
{
}

void Aim::Initialize()
{
    pPlayer_ = (Player*)FindObject("Player");

    //�}�E�X�����ʒu
    Input::SetMousePosition(800 / 2, 600 / 2);

    //�摜�f�[�^�̃��[�h
    hPict_ = Image::Load("cross.png");
    assert(hPict_ >= 0);
}

void Aim::Update()
{
    //�}�E�X�ړ���
    XMFLOAT3 mouseMove = Input::GetMouseMove(); //�}�E�X�̈ړ��ʂ��擾

    //�ړ��ʂ��v�Z
    transform_.rotate_.y += (mouseMove.x * 0.05f) * mouseSensitivity; //�������̉�]
    transform_.rotate_.x -= (mouseMove.y * 0.05f) * mouseSensitivity; //�c�����̉�]
    if (transform_.rotate_.x <= -89.0f) transform_.rotate_.x = -89.0f;
    if (transform_.rotate_.x >= 89.0f) transform_.rotate_.x = 89.0f;

    //�J�����̉�]
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    XMMATRIX matT = XMMatrixTranslation(0, 0, 0);

    //�J�����̈ʒu�Ɖ�]������
    XMMATRIX mView = mRotX * mRotY; //�J�����p
    XMMATRIX mPlaMove = mRotY;     //�v���C���[�̈ړ��p   

    //�v���C���[�N���X�ɐi�s�����x�N�g��(float3)��`����p
    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR caDire = XMVector3TransformNormal(forwardVector, mPlaMove); //XMVector3TransformNormal���g�p���邱�Ƃŉ�]�݂̂�K�p���܂�
    XMVector3Normalize(caDire);
    XMStoreFloat3(&aimDirection_, -caDire);

    //�v���C���[�̈ʒu���J�����̈ʒu�Ƃ���
    plaPos_ = pPlayer_->GetPlaPos();
    cameraPos_.x = plaPos_.x + aimDirection_.z;
    cameraPos_.y = plaPos_.y + heightDistance_; //�ڐ�����
    cameraPos_.z = plaPos_.z - aimDirection_.x;

    //�J�����œ_
    XMVECTOR caTarget = XMLoadFloat3(&cameraPos_);

    // �J�����|�W�V����
    XMVECTOR camPos = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormal���g�p���邱�Ƃŉ�]�݂̂�K�p���܂�
    camPos = caTarget + (camPos * perspectiveDistance_); // �v���C���[�̔��a���l�����ĉ�]��K�p���܂�

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
    Image::SetTransform(hPict_, cross);
    Image::Draw(hPict_);
}

void Aim::Release()
{
}
