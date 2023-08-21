#include "MissileObstacle.h"
#include "Engine/BoxCollider.h"
#include "Engine/Model.h"

MissileObstacle::MissileObstacle(GameObject* parent)
	:Obstacle(parent)
{
}

MissileObstacle::~MissileObstacle()
{
}

void MissileObstacle::Initialize()
{
	//���f�����[�h
	hModel_ = Model::Load("Missile.fbx");
	assert(hModel_ >= 0);

	//�����蔻��t�^
	BoxCollider* collision = new BoxCollider(XMFLOAT3(0,0,0), XMFLOAT3(0.8f, 0.8f, 1.8f));
	AddCollider(collision);

	//�h���A�j���[�V����
	Model::SetAnimFrame(hModel_, 0, 60, 1);

}

void MissileObstacle::Update()
{
	if (!active_) return;

	///////////////�ړ��e�X�g
	transform_.position_.z -= 0.1f;
	transform_.position_.y = 1.0f;


}

void MissileObstacle::Draw()
{
	if (!active_) return;

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void MissileObstacle::Release()
{
}

void MissileObstacle::OnCollision(GameObject* pTarget)
{
	XMFLOAT3 position = {0,0,0};
	//Player�ɓ��������Ƃ�
	if (pTarget->GetObjectName() == "Player")
	{
		//SetPosition(XMFLOAT3(0, 0, 30));
		position = pTarget->GetPosition();
		//////////////////�v���C���[�m�b�N�o�b�N����	
	}
}