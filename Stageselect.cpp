#include "Stageselect.h"
#include "Engine/Input.h"
#include "ButtonFactory.h"
#include "Button.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"

StageSelect::StageSelect(GameObject* parent)
	:GameObject(parent, "StageSelect"), hPict_(-1), time_(0), pButtonFactory_(nullptr)
{
}

StageSelect::~StageSelect()
{
}

void StageSelect::Initialize()
{
	hPict_ = Image::Load("StageSelect.png");
	assert(hPict_ >= 0);

	pButtonFactory_ = Instantiate<ButtonFactory>(this);
	pButtonFactory_->ButtonCreate(-1000.0f, -200.0f, 0.5f, 0.5f, "Clear", "Stage01");
	pButtonFactory_->ButtonCreate(0.0f, -200.0f, 0.6f, 1.2f, "Over");
	pButtonFactory_->ButtonCreate(1000.0f, -200.0f, 0.6f, 1.2f, "Clear", "Team");
	pButtonFactory_->ButtonCreate(0.0f, -500.0f, 1.0f, 1.0f, "Back", "GameOver");

}

void StageSelect::Update()
{
	time_++;
	if (time_ < 2) return;

	if (pButtonFactory_->CheckButtonPressed() == "Clear") {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
	else if (pButtonFactory_->CheckButtonPressed() == "Over") {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
	else if (pButtonFactory_->CheckButtonPressed() == "Clear") {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
	else if (pButtonFactory_->CheckButtonPressed() == "Back") {
		GameObject* gs2 = GetParent()->FindObject("ButtonFactory");
		ButtonFactory* pB = (ButtonFactory*)gs2;
		pB->SetActive(true);
		pB->SetAlpha(100);
		pB->SetFrameAlpha(100);

		KillMe();
		return;
	}

}

void StageSelect::Draw()
{
	Transform pos;
	pos.position_.y = 0.1f;
	pos.scale_ = XMFLOAT3(0.5, 0.5f, 0.5f);
	Image::SetTransform(hPict_, pos);
	Image::Draw(hPict_);
}

void StageSelect::Release()
{
}
