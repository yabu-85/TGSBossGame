#pragma once
#include "Engine/GameObject.h"
class Logo : public GameObject
{
	int hPict_;
	bool end_;		//ロゴの表示終わったか

public:
	Logo(GameObject* parent);
	~Logo();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	bool IsEndDraw() { return end_; };
};

