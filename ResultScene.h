#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class ResultScene : public GameObject
{
	int hPict_[3];

	void DrawCursor();
	void ButtonInitializ();
	void CheckButtonPressed();	//ƒ{ƒ^ƒ“‰Ÿ‚³‚ê‚½‚©’²‚×‚é

public:
	ResultScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};