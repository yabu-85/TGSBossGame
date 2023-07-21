#include "TitleScene.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Text.h"
#include "Engine/SceneManager.h"

//コンストラクタ
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hPict_{-1, -1, -1}, pText_(nullptr)
{
}

//初期化
void TitleScene::Initialize()
{
	hPict_[0] = Image::Load("Title.png");
	assert(hPict_[0] >= 0);

	hPict_[1] = Image::Load("Start.png");
	assert(hPict_[1] >= 0);

	hPict_[2] = Image::Load("cross.png");
	assert(hPict_[2] >= 0);

	pText_ = new Text;
	pText_->Initialize();

}

//更新
void TitleScene::Update()
{
}

//描画
void TitleScene::Draw()
{
	static int screenWidth = GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	static int screenHeight = GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ

	static XMFLOAT3 mousePos = { 0,0,0 };
	mousePos.x += Input::GetMouseMove().x * 2.5f;
	mousePos.y += Input::GetMouseMove().y * 2.5f;
	if (screenWidth < abs(mousePos.x)) mousePos.x = screenWidth * (mousePos.x > 0.0 ? 1 : -1);
	if (screenHeight < abs(mousePos.y)) mousePos.y = screenHeight * (mousePos.y > 0.0 ? 1 : -1);

	//背景のPNG
	pText_->Draw(30, 30, mousePos.x);
	pText_->Draw(30, 70, mousePos.y);

	Image::SetTransform(hPict_[0], transform_);
	Image::Draw(hPict_[0]);

	//StartのPNG

	//変えるとこ x1,y1で場所を決めてる
	static int x1 = -480;
	static int x2 = x1 * -1;
	static int y1 = -135;
	static int y2 = y1 * -1;

	Transform start = transform_;

	if (mousePos.x > x1 && mousePos.x < x2 &&
		mousePos.y > y1 && mousePos.y < y2) {
		start.scale_.x = 1.25f;
		start.scale_.y = 1.25f;

		if (Input::IsMouseButtonDown(0)) {
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_PLAY);
		}
	}

	Image::SetTransform(hPict_[1], start);
	Image::Draw(hPict_[1]);

	//------カーソル----------
	Transform aim;
	aim.scale_.x = 0.5f;
	aim.scale_.y = 0.5f;
	aim.scale_.z = 0.5f;
	aim.position_ = mousePos;

	aim.position_ = { aim.position_.x / screenWidth ,-aim.position_.y / screenHeight ,0 };

	Image::SetTransform(hPict_[2], aim);
	Image::Draw(hPict_[2]);

}

//開放
void TitleScene::Release()
{
}