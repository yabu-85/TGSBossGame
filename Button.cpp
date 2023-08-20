#include "Button.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "TitleScene.h"

Button::Button(GameObject* parent):
	GameObject(parent, "Button"), hPict_{-1,-1}, width_(0), height_(0), name_(""), widePos_{0,0,0}, alpha_(255),
	isButtonInactive_(true), frameAlpha_(255)
{
}

Button::~Button()
{
}

void Button::Initialize()
{
	alpha_ = 255;
	frameAlpha_ = 255;
}

void Button::Update()
{

	if (!isButtonInactive_)
		return;

	XMFLOAT3 mouse = Input::GetMousePositionSub();
	if (-mouse.y < widePos_.y + 80 && -mouse.y > widePos_.y - 80) {
		transform_.scale_.x = 0.65f;
		transform_.scale_.y = 0.7f;
		alpha_ = 255;
	}
	else {
		transform_.scale_.x = 0.6f;
		transform_.scale_.y = 0.6f;
		alpha_ = 150;
	}

}

void Button::Draw()
{
	Image::SetAlpha(hPict_[0], alpha_);
	Image::SetTransform(hPict_[0], transform_);
	Image::Draw(hPict_[0]);

	Transform pos = transform_;
	Image::SetAlpha(hPict_[1], frameAlpha_);
	Image::SetTransform(hPict_[1], pos);
	Image::Draw(hPict_[1]);

}

void Button::Release()
{
}

void Button::SetValue(float x, float y, float w, float h, std::string n)
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
	widePos_ = { x, y, 0 };

	std::string str1 = name_;
	std::string str2 = ".png";
	std::string str = str1 + str2;

	hPict_[0] = Image::Load(str);
	assert(hPict_[0] >= 0);

	// 1100 * 120
	hPict_[1] = Image::Load("Frame.png");
	assert(hPict_[1] >= 0);
}

bool Button::IsButtonClicked()
{
	if (!isButtonInactive_)
		return false;

	XMFLOAT3 mouse = Input::GetMousePositionSub();
	if (-mouse.y < widePos_.y + 80 && -mouse.y > widePos_.y - 80) 
		return true;
	
	return false;
}
