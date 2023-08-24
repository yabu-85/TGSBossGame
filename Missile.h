#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"

class Player;

class Missile :
    public GameObject
{	
    XMFLOAT3 launchPoint_;          //発射場所
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

