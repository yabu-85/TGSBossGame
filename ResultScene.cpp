#include "ResultScene.h"
#include "ExitMenu.h"	
#include "Engine/SceneManager.h"
#include "Button.h"
#include "Engine/Input.h"

//コンストラクタ
ResultScene::ResultScene(GameObject* parent)
	: GameObject(parent, "ResultScene"), hPict_{ -1, -1 }
{
}

struct ButtonInfoResult {
	float x;
	float y;
	float width;
	float height;
	std::string name;
}tbl[] = {
	{0.0f, -300.0f, 100.0f, 100.0f, "ReturnTitle"},
	{0.0f, -600.0f, 100.0f, 100.0f, "Quit"},

};

//初期化
void ResultScene::Initialize()
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager->GetResult()) {
		hPict_[0] = Image::Load("Clear.png");
		assert(hPict_[0] >= 0);
	}
	else {
		hPict_[0] = Image::Load("Over.png");
		assert(hPict_[0] >= 0);
	}

	hPict_[1] = Image::Load("cross.png");
	assert(hPict_[1] >= 0);

	ButtonInitializ();

}

//更新
void ResultScene::Update()
{
	CheckButtonPressed();
}

//描画
void ResultScene::Draw()
{
	Transform pos;
	pos.position_.y = 0.2f;
	Image::SetTransform(hPict_[0], pos);
	Image::Draw(hPict_[0]);

	DrawCursor();
}

//開放
void ResultScene::Release()
{
}



void ResultScene::DrawCursor()
{
	static float screenWidth = (float)GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	static float screenHeight = (float)GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ

	Transform aim;
	aim.scale_.x = 0.5f;
	aim.scale_.y = 0.5f;
	aim.scale_.z = 0.5f;
	aim.position_ = Input::GetMousePositionSub();
	aim.position_ = { aim.position_.x / screenWidth, -aim.position_.y / screenHeight , 0 };

	Image::SetTransform(hPict_[1], aim);
	Image::Draw(hPict_[1]);
}

void ResultScene::ButtonInitializ()
{
	const int button = sizeof(tbl) / sizeof(tbl[0]);
	for (int i = 0; i < button; i++) {
		Button* pButton = nullptr;
		pButton = Instantiate<Button>(this);
		pButton->SetValue(tbl[i].x, tbl[i].y, tbl[i].width, tbl[i].height, tbl[i].name);
		pButton->SetAlpha_(255);
		pButton->SetFrameAlpha_(255);
	}
}

void ResultScene::CheckButtonPressed()
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

			if (na == "ReturnTitle") {
				SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
				pSceneManager->ChangeScene(SCENE_ID_TITLE);
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
					pButton->SetFrameAlpha_(10);

				}

				break;
			}
		}
	}
}