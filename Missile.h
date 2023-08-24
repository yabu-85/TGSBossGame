#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"

class Player;

class Missile :
    public GameObject
{	
    XMFLOAT3 launchPoint_;          //���ˏꏊ
	XMVECTOR position;				//���W
	XMVECTOR velocity;				//���x
	XMVECTOR target;				//�^�[�Q�b�g
	float maxCentripetalAccel;		//�ő���S�����x
	float propulsion;				// ���i��
	float countPerMeter;			//���[�g��������̃J�E���g
	float speed;					//�X�s�[�h
	float damping;					//����
	float impact;					//�Փˋ���
	int hModel_;
    bool missileReflected_;         //���˂��ꂽ���ǂ��� false����ĂȂ�
	Player* pPlayer_;
    EmitterData dataExp;


public:
	Missile(GameObject* parent);
	~Missile();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetTarget(float x, float y, float z);

};

