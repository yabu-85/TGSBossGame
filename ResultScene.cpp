#include "ResultScene.h"
#include "ExitMenu.h"	

//�R���X�g���N�^
ResultScene::ResultScene(GameObject* parent)
	: GameObject(parent, "ResultScene")
{
}

//������
void ResultScene::Initialize()
{
	Instantiate<ExitMenu>(this);
}

//�X�V
void ResultScene::Update()
{
}

//�`��
void ResultScene::Draw()
{
}

//�J��
void ResultScene::Release()
{
}