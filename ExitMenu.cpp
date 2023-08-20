#include "ExitMenu.h"
#include "Engine/Image.h"
#include "Button.h"
#include "Engine/Input.h"

struct ButtonInfoExit {
	float x;
	float y;
	float width;
	float height;
	std::string name;
}tbl[] = {
	{0.0f, -300.0f, 1.0f, 1.0f, "Ok"},
	{0.0f, -600.0f, 1.0f, 1.0f, "Back"},

};

ExitMenu::ExitMenu(GameObject* parent):
	GameObject(parent, "ExitMenu"), hPict_(-1), time_(0)
{
}

ExitMenu::~ExitMenu()
{
}

void ExitMenu::Initialize()
{
	ButtonInitializ();
	time_ = 0;
}

void ExitMenu::Update()
{
	if(time_ > 2)
	CheckButtonPressed();
	time_++;
}

void ExitMenu::Draw()
{
}

void ExitMenu::Release()
{
}

void ExitMenu::CheckButtonPressed()
{
	if (!Input::IsMouseButtonDown(0))
		return;

	std::list<GameObject*>* gs = GetChildList();
	for (GameObject* obj : *gs) {
		if (obj->GetObjectName() != "Button") {
			continue;
		}

		Button* pButton = (Button*)obj;
		if (pButton->IsButtonClicked()) {
			std::string na = pButton->GetName();

			if (na == "Ok") {
				PostQuitMessage(0); // プログラムを終了させるメッセージを送信
				break;
			}
			else if (na == "Back") {

				std::list<GameObject*>* gs2 = GetParent()->GetChildList();
				for (GameObject* obj : *gs2) {
					if (obj->GetObjectName() != "Button") {
						continue;
					}

					Button* pButton = (Button*)obj;
					pButton->SetActive(true);
					if (FindObject("PlayScene")) {
						pButton->SetAlpha_(100);
						pButton->SetFrameAlpha_(100);
					}
					else {
						pButton->SetAlpha_(255);
						pButton->SetFrameAlpha_(255);
					}
				}

				KillMe();
				break;
			}

		}
	}
}

void ExitMenu::ButtonInitializ()
{
	const int button = sizeof(tbl) / sizeof(tbl[0]);
	for (int i = 0; i < button; i++) {
		Button* pButton = nullptr;
		pButton = Instantiate<Button>(this);
		pButton->SetValue(tbl[i].x, tbl[i].y, tbl[i].width, tbl[i].height, tbl[i].name);
		pButton->SetAlpha_(100);
		pButton->SetFrameAlpha_(100);
	}
}
