#pragma once
#include "Engine/GameObject.h"

class Player;

class Beam :
    public GameObject
{
    int hModel_;
	bool isActive_;					//�A�N�e�B�u��Ԃ�
	bool isTargetHit_;				//�^�[�Q�b�g�ʒu�ɓ�������
	bool isPlayerHit_;				//�v���C���[�ɓ�������
	float minLeng_;					//�����蔻��p
	XMFLOAT3 target_;				//�^�[�Q�b�g
	XMVECTOR vec_;					//�ړ�����
	XMFLOAT3 prevPosition_;			//�O�̍��W�i�����蔻��̂���

	Player* pPlayer_;

	void Rotate();

public:
	Beam(GameObject* parent);
	~Beam();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetActive(bool b) { isActive_ = b; };
	void KillMeSub();

	bool IsTargetHit() { return isTargetHit_; };	//�ڕW�n�_�ɂ��Ĕ��j������
	bool IsPlayerHit() { return isPlayerHit_; };	//�^�[�Q�b�g�ɓ���������

};

