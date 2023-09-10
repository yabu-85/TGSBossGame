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
    bool missileReflected_;         //反射されたかどうか falseされてない
	bool isActive_;					//アクティブ状態か
	
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

