#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class TitleScene : public GameObject
{
	int hPict_[2];		//画像番号
	Text* pText_;		//
	bool disp_;			//表示するかどうか
	XMFLOAT3 mousePos_;	//マウス座標

	void DrawCursor();
	void ButtonInitializ();

public:

	TitleScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};