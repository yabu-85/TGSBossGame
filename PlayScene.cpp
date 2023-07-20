#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
}

//������
void PlayScene::Initialize()
{
	Stage* pStage = Instantiate<Stage>(this);
	Player* pPlayer = Instantiate<Player>(this);

	pPlayer->SetPosition(pStage->GetPlaPos());
}

//�X�V
void PlayScene ::Update()
{
}

//�`��
void PlayScene ::Draw()
{
}

//�J��
void PlayScene ::Release()
{
}