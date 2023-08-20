#include "Stageselect.h"
#include "Engine/Input.h"
#include "Button.h"

Stageselect::Stageselect(GameObject* parent)
	:GameObject(parent, "Timer")
{
}

struct ButtonInfoStage {
	float x;
	float y;
	float width;
	float height;
	std::string name;
}tbl[] = {
	{-1000.0f, 0.0f, 0.6f, 1.0f, "Clear"},
	{0.0f, 0.0f, 0.6f, 1.0f, "Over"},
	{1000.0f, 0.0f, 0.6f, 1.0f, "Title"},
	{0.0f, -300.0f, 1.0f, 1.0f, "Back"},

};

Stageselect::~Stageselect()
{
}

void Stageselect::Initialize()
{
	ButtonInitializ();
}

void Stageselect::Update()
{
	CheckButtonPressed();

}

void Stageselect::Draw()
{
}

void Stageselect::Release()
{
}



void Stageselect::CheckButtonPressed()
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

			if (na == "1") {
				PostQuitMessage(0); // プログラムを終了させるメッセージを送信
				break;
			}
			else if (na == "2") {
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
					pButton->SetAlpha_(255);
					pButton->SetFrameAlpha_(255);

				}

				KillMe();
				break;
			}

		}
	}
}

void Stageselect::ButtonInitializ()
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