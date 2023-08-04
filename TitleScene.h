#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class TitleScene : public GameObject
{
	int hPict_[3];		//�摜�ԍ�
	Text* pText_;		//
	bool disp_;			//�\�����邩�ǂ���
	XMFLOAT3 mousePos_;	//�}�E�X���W

	void DrawCursor();
	void ButtonInitializ();
	void CheckButtonPressed();	//�{�^�������ꂽ�����ׂ�

public:

	TitleScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	XMFLOAT3 GetMousePos() { return mousePos_; };
};