#pragma once
#include "Engine/GameObject.h"
class Logo : public GameObject
{
	int hPict_[2];
	bool end_;		//���S�̕\���I�������

public:
	Logo(GameObject* parent);
	~Logo();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	bool IsEndDraw() { return end_; };
};

