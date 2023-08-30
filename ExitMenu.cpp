#include "ExitMenu.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "ButtonFactory.h"
#include "Button.h"

ExitMenu::ExitMenu(GameObject* parent):
	GameObject(parent, "ExitMenu"), hPict_(-1), time_(0), pButtonFactory_(nullptr)
{
}

ExitMenu::~ExitMenu()
{
}

void ExitMenu::Initialize()
{
	pButtonFactory_ = Instantiate<ButtonFactory>(this);
	pButtonFactory_->ButtonCreate(0.0f, -300.0f, 1.0f, 1.0f, "Ok");
	pButtonFactory_->ButtonCreate(0.0f, -600.0f, 1.0f, 1.0f, "Back");
	pButtonFactory_->SetAlpha(255);
	pButtonFactory_->SetFrameAlpha(255);
	pButtonFactory_->SetBlendMode(0);

	time_ = 0;
}

void ExitMenu::Update()
{
	time_++;
	if (time_ < 2) return;

	if (pButtonFactory_->CheckButtonPressed() == "Ok") {
		PostQuitMessage(0); // プログラムを終了させるメッセージを送信
		return;
	}
	else if (pButtonFactory_->CheckButtonPressed() == "Back") {
		GameObject* gs2 = GetParent()->FindObject("ButtonFactory");
		ButtonFactory* pB = (ButtonFactory*)gs2;
		pB->SetActive(true);
		pB->SetAlpha(200);
		pB->SetFrameAlpha(200);

		KillMe();
		return;
	}
}

void ExitMenu::Draw()
{
}

void ExitMenu::Release()
{
}
