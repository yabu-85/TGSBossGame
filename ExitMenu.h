#pragma once
#include "Engine/GameObject.h"

class ExitMenu : public GameObject
{
	int hPict_;
	int time_;

	void CheckButtonPressed();
	void ButtonInitializ();

public:
	ExitMenu(GameObject* parent);
	~ExitMenu();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;


};

