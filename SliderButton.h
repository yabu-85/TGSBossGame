#pragma once
#include "Engine/GameObject.h"
#include "Engine/Direct3D.h"

class TitleScene;

class SliderButton : public GameObject
{
	int hPict_[3];
	int alpha_;				//透明度
	int frameAlpha_;		//透明度

	float width_;			//ボタンの幅
	float height_;			//ボタンの高さ
	float buttonPosX_;		//X軸のボタンの座標（0~1
	bool isButtonInactive_; //ボタンを押す感知しない
	bool isFirstPoint;		//初回照準か
	std::string name_;		//ボタンに表示するPngの
	XMFLOAT3 widePos_;		//-1〜1じゃなく,-1280〜1280とかの座標
	XMFLOAT3 frameSize_;	//Frameのサイズ
	Direct3D::BLEND_MODE mode_;

public:
	SliderButton(GameObject* parent);
	~SliderButton();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//因数:座標x,y,サイズx.y, 画像の名前
	void SetValue(float x, float y, float w, float h, std::string n);

	void SetAlpha(int i) { alpha_ = i; }; //0（透明）〜255（表示）
	void SetFrameAlpha(int i) { frameAlpha_ = i; }; //0（透明）〜255（表示）
	void SetActive(bool b) { isButtonInactive_ = b; };
	bool IsWithinBound();
	std::string GetName() { return name_; };
	void SetBlendMode(int mode);

};

