#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class ResultScene : public GameObject
{
	int hPict_[3];

	void DrawCursor();
	void ButtonInitializ();
	void CheckButtonPressed();	//�{�^�������ꂽ�����ׂ�

public:
	ResultScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};