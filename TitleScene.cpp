#include "TitleScene.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include "Logo.h"
#include "ButtonFactory.h"
#include "Button.h"
#include "Engine/Text.h"
#include "Engine/Input.h"
#include "ExitMenu.h"
#include "Stageselect.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hPict_{-1, -1, -1}, pText_(nullptr), disp_(false), mousePos_{0,0,0},
	pButtonFactory_(nullptr)
{
}

void TitleScene::Initialize()
{
	//test用
	pText_ = new Text;
	pText_->Initialize();

	const char* fileName[] = { "Png/TitleBG.png", "Png/cross.png", "Png/Title.png"};
	const int png = sizeof(fileName) / sizeof(fileName[0]);
	for (int i = 0; i < png; i++) {
		hPict_[i] = Image::Load(fileName[i]);
		assert(hPict_[i] >= 0);
	}

	disp_ = false;

	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	bool ab = !pSceneManager->IsLogoShown();
	if (ab) {
		//ロゴを表示
		pSceneManager->SetLogoShown(true);
		Instantiate<Logo>(this);

	} else {
		//ロゴ表示しない
		disp_ = true;
		pButtonFactory_ = Instantiate<ButtonFactory>(this);
		pButtonFactory_->ButtonCreate(0.0f, 0.0f, 1.0f, 1.0f, "Start");
		pButtonFactory_->ButtonCreate(0.0f, -300.0f, 1.0f, 1.0f, "Quit");
		pButtonFactory_->SetAlpha(200);
		pButtonFactory_->SetFrameAlpha(200);
		pButtonFactory_->SetBlendMode(0);

	}
}

void TitleScene::Update()
{
	if (!disp_) {
		//Logoの処理
		Logo* pLogo = (Logo*)FindObject("Logo");
		if (pLogo->IsEndDraw()) {
			disp_ = true;
			pLogo->KillMe();
			pButtonFactory_ = Instantiate<ButtonFactory>(this);
			pButtonFactory_->ButtonCreate(0.0f, 0.0f, 1.0f, 1.0f, "Start");
			pButtonFactory_->ButtonCreate(0.0f, -300.0f, 1.0f, 1.0f, "Quit");
			pButtonFactory_->SetAlpha(200);
			pButtonFactory_->SetFrameAlpha(200);
			pButtonFactory_->SetBlendMode(0);

		}
	}

	if (pButtonFactory_->CheckButtonPressed() == "Start") {
		Instantiate<StageSelect>(this);

		pButtonFactory_->SetActive(false);
		pButtonFactory_->SetAlpha(10);
		pButtonFactory_->SetFrameAlpha(10);

	}
	else if (pButtonFactory_->CheckButtonPressed() == "Quit") {
		Instantiate<ExitMenu>(this);

		pButtonFactory_->SetActive(false);
		pButtonFactory_->SetAlpha(10);
		pButtonFactory_->SetFrameAlpha(10);
		
	}
}

void TitleScene::Draw()
{
	if (disp_) {
		Transform pos1 = transform_;
		pos1.scale_.x = 0.8f;
		pos1.scale_.y = 0.8f;
		Image::SetTransform(hPict_[0], pos1);
		Image::Draw(hPict_[0]);

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
	SAFE_DELETE(pText_);
}

//private関数たちーーーーーーーーーーーーー

void TitleScene::DrawCursor()
{
	static float screenWidth = (float)GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	static float screenHeight = (float)GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ

	Transform aim;
	aim.scale_.x = 0.5f;
	aim.scale_.y = 0.5f;
	aim.scale_.z = 0.5f;
	aim.position_ = Input::GetMousePositionSub();
	aim.position_ = { aim.position_.x / screenWidth, -aim.position_.y / screenHeight , 0};

	Image::SetTransform(hPict_[1], aim);
	Image::Draw(hPict_[1]);
}
