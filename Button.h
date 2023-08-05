#pragma once
#include "Engine/GameObject.h"

class TitleScene;

class Button : public GameObject
{
	int hPict_[2];
	int alpha_;				//�����x
	float width_;			//�{�^���̕�
	float height_;			//�{�^���̍���
	bool isButtonInactive_; //�{�^�����������m���Ȃ�
	std::string name_;		//�{�^���ɕ\������Png��
	XMFLOAT3 widePos_;		//-1�`1����Ȃ�,-1280�`1280�Ƃ��̍��W

public:
	Button(GameObject* parent);
	~Button();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetValue(float x, float y, float w, float h, std::string n);
	void SetAlpha_(int i) { alpha_ = i; }; //0�i�����j�`255�i�\���j
	void SetActive(bool b) { isButtonInactive_ = b; };
	bool IsButtonClicked();
	std::string GetName() { return name_; };
};

