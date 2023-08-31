#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"
#include "Engine/Audio.h"
class ButtonFactory;

class TitleScene : public GameObject
{
	Text* pText_;		//�e�L�X�g
	int hSound_;		//�T�E���h
	int hPict_[3];		//�摜�ԍ�
	bool disp_;			//�\�����邩�ǂ���
	XMFLOAT3 mousePos_;	//�}�E�X���W
	ButtonFactory* pButtonFactory_;

	void DrawCursor();

public:

	TitleScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	XMFLOAT3 GetMousePos() { return mousePos_; };
};