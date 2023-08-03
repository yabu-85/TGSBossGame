#include "TitleScene.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include "Logo.h"
#include "Button.h"

#include "Engine/Text.h"
#include "Engine/Input.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hPict_{-1, -1, -1}, pText_(nullptr), disp_(false), mousePos_{0,0,0}
{
}

struct ButtonInfo {
	int x;
	int y;
	int width;
	int height;
	std::string name;
}tbl[] = {
	{0,0,100,100,"Start"},
	{0,-200,100,100,"Setting"},
	{0,-400,100,100,"Quit"},

};

void TitleScene::Initialize()
{
	//test用
	pText_ = new Text;
	pText_->Initialize();

	const char* fileName[] = { "Title.png", "cross.png", "GameTitle.png"};
	const int png = sizeof(fileName) / sizeof(fileName[0]);

	for (int i = 0; i < png; i++) {
		hPict_[i] = Image::Load(fileName[i]);
		assert(hPict_[i] >= 0);
	}

	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (!pSceneManager->IsLogoShown()) {
		//ロゴを表示
		Instantiate<Logo>(this);

	} else {
		//ロゴ表示しない
		disp_ = true;
		ButtonInitializ();
	}
}

void TitleScene::Update()
{
	if (disp_) {
		if (Input::IsMouseButtonDown(0)) {
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_PLAY);
		}

	}
	else {
		//Logoの処理
		Logo* pLogo = (Logo*)FindObject("Logo");
		if (pLogo->IsEndDraw()) {
			disp_ = true;
			pLogo->KillMe();

			ButtonInitializ();
		}
	}
}

void TitleScene::Draw()
{
	if (disp_) {
	//	Image::SetTransform(hPict_[0], transform_);
	//	Image::Draw(hPict_[0]);

		DrawCursor();

		Transform pos = transform_;
		pos.position_.y = 0.5f;
		pos.scale_.x = 1.2f;
		pos.scale_.y = 1.2f;
		Image::SetTransform(hPict_[2], pos);
		Image::Draw(hPict_[2]);

	}

}

void TitleScene::Release()
{
}

void TitleScene::DrawCursor()
{
	static int screenWidth = GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	static int screenHeight = GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ
	
	mousePos_.x += Input::GetMouseMove().x * 2.5f;
	mousePos_.y += Input::GetMouseMove().y * 2.5f;
	if (screenWidth < abs(mousePos_.x)) mousePos_.x = screenWidth * (mousePos_.x > 0.0 ? 1 : -1);
	if (screenHeight < abs(mousePos_.y)) mousePos_.y = screenHeight * (mousePos_.y > 0.0 ? 1 : -1);

	//背景のPNG
	pText_->Draw(30, 30, mousePos_.x);
	pText_->Draw(30, 70, mousePos_.y);

	Transform aim;
	aim.scale_.x = 0.5f;
	aim.scale_.y = 0.5f;
	aim.scale_.z = 0.5f;
	aim.position_ = mousePos_;

	aim.position_ = { aim.position_.x / screenWidth ,-aim.position_.y / screenHeight ,0 };

	Image::SetTransform(hPict_[1], aim);
	Image::Draw(hPict_[1]);
}

void TitleScene::ButtonInitializ()
{
	const int button = 3;
	for (int i = 0; i < button; i++) {
		Button* pButton = nullptr;
		pButton = Instantiate<Button>(this);
		pButton->SetValue(tbl[i].x, tbl[i].y, tbl[i].width, tbl[i].height, tbl[i].name);
	}
}