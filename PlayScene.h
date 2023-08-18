#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class PlayScene  : public GameObject
{
public:
	PlayScene (GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//タイマー
	void TimeProcess();
};