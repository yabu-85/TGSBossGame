#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class TitleScene : public GameObject
{
	Text* pText_;		//

	int hPict_[3];		//画像番号
	bool disp_;			//表示するかどうか
	XMFLOAT3 mousePos_;	//マウス座標

	void DrawCursor();
	void ButtonInitializ();
	void CheckButtonPressed();	//ボタン押されたか調べる

public:

	TitleScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	XMFLOAT3 GetMousePos() { return mousePos_; };
};