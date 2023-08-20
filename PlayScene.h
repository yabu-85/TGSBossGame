#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class Timer;

class PlayScene  : public GameObject
{
	Timer* pTimer_;

public:
	PlayScene (GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};