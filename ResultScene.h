#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class ButtonFactory;

class ResultScene : public GameObject
{
	int hPict_[3];
	ButtonFactory* pButtonFactory_;

	void DrawCursor();

public:
	ResultScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};