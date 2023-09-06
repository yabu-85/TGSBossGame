#include "SliderButton.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "TitleScene.h"
#include "Engine/Direct3D.h"
#include "AudioManager.h"

namespace {
	const XMFLOAT3 iSize = { 0.65f, 0.7f, 0 };
	const XMFLOAT3 nSize = { 0.6f, 0.6f, 0 };
	const int buttonPng = 3;

	int maxNum = 100;
	int num = 80;

}

SliderButton::SliderButton(GameObject* parent):
	GameObject(parent, "SliderButton"), hPict_{-1,-1, -1}, width_(0), height_(0), name_(""), widePos_{0,0,0}, alpha_(255), frameAlpha_(255),
	isButtonInactive_(true), frameSize_{0, 0, 0}, mode_(Direct3D::BLEND_ADD),isFirstPoint(true), buttonPosX_(0.0f)
{
}

SliderButton::~SliderButton()
{
}

void SliderButton::Initialize()
{
	alpha_ = 255;
	frameAlpha_ = 255;

	std::string fileName[] = { "SliderFrame", "Slider", "SliderButton" };

	for (int i = 0; i < 3; i++) {
		hPict_[i] = Image::Load("Png/" + fileName[i] + ".png");
		assert(hPict_[i] >= 0);
	}

}

void SliderButton::Update()
{
	if (!isButtonInactive_)
		return;

	XMFLOAT3 mouse = Input::GetMousePositionSub();
	if (IsWithinBound()) {
		//カーソルが重なってるとき一回再生
		if(isFirstPoint)
		{
			AudioManager::PlaySoundMa(AUDIO_POINTCURSOR);
			isFirstPoint = false;
		}

		if (Input::IsMouseButton(0)) {
			int frameSizeX = 512;
			float scrX = GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");
			float scrY = GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");

			XMFLOAT3 mouse = Input::GetMousePositionSub();
			mouse = { mouse.x / scrX, -(mouse.y / scrY), 0 };
			float gauge = (float)((num * 100) / maxNum) * 0.01f;

			if ((float)frameSizeX / (float)scrX * 2.0f * gauge - (float)frameSizeX / (float)scrX < mouse.x) {
				num++;
			}
			if ((float)frameSizeX / (float)scrX * 2.0f * gauge - (float)frameSizeX / (float)scrX > mouse.x) {
				num--;
			}
		}

		transform_.scale_.x = iSize.x * width_;
		transform_.scale_.y = iSize.y * height_;
	}
	else {
		isFirstPoint = true;

		transform_.scale_.x = nSize.x * width_;
		transform_.scale_.y = nSize.y * height_;
	}
}

void SliderButton::Draw()
{
	Direct3D::SetBlendMode(mode_);

	int frameSizeX = 512;
	float gauge = (float)((num * 100) / maxNum) * 0.01f;
	const int screnWidth = GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");

	//濃いほうのバー
	Transform slider = transform_;
	slider.scale_ = { 1.0f * width_, 1.0f * height_, 1.0f };
	Image::SetAlpha(hPict_[1], alpha_);
	Image::SetTransform(hPict_[1], slider);
	Image::Draw(hPict_[1]);

	//薄いほうのバー
	slider.position_.x -= ((float)frameSizeX / (float)screnWidth) - (float)frameSizeX / (float)screnWidth * gauge;
	Image::SetAlpha(hPict_[0], frameAlpha_);
	Image::SetRect(hPict_[0], 0, 0, (int)(frameSizeX * gauge) , 32);
	Image::SetTransform(hPict_[0], slider);
	Image::Draw(hPict_[0]);

	//ボタン
	Transform button = transform_;
	button.position_.x += (float)frameSizeX / (float)screnWidth * 2.0f * gauge - (float)frameSizeX / (float)screnWidth;
	Image::SetAlpha(hPict_[2], alpha_);
	Image::SetTransform(hPict_[2], button);
	Image::Draw(hPict_[2]);
	buttonPosX_ = button.position_.x;

}

void SliderButton::Release()
{
}

void SliderButton::SetValue(float x, float y, float w, float h, std::string n)
{
	static float screenWidth = (float)GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	static float screenHeight = (float)GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ
	frameSize_ = Image::GetTextureSize(hPict_[1]);

	transform_.position_.x = x / screenWidth;
	transform_.position_.y = y / screenHeight;

	transform_.scale_.x = 0.6f;
	transform_.scale_.y = 0.6f;

	width_ = w;
	height_ = h;
	name_ = n;
	widePos_ = { x, y, 0 };

}

bool SliderButton::IsWithinBound()
{
	if (!isButtonInactive_)
		return false;

	float scrX = GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");
	float scrY = GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");

	XMFLOAT3 mouse = Input::GetMousePositionSub();
	mouse = { mouse.x / scrX, -(mouse.y / scrY), 0 };

	XMFLOAT3 button = { buttonPosX_, widePos_.y / scrY, 0 };

	float distance = (float)sqrt(
		(mouse.x - button.x) * (mouse.x - button.x) +
		(mouse.y - button.y) * (mouse.y - button.y)
	);

	if (distance <= 0.05f) {
		return true;
	}

	return false;
}

void SliderButton::SetBlendMode(int mode)
{
	mode_ = (Direct3D::BLEND_MODE)mode;
}
