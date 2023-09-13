#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class ButtonFactory;

class ResultScene : public GameObject
{
	int hPict_[3];
	int hPictTime_[12];    //âÊëúî‘çÜ
	int hPictLank_;
	int clearTime_;
	int clearTimeSma_;
	bool result_;

	ButtonFactory* pButtonFactory_;

	void DrawCursor();

public:
	ResultScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};