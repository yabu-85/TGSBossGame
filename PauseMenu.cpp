#include "PauseMenu.h"
#include "Engine/Model.h"

PauseMenu::PauseMenu(GameObject* parent)
	:GameObject(parent, "PauseMenu"), hModel_{-1,-1,-1,-1,-1}
{
}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::Initialize()
{
	//モデルデータのロード
	hModel_[0] = Model::Load("Human.fbx");
	assert(hModel_[0] >= 0);
	Model::SetAnimFrame(hModel_[0], 0, 170, 1);

	hModel_[1] = Model::Load("hako.fbx");
	assert(hModel_[1] >= 0);

	hModel_[2] = Model::Load("eee.fbx");
	assert(hModel_[2] >= 0);
	Model::SetAnimFrame(hModel_[2], 0, 120, 1);

	hModel_[3] = Model::Load("ccc.fbx");
	assert(hModel_[3] >= 0);


}

void PauseMenu::Update()
{
}

void PauseMenu::Draw()
{
	Transform pos0 = transform_;
	pos0.position_.x = 15.0f;
	pos0.position_.z = 15.0f;
	Model::SetTransform(hModel_[0], pos0);
	Model::Draw(hModel_[0]);

	Transform pos1 = transform_;
	pos1.position_.x = 5.0f;
	pos1.position_.z = 15.0f;
	Model::SetTransform(hModel_[1], pos1);
	Model::Draw(hModel_[1]);

	Transform pos2 = transform_;
	pos2.position_.x = 15.5f;
	pos2.position_.z = 8.0f;
	pos2.rotate_.y = 180.0f;
	Model::SetTransform(hModel_[2], pos2);
	Model::Draw(hModel_[2]);

	Transform pos3 = transform_;
	pos3.position_.x = 5.0f;
	pos3.position_.y = 3.0f;
	pos3.position_.z = 7.0f;
	Model::SetTransform(hModel_[3], pos3);
	Model::Draw(hModel_[3]);

}

void PauseMenu::Release()
{
}
