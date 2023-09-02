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

	//これはAUDIOIDを打てば使える
	//ButtonとTitleで試しに使ってる
	void PlaySoundMa(AUDIO_ID i);
	void StopSoundMa(AUDIO_ID i);

	//音の数だけこれを作ってそれを呼ぶ
	void PlaySoundEnterCurSor();
	void StopSoundEnterCursor();
	void PlaySoundPointCurSor();
	void StopSoundPointCursor();


};