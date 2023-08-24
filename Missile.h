#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"

class Player;
class RobotObstacle;

class Missile :
    public GameObject
{	
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
	int hModel_;
    bool missileReflected_;         //���˂��ꂽ���ǂ��� false����ĂȂ�
	bool killParent_;				//�e��|���ő̂��ǂ���
	EmitterData dataExp_;
	RobotObstacle* pRobotObstacle_;	
	Player* pPlayer_;


public:
	Missile(GameObject* parent);
	~Missile();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetTarget(float x, float y, float z);
	void SetParent(RobotObstacle* p) { pRobotObstacle_ = p; };
	void SetKillParent(bool b) { killParent_ = b; };

};

