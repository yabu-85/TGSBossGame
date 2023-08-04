#pragma once
#include "Engine/GameObject.h"

class TitleScene;

class Button : public GameObject
{
	int hPict_[2];	
	float width_;		// �{�^���̕�
	float height_;		// �{�^���̍���
	std::string name_;	// �{�^���ɕ\������Png��
	TitleScene* pTitleScene_;
	XMFLOAT3 widePos_;	//-1�`1����Ȃ�,-1280�`1280�Ƃ��̍��W
	
public:
	Button(GameObject* parent);
	~Button();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetValue(float x, float y, float w, float h, std::string n);
};

