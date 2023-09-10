#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"

class Player;
class RobotObstacle;

class Missile :
    public GameObject
{	
	int hModel_;
	EmitterData dataExp_;

    XMFLOAT3 launchPoint_;          //���ˏꏊ
	XMFLOAT3 rotationAngle_;		//���ˌ�̃����_���ȉ�]
	XMVECTOR position;				//���W
	XMVECTOR velocity;				//���x
	XMVECTOR target;				//�^�[�Q�b�g
	float maxCentripetalAccel;		//�ő���S�����x
	float propulsion;				// ���i��
	float countPerMeter;			//���[�g��������̃J�E���g
	float speed;					//�X�s�[�h
	float damping;					//����
	float impact;					//�Փˋ���
    bool missileReflected_;         //���˂��ꂽ���ǂ��� false����ĂȂ�
	bool isActive_;					//�A�N�e�B�u��Ԃ�
	
	RobotObstacle* pRobotObstacle_;	
	Player* pPlayer_;

	void CreateExplodeParticle();

public:
	Missile(GameObject* parent);
	~Missile();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetTarget(float x, float y, float z);
	void SetParent(RobotObstacle* p) { pRobotObstacle_ = p; };
	void SetReflected(bool b) { missileReflected_ = b; };
	void Reflect();

	void SetActive(bool b) { isActive_ = b; };
	void KillMeSub();

};

