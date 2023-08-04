#include "TitleScene.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include "Logo.h"
#include "Button.h"
#include "Engine/Text.h"
#include "Engine/Input.h"
#include "ExitMenu.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hPict_{-1, -1, -1}, pText_(nullptr), disp_(false), mousePos_{0,0,0}
{
}

struct ButtonInfoTitle {
	float x;
	float y;
	float width;
	float height;
	std::string name;
}tbl[] = {
	{0.0f, 0.0f, 100.0f, 100.0f, "Start"},
	{0.0f, -200.0f, 100.0f, 100.0f, "Setting"},
	{0.0f, -400.0f, 100.0f, 100.0f, "Quit"},

};

void TitleScene::Initialize()
{
	//test�p
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
		//���S��\��
		Instantiate<Logo>(this);

	} else {
		//���S�\�����Ȃ�
		disp_ = true;
		ButtonInitializ();
	}

}

void TitleScene::Update()
{
	if (disp_) {
		CheckButtonPressed();

	}
	else {
		//Logo�̏���
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
		Image::SetAlpha(hPict_[0], 0);
		Transform pos1 = transform_;
		pos1.scale_.x = 1.6f;
		pos1.scale_.y = 1.2f;
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
}

//private�֐������[�[�[�[�[�[�[�[�[�[�[�[�[

void TitleScene::DrawCursor()
{
	static int screenWidth = GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//�X�N���[���̕�
	static int screenHeight = GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//�X�N���[���̍���
	
	mousePos_.x += Input::GetMouseMove().x * 2.5f;
	mousePos_.y += Input::GetMouseMove().y * 2.5f;
	if (screenWidth < abs(mousePos_.x)) mousePos_.x = screenWidth * (mousePos_.x > 0.0 ? 1 : -1);
	if (screenHeight < abs(mousePos_.y)) mousePos_.y = screenHeight * (mousePos_.y > 0.0 ? 1 : -1);

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
	const int button = sizeof(tbl) / sizeof(tbl[0]);
	for (int i = 0; i < button; i++) {
		Button* pButton = nullptr;
		pButton = Instantiate<Button>(this);
		pButton->SetValue(tbl[i].x, tbl[i].y, tbl[i].width, tbl[i].height, tbl[i].name);
	}
}

void TitleScene::CheckButtonPressed()
{
	if (!Input::IsMouseButtonDown(0))
		return;

	std::list<GameObject*>* gs = GetChildList();
	for (GameObject* obj : *gs) {
		if (obj->GetObjectName() != "Button") {
			continue;
		}

		Button* pButton = (Button*)obj;
		if (pButton->IsButtonClicked()){
			std::string na = pButton->GetName();
			
			if(na == "Start") {
				SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
				pSceneManager->ChangeScene(SCENE_ID_PLAY);
			}
			else if (na == "Setting") {
				mousePos_.y += 200.0f;
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
