#include "UfoObstacle.h"
#include "Engine/SphereCollider.h"
#include "Engine/Model.h"

UfoObstacle::UfoObstacle(GameObject* parent)
	:Obstacle(parent)
{
}

UfoObstacle::~UfoObstacle()
{
}

void UfoObstacle::Initialize()
{
	//���f�����[�h
	hModel_ = Model::Load("Ufo.fbx");
	assert(hModel_ >= 0);

	//�����蔻��t�^
	BoxCollider* collision = new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(3.0f, 1.0f, 3.0f));
	AddCollider(collision);
}

void UfoObstacle::Update()
{
	///////////////�ړ��e�X�g
	transform_.position_.z -= 0.1f;
}

void UfoObstacle::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void UfoObstacle::Release()
{
}

void UfoObstacle::OnCollision(GameObject* pTarget)
{
	XMFLOAT3 position = { 0,0,0 };
	//Player�ɓ��������Ƃ�
	if (pTarget->GetObjectName() == "Player")
	{
		//SetPosition(XMFLOAT3(0, 0, 30));
		position = pTarget->GetPosition();
		//////////////////�v���C���[�m�b�N�o�b�N����	
	}
}