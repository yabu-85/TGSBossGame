#include "PauseMenu.h"
#include "Engine/Model.h"

PauseMenu::PauseMenu(GameObject* parent)
	:GameObject(parent, "PauseMenu"), hModel_(-1)
{
}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Test.fbx");
	assert(hModel_ >= 0);
}

void PauseMenu::Update()
{
}

void PauseMenu::Draw()
{
}

void PauseMenu::Release()
{
}
