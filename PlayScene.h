#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class PlayScene  : public GameObject
{
	int count_;				//��
	Text* pText_;			//�e�L�X�g
	int clearTime_;			//�N���A����
public:
	PlayScene (GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�^�C�}�[
	void TimeProcess();
};