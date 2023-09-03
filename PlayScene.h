#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class Timer;
class Player;

class PlayScene  : public GameObject
{
	int hPict_;		//画像番号

	Timer* pTimer_;
	Player* pPlayer_;

public:
	PlayScene (GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//PlaySceneのすべてのActiveを制御する
	void SetObjectActive(bool _active);

};