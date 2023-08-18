#include "Player.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Aim.h"
#include "Stage.h"
#include <thread>
#include <chrono>
#define SAFE_DELETE(p) if(p != nullptr){ p = nullptr; delete p;}

static const std::vector <std::pair<int, int>> atkTime = { {320 , 280}, { 365 , 320 }, { 395 , 365 } };
static const std::vector <int> nextAttackTime = { 10, 10, 10 , 10 }; //�Ō�̂�1�R���{�ځi�ŏ��̐����j
static int animeTime = 0;
static int combo = 0;
static bool nextCombo = false;

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_(-1), targetRotation_(0), firstJump_(false), secondJump_(false), isCrouching_(false),
    graY_(0), fMove_{ 0,0,0 }, previousPosition_{ 0,0,0 }, state_(S_IDLE), anime_(false), pAim_(nullptr), cameraHeight_(1.0f),
    playerMovement_{ 0,0,0 }, pText_(nullptr), bulletJump_(false), decelerationTime_(0.0f), isDecelerated_(false),
    isDecelerating_(false), pStage_(nullptr), maxMoveSpeed_(1.0f), isActive_(false)
{
    moveSpeed_ = 0.75f;
    rotationSpeed_ = 13.0f;
    gravity_ = 0.0075f;
    initVy_ = 0.17f;
}

Player::~Player()
{
    SAFE_DELETE(pAim_);
    SAFE_DELETE(pText_);
    SAFE_DELETE(pStage_);
}

void Player::Initialize()
{
    transform_.scale_.x = 0.2f;
    transform_.scale_.y = 0.2f;
    transform_.scale_.z = 0.2f;

    transform_.rotate_.y = 180.0f;  //Blender

    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("huu.fbx");
    assert(hModel_ >= 0);

    pAim_ = Instantiate<Aim>(this);
    pStage_ = (Stage*)FindObject("Stage");

    pText_ = new Text;
    pText_->Initialize();
}

void Player::Update()
{
    if (!isActive_) return;

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

    transform_.position_.x += (playerMovement_.x * moveSpeed_); // �ړ��I
    transform_.position_.z += (playerMovement_.z * moveSpeed_); // z

    //�ړ�����Ȃ������ς���
    if (IsMovementKeyPressed()) GradualRotate(playerMovement_.x, playerMovement_.z);

    //�d��
    if (!IsPlayerOnGround()) Gravity();

    //���Ⴊ��
    Crouch();

    if(IsPlayerMove())
    IsInWall();

    //jump
    if (Input::IsKeyDown(DIK_SPACE)) Jump();

    static XMFLOAT3 aimDirection2 = { 0,0,0 };
    //�t���I�[�g��Key�A�P��||�ߐ�
    if (Input::IsMouseButtonDown(0)) nextCombo = true;

    if (nextCombo && animeTime <= nextAttackTime[combo]) {
        if (combo >= 3) combo = 0;
        combo++;
        nextCombo = false;

        aimDirection2 = pAim_->GetAimDirectionY();
        InstantRotate(aimDirection2.x, aimDirection2.z);

        Model::SetAnimFrame(hModel_, atkTime[combo - 1].second, atkTime[combo - 1].first, 1);
        animeTime = atkTime[combo - 1].first - atkTime[combo - 1].second;
    }

    if (animeTime > 0) {
        animeTime--;

        //�����e�R���{�̏���
        if (combo == 1){
            if (animeTime >= 10) {
                fMove_ = { 0,0,0 };
                fMove_.x += aimDirection2.x;
                fMove_.z += aimDirection2.z;
                
                XMVECTOR vMove = XMLoadFloat3(&fMove_);
                XMStoreFloat3(&fMove_, vMove * 0.05f);
                transform_.position_.x += fMove_.x;
                transform_.position_.z += fMove_.z;
                
                if (animeTime >= 20) {
                    vMove = XMVector3Normalize(vMove);
                    XMStoreFloat3(&fMove_, vMove * 0.025f);
                    transform_.position_.x += fMove_.x;
                    transform_.position_.z += fMove_.z;

                }
            }
        } 
        else if (combo == 2) {
            if (animeTime >= 10) {
                fMove_ = { 0,0,0 };
                fMove_.x += aimDirection2.x;
                fMove_.z += aimDirection2.z;

                XMVECTOR vMove = XMLoadFloat3(&fMove_);
                XMStoreFloat3(&fMove_, vMove * 0.05f);
                transform_.position_.x += fMove_.x;
                transform_.position_.z += fMove_.z;

                if (animeTime >= 20) {
                    vMove = XMVector3Normalize(vMove);
                    XMStoreFloat3(&fMove_, vMove * 0.025f);
                    transform_.position_.x += fMove_.x;
                    transform_.position_.z += fMove_.z;

                }
            }
        }

        if (!nextCombo && animeTime <= 0) {
            Model::SetAnimFrame(hModel_, 0, 0, 1);
            combo = 0;
        }
    }

    //�`������||�ߐڃK�[�h||slow�i�������x�ቺ�j
    //�������͍ŏ��ɉ�������̂��
    if (Input::IsMouseButtonDown(1) && !IsPlayerOnGround()) {
        isDecelerating_ = true;
        pAim_->TriggerCameraShake(10, 0.1f);

        if (!isDecelerated_) {
            //��ֈړ����Ă���ꍇ��0��
            if (graY_ > 0.0f)graY_ = -gravity_ * 0.5f;
            else graY_ *= 0.2f;

            decelerationTime_ = 0.0f;
            isDecelerated_ = true;
        }
        else {
            graY_ = graY_ * decelerationTime_;

        }
    }

    if(Input::IsMouseButtonUp(1)) {
        isDecelerating_ = false;
    }

    //�������Ă���
    if (isDecelerating_ ) {
        const float noDe = 0.8f; //��������߂鎞��
        const float deTime = 0.0055f; //���Ԃ̑���

        decelerationTime_ += deTime;

        //�������ԏI�������̏���
        if (decelerationTime_ > noDe) isDecelerating_ = false;
    }

    //�f�o�b�N�p���܂��
    if (Input::IsKey(DIK_UPARROW)) transform_.position_.y += 1.0f;
}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

    XMVECTOR v = XMLoadFloat3(&playerMovement_);
    pText_->Draw(30, 70, (int)( Length(v) * 100.0f ) );
    pText_->Draw(30, 110, (int)( (maxMoveSpeed_ * 100.0f)));

    pText_->Draw(30, 190, (int)(pStage_->GetFloorHeight((int)transform_.position_.x, (int)transform_.position_.z) ));
    pText_->Draw(30, 250, (int)(transform_.position_.x));
    pText_->Draw(30, 290, (int)(transform_.position_.y));
    pText_->Draw(30, 340, (int)(transform_.position_.z));

    pText_->Draw(30, 400, (int)((decelerationTime_ * 100.0f)));


    pText_->Draw(30, 470, combo);
    pText_->Draw(30, 510, animeTime);
    if(nextCombo) pText_->Draw(30, 550, "Yes");

}

void Player::Release()
{
}

void Player::SetActiveWithDelay(bool isActive)
{
    //1�b��Ɏ��ۂ̃A�N�e�B�u��Ԃ�ݒ肷��^�C�}�[���Z�b�g�A�b�v
    std::thread([this, isActive]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        isActive_ = isActive;
    }).detach();    
}

XMVECTOR Player::GetPlaVector() {
    XMVECTOR v = previousPosition_ - transform_.position_;

    return v;

}

float Player::IsAiming()
{
    //�������J�����ʒu�߂��Ȃ�͉̂���������������Ă���Ƃ��̂�
    //�����獡�͖�����
    if (false) {
        if (IsPlayerOnGround()) {
            if (Input::IsMouseButton(1)) return 0.4f;
        }
        else if (isDecelerating_) return 0.75f;

    }   

    return 1.0f;
}

/*--------------------------------State------------------------*/

void Player::UpdateIdle()
{
    if (IsMovementKeyPressed()) {
        if (anime_ == false) {
            anime_ = true;
            Model::SetAnimFrame(hModel_, 30, 60, 1);

        }
        state_ = S_MOVE;
    }
}

void Player::UpdateMove()
{
    CalcMove();

    if (!IsMovementKeyPressed()) {
        anime_ = false;
        Model::SetAnimFrame(hModel_, 0, 0, 1);
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
    XMFLOAT3 aimDirection = pAim_->GetAimDirectionY();

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

    XMVECTOR vMove = XMLoadFloat3(&fMove_);
    vMove = XMVector3Normalize(vMove);
    
    if (IsPlayerOnGround()) {
        XMStoreFloat3(&fMove_, vMove * 0.1f);
        playerMovement_ = fMove_;

    }
    else  {
        XMStoreFloat3(&fMove_, vMove);

        static const float airMoveSpeed = 0.002f;

        //fMove_
        fMove_ = { ((fMove_.x - playerMovement_.x) * airMoveSpeed) , 0.0f , ((fMove_.z - playerMovement_.z) * airMoveSpeed) };
        playerMovement_ = { playerMovement_.x + fMove_.x , 0.0f , playerMovement_.z + fMove_.z};

        XMVECTOR vMax = XMLoadFloat3(&playerMovement_);
        float max = XMVectorGetX(XMVector3Length(vMax));

        if (max > maxMoveSpeed_) {
            vMove = XMLoadFloat3(&playerMovement_);
            vMove = XMVector3Normalize(vMove);
            vMove *= maxMoveSpeed_;

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

void Player::InstantRotate(float x, float z) {
    float tx = transform_.position_.x + x;
    float tz = transform_.position_.z + z;

    const XMVECTOR vFront{ 0, 0, 1, 0 };
    XMFLOAT3 fAimPos = XMFLOAT3(transform_.position_.x - tx, 0, transform_.position_.z - tz);
    XMVECTOR vAimPos = XMLoadFloat3(&fAimPos);
    vAimPos = XMVector3Normalize(vAimPos);
    XMVECTOR vDot = XMVector3Dot(vFront, vAimPos);
    float dot = XMVectorGetX(vDot);
    float angle = (float)acos(dot);

    // �O�ς����߂Ĕ���]�������� angle �� -1 ���|����
    XMVECTOR vCross = XMVector3Cross(vFront, vAimPos);
    if (XMVectorGetY(vCross) < 0) {
        angle *= -1;
    }

    transform_.rotate_.y = XMConvertToDegrees(angle);
    transform_.rotate_.y += 180.0f; //Blender

}

void Player::GradualRotate(float x, float z)
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

    // �O�ς����߂Ĕ���]�������� angle �� -1 ���|����
    XMVECTOR vCross = XMVector3Cross(vFront, vAimPos);
    if (XMVectorGetY(vCross) < 0) {
        angle *= -1;
    }

    // �ڕW�̉�]�p�x��ݒ�
    targetRotation_ = XMConvertToDegrees(angle);

    // ��]�p�x���X���[�Y�ɕύX
    float rotationDiff = NormalizeAngle(targetRotation_ - (transform_.rotate_.y -180.0f) );  //Blender
    if (rotationDiff != 0) {
        if (rotationSpeed_ > abs(rotationDiff)) {
            transform_.rotate_.y = targetRotation_;
            transform_.rotate_.y += 180.0f; //Blender
        }
        else {
            transform_.rotate_.y += rotationSpeed_ * (rotationDiff > 0 ? 1 : -1);
        }
    }
}

void Player::Gravity()
{

    if (isDecelerating_) graY_ -= (decelerationTime_ * 0.2f * gravity_);
    else graY_ -= gravity_;

    const float maxGra = -1.0f;
    if (graY_ < maxGra) graY_ = maxGra;

    transform_.position_.y += graY_;


    //�󒆂ɂ���Ȃ���ڂ̃W�����v�͕s��
    firstJump_ = true;

    if (IsPlayerOnGround()) {
        firstJump_ = false;
        secondJump_ = false;
        bulletJump_ = false;
        isDecelerated_ = false;
        playerMovement_ = { 0.0f , 0.0f , 0.0f};
        transform_.position_.y = pStage_->GetFloorHeight((int)transform_.position_.x, (int)transform_.position_.z);
        graY_ = 0.0f;

        return;
    }

}

void Player::Crouch()
{
    if (Input::IsKey(DIK_F)) {
        isCrouching_ = true;
        
        if (Input::IsKeyDown(DIK_F))
        Model::SetAnimFrame(hModel_, 120, 160, 1);

        if (cameraHeight_ > 0.8f)
            cameraHeight_ -= 0.02f;

    }
    else {
        isCrouching_ = false;

        if(Input::IsKeyUp(DIK_F))
        Model::SetAnimFrame(hModel_, 0, 0, 1);

        if (cameraHeight_ < 1.0f)
            cameraHeight_ += 0.02f;

    }
}

void Player::Jump()
{
    isDecelerating_ = false;

    //BulletJump
    if (isCrouching_ == true && !bulletJump_ && (!firstJump_ || !secondJump_)) {

        //flag����
        if (!firstJump_) firstJump_ = true;
        else secondJump_ = true;
        bulletJump_ = true;

        //XZ,Y���̈ړ��ʌv�Z
        const float buJumpY = 1.65f;
        const float buJumpXZ = 0.3f;
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

        playerMovement_ = { aimDirection.x * buJumpXZ, 0.0f , aimDirection.z * buJumpXZ};

        //maxSpeed
        XMVECTOR vMax = XMLoadFloat3(&playerMovement_);
        float a = XMVectorGetX(XMVector3Length(vMax));
        maxMoveSpeed_ = a;
        if (maxMoveSpeed_ < 0.1f) maxMoveSpeed_ = 0.1f;

        InstantRotate(playerMovement_.x, playerMovement_.z);

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

bool Player::IsPlayerOnGround() {
    
    //������Ƃ��̏u�Ԉړ��͂Ȃ������Ƃ��ł��邯�Ǒ����G���[�o����
    /*
    int checkX1, checkX2;
    int checkZ1, checkZ2;
    float ground1, ground2;

    checkX1 = (int)(transform_.position_.x + 0.15f); //�O
    checkZ1 = (int)(transform_.position_.z + 0.3f);
    checkX2 = (int)(transform_.position_.x - 0.15f);
    checkZ2 = (int)(transform_.position_.z + 0.3f);

    ground1 = pStage_->GetFloorHeight(checkX1, checkZ1);
    ground2 = pStage_->GetFloorHeight(checkX2, checkZ2);
    if (transform_.position_.y <= ground1 || transform_.position_.y <= ground2)
        return true;

    checkX1 = (int)(transform_.position_.x + 0.3f); //�E
    checkZ1 = (int)(transform_.position_.z + 0.15f);
    checkX2 = (int)(transform_.position_.x + 0.3f);
    checkZ2 = (int)(transform_.position_.z - 0.15f);

    ground1 = pStage_->GetFloorHeight(checkX1, checkZ1);
    ground2 = pStage_->GetFloorHeight(checkX2, checkZ2);
    if (transform_.position_.y <= ground1 || transform_.position_.y <= ground2)
        return true;

    checkX1 = (int)(transform_.position_.x + 0.15f); //���
    checkZ1 = (int)(transform_.position_.z - 0.3f);
    checkX2 = (int)(transform_.position_.x - 0.15f);
    checkZ2 = (int)(transform_.position_.z - 0.3f);

    ground1 = pStage_->GetFloorHeight(checkX1, checkZ1);
    ground2 = pStage_->GetFloorHeight(checkX2, checkZ2);
    if (transform_.position_.y <= ground1 || transform_.position_.y <= ground2)
        return true;

    checkX1 = (int)(transform_.position_.x - 0.3f); //��
    checkZ1 = (int)(transform_.position_.z + 0.15f);
    checkX2 = (int)(transform_.position_.x - 0.3f);
    checkZ2 = (int)(transform_.position_.z - 0.15f);

    ground1 = pStage_->GetFloorHeight(checkX1, checkZ1);
    ground2 = pStage_->GetFloorHeight(checkX2, checkZ2);
    if (transform_.position_.y <= ground1 || transform_.position_.y <= ground2)
        return true;
    */
    
    float ground = pStage_->GetFloorHeight((int)transform_.position_.x, (int)transform_.position_.z);
    if (transform_.position_.y <= ground)
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
    XMVECTOR v = XMLoadFloat3(&previousPosition_) - XMLoadFloat3(&transform_.position_);
    float a = XMVectorGetX(XMVector3Length(v));
    if (a != 0.0f)
        return true;

    return false;
}

void Player::IsInWall() {
    //�ǂƂ̔���  �����ǂ������Ƃ�
    int checkX1, checkX2;
    int checkZ1, checkZ2;
    float ground1, ground2;

    checkX1 = (int)(transform_.position_.x + 0.15f); //�O
    checkZ1 = (int)(transform_.position_.z + 0.3f);
    checkX2 = (int)(transform_.position_.x - 0.15f);
    checkZ2 = (int)(transform_.position_.z + 0.3f);

    ground1 = pStage_->GetFloorHeight(checkX1, checkZ1);
    ground2 = pStage_->GetFloorHeight(checkX2, checkZ2);
    if (transform_.position_.y < ground1 || transform_.position_.y < ground2)

    if (pStage_->IsWall(checkX1, checkZ1) == true || pStage_->IsWall(checkX2, checkZ2) == true) { //���₯�������
        transform_.position_.z = (float)((int)transform_.position_.z) + (1.0f - 0.3f);
    }

    checkX1 = (int)(transform_.position_.x + 0.3f); //�E
    checkZ1 = (int)(transform_.position_.z + 0.15f);
    checkX2 = (int)(transform_.position_.x + 0.3f);
    checkZ2 = (int)(transform_.position_.z - 0.15f);

    ground1 = pStage_->GetFloorHeight(checkX1, checkZ1);
    ground2 = pStage_->GetFloorHeight(checkX2, checkZ2);
    if (transform_.position_.y < ground1 || transform_.position_.y < ground2)

    if (pStage_->IsWall(checkX1, checkZ1) == true || pStage_->IsWall(checkX2, checkZ2) == true) {
        transform_.position_.x = (float)((int)transform_.position_.x + 1) - 0.3f;  // x�@�����߂����ƂŎ΂߈ړ����ł���悤�ɂȂ�     
    }

    checkX1 = (int)(transform_.position_.x + 0.15f); //���
    checkZ1 = (int)(transform_.position_.z - 0.3f);
    checkX2 = (int)(transform_.position_.x - 0.15f);
    checkZ2 = (int)(transform_.position_.z - 0.3f);

    ground1 = pStage_->GetFloorHeight(checkX1, checkZ1);
    ground2 = pStage_->GetFloorHeight(checkX2, checkZ2);
    if (transform_.position_.y < ground1 || transform_.position_.y < ground2)

    if (pStage_->IsWall(checkX1, checkZ1) == true || pStage_->IsWall(checkX2, checkZ2) == true) {
        transform_.position_.z = (float)((int)transform_.position_.z) + 0.3f;
    }

    checkX1 = (int)(transform_.position_.x - 0.3f); //��
    checkZ1 = (int)(transform_.position_.z + 0.15f);
    checkX2 = (int)(transform_.position_.x - 0.3f);
    checkZ2 = (int)(transform_.position_.z - 0.15f);

    ground1 = pStage_->GetFloorHeight(checkX1, checkZ1);
    ground2 = pStage_->GetFloorHeight(checkX2, checkZ2);
    if (transform_.position_.y < ground1 || transform_.position_.y < ground2)

    if (pStage_->IsWall(checkX1, checkZ1) == true || pStage_->IsWall(checkX2, checkZ2) == true) {
        transform_.position_.x = (float)((int)transform_.position_.x) + 0.3f;
    }

}