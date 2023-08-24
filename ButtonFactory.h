#pragma once
#include "Engine/GameObject.h"
#include <vector>

class Button;

class ButtonFactory :
    public GameObject
{
	std::vector<Button*> buttons_;

public:
	ButtonFactory(GameObject* parent);
	~ButtonFactory();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//すべてのボタンが押されているか調べる
	//引数：ボタンの名前
	std::string CheckButtonPressed();

	//ボタン生成
	void ButtonCreate(float x, float y, float w, float h, std::string n);
	//ボタン生成
	void ButtonCreate(float x, float y, float w, float h, std::string n, std::string f);

	void SetAlpha(int i);
	void SetFrameAlpha(int i);
	void SetActive(bool b);

	//Default=0, Add = 1
	void SetBlendMode(int mode, std::string name);
};

