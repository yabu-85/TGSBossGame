#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class PlayScene  : public GameObject
{
	int count_;				//回数
	Text* pText_;			//テキスト
	int clearTime_;			//クリア時間
public:
	PlayScene (GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//タイマー
	void TimeProcess();
};