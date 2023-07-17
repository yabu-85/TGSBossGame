#include "TitleScene.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Text.h"
#include "Engine/SceneManager.h"

//コンストラクタ
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hPict_{-1, -1}, pText_(nullptr)
{
}

//初期化
void TitleScene::Initialize()
{
	hPict_[0] = Image::Load("Title.png");
	assert(hPict_[0] >= 0);

	hPict_[1] = Image::Load("Start.png");
	assert(hPict_[1] >= 0);

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
	//背景のPNG
	pText_->Draw(30, 30, Input::GetMousePosition().x);
	pText_->Draw(30, 70, Input::GetMousePosition().y);

	Image::SetTransform(hPict_[0], transform_);
	Image::Draw(hPict_[0]);


	//StartのPNG
	int screenWidth = GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	int screenHeight = GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ

	//変えるとこ x1,y1で場所を決めてる　x=380、y=300がデフォ
	static int x1 = 380;
	static int x2 = screenWidth + (x1 - (380 * 2));
	static int y1 = 300;
	static int y2 = screenHeight + (y1 - (300 * 2));

	Transform start = transform_;

	if (Input::GetMousePosition().x > x1 && Input::GetMousePosition().x < x2 &&
		Input::GetMousePosition().y > y1 && Input::GetMousePosition().y < y2) {
		start.scale_.x = 1.25f;
		start.scale_.y = 1.25f;

		if (Input::IsMouseButtonDown(0)) {
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_PLAY);
		}
	}

	Image::SetTransform(hPict_[1], start);
	Image::Draw(hPict_[1]);

}

//開放
void TitleScene::Release()
{
}