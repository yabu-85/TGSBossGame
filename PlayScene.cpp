#include "PlayScene.h"
#include "Timer.h"
#include "Player.h"
#include "Stage.h"
#include "PauseMenu.h"
#include "Engine/Input.h"
#include "ExitMenu.h"

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
	pPlayer->SetActiveWithDelay(true);

	//���e�L�X�g�͑O�ʂɂȂ�悤�ɕ`��
	//�^�C�}�[�ݒ�
	Timer* pTimer = Instantiate<Timer>(this);
	pTimer->SetLimit(15);
}

//�X�V
void PlayScene ::Update()
{
	if (Input::IsKeyDown(DIK_E)) {
		Instantiate<PauseMenu>(this);
		Player* pPlayer = (Player*)FindObject("Player");
		pPlayer->SetActive(false);
	}

	//�^�C�}�[����
	TimeProcess();
}

//�`��
void PlayScene ::Draw()
{
}

//�J��
void PlayScene ::Release()
{
}

//�^�C�}�[
void PlayScene::TimeProcess()
{
	//�^�C�}�[�N��
	if (Input::IsKeyDown(DIK_W))
	{
		Timer* t = (Timer*)FindObject("Timer");
		t->Start();
	}
	//�^�C�}�[��~
	if (Input::IsKeyDown(DIK_Y))
	{
		Timer* t = (Timer*)FindObject("Timer");
		t->Stop();
	}
}