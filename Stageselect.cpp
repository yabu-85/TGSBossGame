#include "Stageselect.h"
#include "Engine/Input.h"
#include "ButtonFactory.h"
#include "Button.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include <future>

StageSelect::StageSelect(GameObject* parent)
	:GameObject(parent, "StageSelect"), hPict_(-1), time_(0), pButtonFactory_(nullptr), hSound_(-1)
{
}

StageSelect::~StageSelect()
{
}

void StageSelect::Initialize()
{
	hPict_ = Image::Load("Png/StageSelect.png");
	assert(hPict_ >= 0);

	//Defaultの上にAddを置くと気持ち明るくなる
	//から画像が重なるときは、ボタンを作る順番でも描画結果が異なる
	//0 = DefaultModeで重ねると暗くなる設定（多分
	//1 = AddModeで加算合成、重ねると明るくなる設定

	pButtonFactory_ = Instantiate<ButtonFactory>(this);
	pButtonFactory_->ButtonCreate(-1000.0f, -200.0f, 0.4f, 0.4f, "Clear", "Stage01");
	pButtonFactory_->ButtonCreate(0.0f, -200.0f, 0.4f, 0.4f, "Over", "Stage02");
	pButtonFactory_->ButtonCreate(0.0f, -700.0f, 1.0f, 1.0f, "Back");
	pButtonFactory_->SetAlpha(200);
	pButtonFactory_->SetFrameAlpha(200);
	pButtonFactory_->SetBlendMode(0);

	//サウンドデータのロード
	hSound_ = Audio::Load("Sound/EnterStage.wav", false, 3);
	hSound2_ = Audio::Load("Sound/EnterCursor.wav", false, 3);
	assert(hSound_ >= 0);

}

void StageSelect::Update()
{
	time_++;
	if (time_ < 2) return;

	if (pButtonFactory_->CheckButtonPressed() == "Clear") {
		Audio::Play(hSound_);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //待機

		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
	else if (pButtonFactory_->CheckButtonPressed() == "Over") {
		Audio::Play(hSound_);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //待機
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
		pB->SetAlpha(200);
		pB->SetFrameAlpha(200);

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
