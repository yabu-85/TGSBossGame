#include "Button.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "TitleScene.h"

static XMFLOAT3 iSize = { 0.65f, 0.7f, 0 };
static XMFLOAT3 nSize = { 0.6f, 0.6f, 0 };

Button::Button(GameObject* parent):
	GameObject(parent, "Button"), hPict_{-1,-1}, width_(0), height_(0), name_(""), widePos_{0,0,0}, alpha_(255),
	isButtonInactive_(true), frameAlpha_(255), frameSize_{0, 0, 0}
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
	if (IsButtonClicked()) {
		transform_.scale_.x = iSize.x * width_;
		transform_.scale_.y = iSize.y * height_;
		alpha_ = 255;
	}
	else {
		transform_.scale_.x = nSize.x * width_;
		transform_.scale_.y = nSize.y * height_;
		alpha_ = 230;
	}

}

void Button::Draw()
{
	Image::SetAlpha(hPict_[0], alpha_);
	Image::SetTransform(hPict_[0], transform_);
	Image::Draw(hPict_[0]);

	Image::SetAlpha(hPict_[1], frameAlpha_);
	Image::SetTransform(hPict_[1], transform_);
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
	frameSize_ = Image::GetTextureSize(hPict_[1]);

}

void Button::SetValue(float x, float y, float w, float h, std::string n, std::string frame)
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

	str1 = frame;
	str2 = ".png";
	str = str1 + str2;

	hPict_[1] = Image::Load(str);
	assert(hPict_[1] >= 0);
	frameSize_ = Image::GetTextureSize(hPict_[1]);
}

bool Button::IsButtonClicked()
{
	if (!isButtonInactive_)
		return false;

	XMFLOAT3 mouse = Input::GetMousePositionSub();
	mouse.x = -mouse.x;
	

	if (-mouse.y < (widePos_.y + (frameSize_.y * iSize.y) * height_) && -mouse.y > (widePos_.y - (frameSize_.y * iSize.y) * height_) &&
		-mouse.x < (widePos_.x + (frameSize_.x * iSize.x) * width_) && -mouse.x > (widePos_.x - (frameSize_.x * iSize.x) * width_))
		return true;

	return false;
}
