#pragma once
#include "Engine/GameObject.h"

struct ButtonInfo;

class Button : public GameObject
{
	int hPict_;	
	float xPos_;		// �{�^����X���W
	float yPos_;		// �{�^����Y���W
	float width_;		// �{�^���̕�
	float height_;		// �{�^���̍���
	std::string name_;	// �{�^���ɕ\������Png�̖��O

public:
	Button(GameObject* parent);
	~Button();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetValue(int x, int y, int w, int h, std::string n);
};

