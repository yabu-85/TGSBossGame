#pragma once
#include "Engine/GameObject.h"

class Missile :
    public GameObject
{	
	XMVECTOR position;				//���W
	XMVECTOR velocity;				//���x
	XMVECTOR target;				//�^�[�Q�b�g
	float maxCentripetalAccel;		//�ő���S�����x
	float propulsion;				// ���i��
	float countPerMeter;			//���[�g��������̃J�E���g
	float speed;					//�X�s�[�h
	float curvatureRadius;			//�ȗ����a
	float damping;					//����
	float impact;					//�Փˋ���

	int hModel_;
public:
	Missile(GameObject* parent);
	~Missile();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetTarget(float x, float y, float z);

};

