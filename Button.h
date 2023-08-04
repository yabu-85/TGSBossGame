#pragma once
#include "Engine/GameObject.h"

class TitleScene;

class Button : public GameObject
{
	int hPict_[2];	
	float width_;		// ボタンの幅
	float height_;		// ボタンの高さ
	std::string name_;	// ボタンに表示するPngの
	TitleScene* pTitleScene_;
	XMFLOAT3 widePos_;	//-1～1じゃなく,-1280～1280とかの座標
	
public:
	Button(GameObject* parent);
	~Button();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetValue(float x, float y, float w, float h, std::string n);
};

