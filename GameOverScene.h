#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class GameOverScene : public GameObject
{
public:
	GameOverScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};