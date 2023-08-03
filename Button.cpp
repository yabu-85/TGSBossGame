#include "Button.h"
#include "Engine/Image.h"
#include "TitleScene.h"

Button::Button(GameObject* parent):
	GameObject(parent, "Button"), hPict_(-1), xPos_(0), yPos_(0), width_(0), height_(0), name_("")
{
}

Button::~Button()
{
}

void Button::Initialize()
{
	hPict_ = Image::Load("water.png");
	assert(hPict_ >= 0);
}

void Button::Update()
{
}

void Button::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void Button::Release()
{
}

void Button::SetValue(int x, int y, int w, int h, std::string n)
{
	xPos_ = x;
	yPos_ = y;
	width_ = w;
	height_ = h;
	name_ = n;
}
