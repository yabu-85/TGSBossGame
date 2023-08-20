#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class ResultScene : public GameObject
{
public:
	ResultScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};