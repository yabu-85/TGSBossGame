#include "Stageselect.h"
#include "Engine/Input.h"
#include "Button.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"

StageSelect::StageSelect(GameObject* parent)
	:GameObject(parent, "StageSelect"), hPict_(-1), time_(0)
{
}

struct ButtonInfoStage {
	float x;
	float y;
	float width;
	float height;
	std::string name;
}tbl[] = {
	{-1000.0f, -200.0f, 0.6f, 1.0f, "Clear"},
	{0.0f, -200.0f, 0.6f, 1.0f, "Over"},
	{1000.0f, -200.0f, 0.6f, 1.0f, "Team"},
	{0.0f, -500.0f, 1.0f, 1.0f, "Back"},

};

StageSelect::~StageSelect()
{
}

void StageSelect::Initialize()
{
	hPict_ = Image::Load("StageSelect.png");
	assert(hPict_ >= 0);

	ButtonInitializ();
}

void StageSelect::Update()
{
	if (time_ > 2)
		CheckButtonPressed();
	time_++;

}

void StageSelect::Draw()
{
	Transform pos;
	pos.position_.y = 0.0f;
	pos.scale_ = XMFLOAT3(0.5, 0.5f, 0.5f);
	Image::SetTransform(hPict_, pos);
	Image::Draw(hPict_);
}

void StageSelect::Release()
{
}



void StageSelect::CheckButtonPressed()
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

			if (na == "Clear") {
				SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
				pSceneManager->ChangeScene(SCENE_ID_PLAY);
			}
			else if (na == "Over") {
				SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
				pSceneManager->ChangeScene(SCENE_ID_PLAY);
			}
			else if (na == "Title") {
				SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
				pSceneManager->ChangeScene(SCENE_ID_PLAY);
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

void StageSelect::ButtonInitializ()
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