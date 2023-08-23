#include "ButtonFactory.h"
#include "Engine/Input.h"
#include "Button.h"

ButtonFactory::ButtonFactory(GameObject* parent)
	:GameObject(parent, "ButtonFactory")
{
}

ButtonFactory::~ButtonFactory()
{
}

void ButtonFactory::Initialize()
{
}

void ButtonFactory::Update()
{
}

void ButtonFactory::Draw()
{
}

void ButtonFactory::Release()
{
}

std::string ButtonFactory::CheckButtonPressed()
{
	if (!Input::IsMouseButtonDown(0))
		return "";

	for (Button* obj : buttons_) {
		if (obj->IsWithinBound()) {
			std::string na = obj->GetName();
			return na;

		}
	}
	
	return "";
}

void ButtonFactory::ButtonCreate(float x, float y, float w, float h, std::string n)
{
	Button* pButton = nullptr;
	pButton = Instantiate<Button>(this);
	pButton->SetValue(x, y, w, h, n);
	pButton->SetAlpha(100);
	pButton->SetFrameAlpha(100);
	buttons_.push_back(pButton);

}

void ButtonFactory::ButtonCreate(float x, float y, float w, float h, std::string n, std::string f)
{
	Button* pButton = nullptr;
	pButton = Instantiate<Button>(this);
	pButton->SetValue(x, y, w, h, n, f);
	pButton->SetAlpha(100);
	pButton->SetFrameAlpha(100);
	buttons_.push_back(pButton);

}

void ButtonFactory::SetActive(bool b)
{
	for (Button* e : buttons_) {
		e->SetActive(b);
	}
}

void ButtonFactory::SetAlpha(int i)
{
	for (Button* e : buttons_) {
		e->SetAlpha(i);
	}
}

void ButtonFactory::SetFrameAlpha(int i)
{
	for (Button* e : buttons_) {
		e->SetFrameAlpha(i);
	}
}
