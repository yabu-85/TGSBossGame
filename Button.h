#pragma once
#include "Engine/GameObject.h"

struct ButtonInfo;

class Button : public GameObject
{
	int hPict_;	
	float xPos_;		// ボタンのX座標
	float yPos_;		// ボタンのY座標
	float width_;		// ボタンの幅
	float height_;		// ボタンの高さ
	std::string name_;	// ボタンに表示するPngの名前

public:
	Button(GameObject* parent);
	~Button();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetValue(int x, int y, int w, int h, std::string n);
};

