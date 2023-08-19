#include "PlayScene.h"
#include "Timer.h"
#include "Player.h"
#include "Stage.h"
#include "PauseMenu.h"
#include "Engine/Input.h"
#include "ExitMenu.h"
#include "ObstacleManager.h"

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), pText_(nullptr), clearTime_(0), count_(0)
{
}

//������
void PlayScene::Initialize()
{
	Stage* pStage = Instantiate<Stage>(this);
	Player* pPlayer = Instantiate<Player>(this);
	Instantiate<ObstacleManager>(this);

	pPlayer->SetPosition(pStage->GetPlaPos());
	pPlayer->SetActiveWithDelay(true);

	//���e�L�X�g�ނ̓��f�������O�ʂɕ`��(��ɌĂ�)
	Timer* pTimer = Instantiate<Timer>(this);
	pTimer->SetLimit(5);		//�^�C�}�[��ݒ�
	pText_ = new Text;			//�e�L�X�g
	pText_->Initialize();
}

//�X�V
void PlayScene::Update()
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
	//�`��
	pText_->SetScale(3.0f);
	pText_->Draw(300, 200, clearTime_);
}

//�J��
void PlayScene ::Release()
{
}

//�^�C�}�[
void PlayScene::TimeProcess()
{
	Timer* pTimer = (Timer*)FindObject("Timer");

	//�^�C�}�[���N��(���񉟉����̂�)
	if (Input::IsKeyDown(DIK_W)&& count_==0)
	{
		pTimer->Start();
		count_++;
	}

	//�Q�[�����N���A�����ꍇ
	if (Input::IsKeyDown(DIK_Y))
	{
		//�^�C�}�[��~
		pTimer->Stop();
		clearTime_ = pTimer->GetTime();////////�N���A�^�C���擾�m�F�p���������Ȃ��ăI�[�o�[�V�[���Ŏg���Ǝv��
	}
	
	//�^�C�}�[���I�������ꍇ
	if (pTimer->IsFinished())
	{
		////////(�Q�[���I�[�o�[)�_�E���A�j���Đ����Ĥ�V�[���؂�ւ��Ƃ����Ƃ���
		//Player* pPlayer = (Player*)FindObject("Player");
		//pPlayer->KillMe();
	}
}