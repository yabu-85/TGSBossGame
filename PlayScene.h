#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class Timer;
class Player;

class PlayScene  : public GameObject
{
	int hPict_;		//�摜�ԍ�

	Timer* pTimer_;
	Player* pPlayer_;

public:
	PlayScene (GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//PlayScene�̂��ׂĂ�Active�𐧌䂷��
	void SetObjectActive(bool _active);

	void ResetGame();

};