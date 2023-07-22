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
	hModel_[0] = Model::Load("hako.fbx");
	assert(hModel_[0] >= 0);

	hModel_[1] = Model::Load("aaa.fbx");
	assert(hModel_[1] >= 0);
	Model::SetAnimFrame(hModel_[1], 0, 170, 1);

	hModel_[2] = Model::Load("monkey.fbx");
	assert(hModel_[2] >= 0);

	hModel_[3] = Model::Load("huu.fbx");
	assert(hModel_[3] >= 0);
	Model::SetAnimFrame(hModel_[1], 0, 170, 1);

	hModel_[4] = Model::Load("ccc.fbx");
	assert(hModel_[4] >= 0);
	Model::SetAnimFrame(hModel_[4], 0, 120, 1);


}

void PauseMenu::Update()
{
}

void PauseMenu::Draw()
{
	Model::SetTransform(hModel_[0], transform_);
	Model::Draw(hModel_[0]);

	Transform pos1 = transform_;
	pos1.position_.x = 3.0f;
	Model::SetTransform(hModel_[1], pos1);
	Model::Draw(hModel_[1]);

	Transform pos2 = transform_;
	pos2.position_.x = 6.5f;
	pos2.scale_.x = 2.0f;
	pos2.scale_.y = 2.0f;
	pos2.scale_.z = 2.0f;
	Model::SetTransform(hModel_[2], pos2);
	Model::Draw(hModel_[2]);

	Transform pos3 = transform_;
	pos3.position_.x = 10.0f;
	Model::SetTransform(hModel_[3], pos3);
	Model::Draw(hModel_[3]);

	Transform pos4 = transform_;
	pos4.position_.x = 14.5f;
	pos4.position_.y = 3.0f;
	Model::SetTransform(hModel_[4], pos4);
	Model::Draw(hModel_[4]);

}

void PauseMenu::Release()
{
}
