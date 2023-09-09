#include "BossObstacle.h"
#include "Engine/Model.h"
#include "Player.h"
#include "Missile.h"

//�����̃C���[�W��CHORUS�̃v���C���[
//HP��0�ɂ����珟���APlayer��HP���O��������TimeOver�ɂȂ����畉��
//���̂Ƃ��̍s��
//�P�F�r�[����ł��Ȃ���O�֐i�ށi�r�[���̎d�l�F�\�����\�����Ȃ��炻�̏ꏊ�֑łj
//�@���̎��v���C���[�����̋����܂ŋ߂Â������_���[�W�������I�ɑ��̍s���ֈڍs
//�Q�F�n���H�𗎂Ƃ��iBRIGHTTRACTER�̃_���[�W�G���A�݂����Ȃ��
//�@��납��O���֏��ɂ��Ó�
//�@1�����̓v���C���[�̂��傢���A�Q���v���C���[�͈͓̔��A�R�`�ړ����Ă��瓖���肻���ȏꏊ��
//�R�F�~�T�C���𔭎˃��{�b�g�̂���̂܂܂ł���������
//�@���˂͉\�ŁA�_���[�W���^������悤��

//HP�������ȉ��ɂȂ�����
//�P�F����r�[����ł悤�ɂ���
//�Q�F�͈͑��₷�H
//�R�F�񐔁A���ː��𑝂₷
//�S�G�o������ǉ��Z

BossObstacle::BossObstacle(GameObject* parent)
	:Obstacle(parent), pPlayer_(nullptr), stateEnter_(true), state_(S_ENTER), moveSpeed_(0.0f)
{
	objectName_ = "BossObstacle";
    MoveDirection_ = XMVectorSet(0, 0, 0, 0);
}

BossObstacle::~BossObstacle()
{
}

void BossObstacle::Initialize()
{
	//���f�����[�h
	hModel_ = Model::Load("Model/Boss_03.fbx");
	assert(hModel_ >= 0);

	pPlayer_ = (Player*)FindObject("Player");
}

void BossObstacle::Update()
{
	if (!active_) return;

    switch (state_) {
    case STATE::S_ENTER:
        UpdateEnter();
        break;
    case STATE::S_MISSILE:
        UpdateMissile();
        break;
    case STATE::S_DEAD:
        UpdateDead();
        break;
    }

    XMFLOAT3 plaPos = pPlayer_->GetPosition();
    if (!IsInTargetPosition(plaPos, 80.0f)) {
        KillMe();
    }

}

void BossObstacle::Draw()
{
	if (!isDraw_) return;

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_, 0);
}

void BossObstacle::Release()
{
}

//--------------state---------------//

void BossObstacle::UpdateEnter()
{
	if (stateEnter_) {
        stateEnter_ = false;
        transform_.position_.z -= 100.0f;
		transform_.position_.y = 2.0f;
        transform_.rotate_.y = 0;
	}

    if (!IsInTargetPosition(2.0f)) {
        targetPosition_ = pPlayer_->GetPosition();
        targetPosition_.y = 5.0f;
        targetPosition_.z += 30.0f;
        moveSpeed_ = 0.6f;
        Move();

        return;
    }

    ChangeState(S_MISSILE);
}

void BossObstacle::UpdateMissile()
{
    return;

    Missile* pMissile = Instantiate<Missile>(GetParent());
    pMissile->SetPosition(transform_.position_.x, transform_.position_.y, transform_.position_.z);
    pMissile->SetTarget(0.0f, 0.0f, 0.0f);
    
    //
}

void BossObstacle::UpdateDead()
{
}

void BossObstacle::ChangeState(STATE s)
{
	state_ = s;
	stateEnter_ = true;
}

//--------------private-----------------//

void BossObstacle::Rotate(float x, float z, float _rotateSpeed)
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

float BossObstacle::NormalizeAngle(float angle)
{
    while (angle > 180.0f) {
        angle -= 360.0f;
    }
    while (angle < -180.0f) {
        angle += 360.0f;
    }
    return angle;
}

void BossObstacle::Move()
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vTar = XMLoadFloat3(&targetPosition_);
    XMVECTOR vVec = XMVector3Normalize(vTar - vPos) * 0.2f;
    vVec = XMVector3Normalize(vVec + MoveDirection_);
    MoveDirection_ = vVec;
    XMStoreFloat3(&transform_.position_, vPos + (vVec * moveSpeed_) );

}

bool BossObstacle::IsInTargetPosition(float _leng)
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vTar = XMLoadFloat3(&targetPosition_);
    float distance = XMVectorGetX(XMVector3Length(vTar - vPos));
    if (distance <= _leng) return true;

    return false;
}

bool BossObstacle::IsInTargetPosition(XMFLOAT3 _tar, float _leng)
{
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vTar = XMLoadFloat3(&_tar);
    float distance = XMVectorGetX(XMVector3Length(vTar - vPos));
    if (distance <= _leng) return true;

    return false;
}

void BossObstacle::ShotBeam()
{

}

void BossObstacle::AirStrike()
{
}

void BossObstacle::ShotMissile()
{
}

