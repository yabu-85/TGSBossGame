#pragma once
#include "Engine/SceneManager.h"

enum AUDIO_ID
{
	AUDIO_ENTERCURSOR = 0,
	AUDIO_POINTCURSOR,

	AUDIO_MAX,
};

namespace AudioManager {

	void Initialize(SCENE_ID i);
	void Release();

	//�����AUDIOID��łĂΎg����
	//Button��Title�Ŏ����Ɏg���Ă�
	void PlaySoundMa(AUDIO_ID i);
	void StopSoundMa(AUDIO_ID i);

	//���̐��������������Ă�����Ă�
	void PlaySoundEnterCurSor();
	void StopSoundEnterCursor();
	void PlaySoundPointCurSor();
	void StopSoundPointCursor();


};