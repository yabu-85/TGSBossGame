#pragma once
#include "Engine/GameObject.h"

class ButtonFactory;

class ExitMenu : public GameObject
{
	int hPict_;
	int time_;
	ButtonFactory* pButtonFactory_;

public:
	ExitMenu(GameObject* parent);
	~ExitMenu();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};

