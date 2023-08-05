#include "PauseMenu.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Button.h"
#include "PlayScene.h"
#include "Aim.h"
#include "ExitMenu.h"
#include "Player.h"
#include "Engine/SceneManager.h"

struct ButtonInfoPause {
	float x;
	float y;
	float width;
	float height;
	std::string name;
}tbl[] = {
	{0.0f, 200.0f, 100.0f, 100.0f, "ReturnGame"},
	{0.0f, 0.0f, 100.0f, 100.0f, "Setting"},
	{0.0f, -200.0f, 100.0f, 100.0f, "ReturnTitle"},
	{0.0f, -400.0f, 100.0f, 100.0f, "Quit"},

};

PauseMenu::PauseMenu(GameObject* parent)
	:GameObject(parent, "PauseMenu"), hPict_{-1,-1}
{
}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::Initialize()
{
	ButtonInitializ();

	Aim* pAim = (Aim*)FindObject("Aim");
	pAim->SetAimMove(false);

	hPict_[0] = Image::Load("Black.png");
	assert(hPict_[0] >= 0);

	hPict_[1] = Image::Load("cross.png");
	assert(hPict_[1] >= 0);
}

void PauseMenu::Update()
{
	CheckButtonPressed();

}

void PauseMenu::Draw()
{

	Image::SetAlpha(hPict_[0], 50);
	Image::SetTransform(hPict_[0], transform_);
	Image::Draw(hPict_[0]);

	Transform abb;
	abb.position_ = Input::GetMousePositionSub();
	static float screenWidth = (float)GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	static float screenHeight = (float)GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ
	abb.position_ = { abb.position_.x / screenWidth, -abb.position_.y / screenHeight , 0 };

	Image::SetTransform(hPict_[1], abb);
	Image::Draw(hPict_[1]);
}

void PauseMenu::Release()
{
}

void PauseMenu::ButtonInitializ()
{
	const int button = sizeof(tbl) / sizeof(tbl[0]);
	for (int i = 0; i < button; i++) {
		Button* pButton = nullptr;
		pButton = Instantiate<Button>(this);
		pButton->SetValue(tbl[i].x, tbl[i].y, tbl[i].width, tbl[i].height, tbl[i].name);
	}
}

void PauseMenu::CheckButtonPressed()
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

			if (na == "ReturnGame") {
				Aim* pAim = (Aim*)FindObject("Aim");
				pAim->SetAimMove(true);
				Player* pPlayer = (Player*)FindObject("Player");
				pPlayer->SetActive(true);
				KillMe();
				break;
			}
			else if (na == "Setting") {

			}
			else if (na == "ReturnTitle") {
				SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
				pSceneManager->ChangeScene(SCENE_ID_TITLE);
				break;
			}
			else if (na == "Quit") {
				Instantiate<ExitMenu>(this);	

				for (GameObject* obj : *gs) {
					if (obj->GetObjectName() != "Button") {
						continue;
					}

					Button* pButton = (Button*)obj;
					pButton->SetActive(false);
					pButton->SetAlpha_(10);

				}
				break;
			}
		}
	}
}
