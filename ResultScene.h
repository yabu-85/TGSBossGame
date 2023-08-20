#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class ResultScene : public GameObject
{
	int hPict_[2];

	void DrawCursor();
	void ButtonInitializ();
	void CheckButtonPressed();	//ボタン押されたか調べる

public:
	ResultScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};