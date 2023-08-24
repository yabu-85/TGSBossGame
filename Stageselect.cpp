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

	//Default�̏��Add��u���ƋC�������邭�Ȃ�
	//����摜���d�Ȃ�Ƃ��́A�{�^������鏇�Ԃł��`�挋�ʂ��قȂ�
	//0 = DefaultMode�ŏd�˂�ƈÂ��Ȃ�ݒ�i����
	//1 = AddMode�ŉ��Z�����A�d�˂�Ɩ��邭�Ȃ�ݒ�

	pButtonFactory_ = Instantiate<ButtonFactory>(this);
	pButtonFactory_->ButtonCreate(-1000.0f, -200.0f, 0.4f, 0.4f, "Clear", "Stage01");
	pButtonFactory_->ButtonCreate(0.0f, -200.0f, 0.4f, 0.4f, "Over", "Stage01");
	pButtonFactory_->ButtonCreate(1000.0f, -200.0f, 0.4f, 0.4f, "Quit", "Stage01");
	pButtonFactory_->ButtonCreate(0.0f, -700.0f, 1.0f, 1.0f, "Back");
	pButtonFactory_->SetAlpha(200);
	pButtonFactory_->SetFrameAlpha(200);
	pButtonFactory_->SetBlendMode(0);

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
	pos.position_.y = 0.2f;
	pos.scale_ = XMFLOAT3(0.5, 0.5f, 0.5f);
	Image::SetTransform(hPict_, pos);
	Image::Draw(hPict_);
}

void StageSelect::Release()
{
}
