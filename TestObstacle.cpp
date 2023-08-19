#include "TestObstacle.h"
#include "Engine/BoxCollider.h"
#include "Engine/Model.h"

TestObstacle::TestObstacle(GameObject* parent)
	:Obstacle(parent)
{
}

TestObstacle::~TestObstacle()
{
}

void TestObstacle::Initialize()
{
	//���f�����[�h
	hModel_ = Model::Load("Missile.fbx");
	assert(hModel_ >= 0);

	//�����蔻��t�^
	BoxCollider* collision = new BoxCollider(XMFLOAT3(0,0,0), XMFLOAT3(0.8f, 0.8f, 1.8f));
	AddCollider(collision);
}

void TestObstacle::Update()
{
	///////////////�ړ��e�X�g
	transform_.position_.z -= 0.1f;
}

void TestObstacle::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void TestObstacle::Release()
{
}

void TestObstacle::OnCollision(GameObject* pTarget)
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