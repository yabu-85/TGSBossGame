#pragma once
#include "Engine/GameObject.h"

class Missile :
    public GameObject
{	
	XMVECTOR position;				//座標
	XMVECTOR velocity;				//速度
	XMVECTOR target;				//ターゲット
	float maxCentripetalAccel;		//最大向心加速度
	float propulsion;				// 推進力
	float countPerMeter;			//メートルあたりのカウント
	float speed;					//スピード
	float curvatureRadius;			//曲率半径
	float damping;					//減衰
	float impact;					//衝突距離

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

