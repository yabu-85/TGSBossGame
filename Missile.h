#pragma once
#include "Engine/GameObject.h"

class Missile :
    public GameObject
{
	int hModel_;
public:
	Missile(GameObject* parent);
	~Missile();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};

