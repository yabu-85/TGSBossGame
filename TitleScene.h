#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class TitleScene : public GameObject
{
	int hPict_[2];		//�摜�ԍ�
	Text* pText_;		//
	bool disp_;			//�\�����邩�ǂ���
	XMFLOAT3 mousePos_;	//�}�E�X���W

	void DrawCursor();
	void ButtonInitializ();

public:

	TitleScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};