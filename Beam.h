#pragma once
#include "Engine/GameObject.h"

class Player;

class Beam :
    public GameObject
{
    int hModel_;
	bool isActive_;					//アクティブ状態か
	bool isTargetHit_;				//ターゲット位置に当たった
	bool isPlayerHit_;				//プレイヤーに当たった
	float minLeng_;					//当たり判定用
	XMFLOAT3 target_;				//ターゲット
	XMVECTOR vec_;					//移動方向
	XMFLOAT3 prevPosition_;			//前の座標（当たり判定のため

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

	bool IsTargetHit() { return isTargetHit_; };	//目標地点について爆破したか
	bool IsPlayerHit() { return isPlayerHit_; };	//ターゲットに当たったか

};

