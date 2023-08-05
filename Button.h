#pragma once
#include "Engine/GameObject.h"

class TitleScene;

class Button : public GameObject
{
	int hPict_[2];
	int alpha_;				//透明度
	float width_;			//ボタンの幅
	float height_;			//ボタンの高さ
	bool isButtonInactive_; //ボタンを押す感知しない
	std::string name_;		//ボタンに表示するPngの
	XMFLOAT3 widePos_;		//-1～1じゃなく,-1280～1280とかの座標

public:
	Button(GameObject* parent);
	~Button();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetValue(float x, float y, float w, float h, std::string n);
	void SetAlpha_(int i) { alpha_ = i; }; //0（透明）～255（表示）
	void SetActive(bool b) { isButtonInactive_ = b; };
	bool IsButtonClicked();
	std::string GetName() { return name_; };
};

