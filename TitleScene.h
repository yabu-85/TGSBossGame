#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class ButtonFactory;

class TitleScene : public GameObject
{
	Text* pText_;		//テキスト
	int hPict_[3];		//画像番号
	bool disp_;			//表示するかどうか
	XMFLOAT3 mousePos_;	//マウス座標
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