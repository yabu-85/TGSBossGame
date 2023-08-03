#include "Button.h"
#include "Engine/Image.h"
#include "TitleScene.h"

Button::Button(GameObject* parent):
	GameObject(parent, "Button"), hPict_{-1,-1}, width_(0), height_(0), name_("")
{
}

Button::~Button()
{
}

void Button::Initialize()
{
}

void Button::Update()
{
}

void Button::Draw()
{
	if (hPict_[0] >= 0) {
		Image::SetTransform(hPict_[0], transform_);
		Image::Draw(hPict_[0]);

		Transform pos = transform_;
		Image::SetTransform(hPict_[1], pos);
		Image::Draw(hPict_[1]);
	}

}

void Button::Release()
{
}

void Button::SetValue(int x, int y, int w, int h, std::string n)
{
	static float screenWidth = (float)GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	static float screenHeight = (float)GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ
	
	transform_.position_.x = x / screenWidth;
	transform_.position_.y = y / screenHeight;

	transform_.scale_.x = 0.6f;
	transform_.scale_.y = 0.6f;

	width_ = w;
	height_ = h;
	name_ = n;

	std::string str1 = name_;
	std::string str2 = ".png";
	std::string str = str1 + str2;

	hPict_[0] = Image::Load(str);
	assert(hPict_[0] >= 0);

	hPict_[1] = Image::Load("Frame.png");
	assert(hPict_[1] >= 0);
}
