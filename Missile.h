#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"

class Player;
class RobotObstacle;

class Missile :
    public GameObject
{	
    XMFLOAT3 launchPoint_;          //発射場所
	XMFLOAT3 rotationAngle_;		//反射後のランダムな回転
	XMVECTOR position;				//座標
	XMVECTOR velocity;				//速度
	XMVECTOR target;				//ターゲット
	float maxCentripetalAccel;		//最大向心加速度
	float propulsion;				// 推進力
	float countPerMeter;			//メートルあたりのカウント
	float speed;					//スピード
	float damping;					//減衰
	float impact;					//衝突距離
	int hModel_;
    bool missileReflected_;         //反射されたかどうか falseされてない
	bool killParent_;				//親を倒す固体かどうか
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

