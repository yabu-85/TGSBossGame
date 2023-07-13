#include "TitleScene.h"
#include "Player.h"
#include "Stage.h"

//�R���X�g���N�^
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

//������
void TitleScene::Initialize()
{
	Instantiate<Player>(this);
	Instantiate<Stage>(this);
}

//�X�V
void TitleScene::Update()
{
}

//�`��
void TitleScene::Draw()
{
}

//�J��
void TitleScene::Release()
{
}