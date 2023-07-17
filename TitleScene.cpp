#include "TitleScene.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Text.h"
#include "Engine/SceneManager.h"

//�R���X�g���N�^
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hPict_{-1, -1}, pText_(nullptr)
{
}

//������
void TitleScene::Initialize()
{
	hPict_[0] = Image::Load("Title.png");
	assert(hPict_[0] >= 0);

	hPict_[1] = Image::Load("Start.png");
	assert(hPict_[1] >= 0);

	pText_ = new Text;
	pText_->Initialize();

}

//�X�V
void TitleScene::Update()
{
}

//�`��
void TitleScene::Draw()
{
	//�w�i��PNG
	pText_->Draw(30, 30, Input::GetMousePosition().x);
	pText_->Draw(30, 70, Input::GetMousePosition().y);

	Image::SetTransform(hPict_[0], transform_);
	Image::Draw(hPict_[0]);


	//Start��PNG
	int screenWidth = GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//�X�N���[���̕�
	int screenHeight = GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//�X�N���[���̍���

	//�ς���Ƃ� x1,y1�ŏꏊ�����߂Ă�@x=380�Ay=300���f�t�H
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

//�J��
void TitleScene::Release()
{
}