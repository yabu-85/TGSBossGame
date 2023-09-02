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

	//‚±‚ê‚ÍAUDIOID‚ğ‘Å‚Ä‚Îg‚¦‚é
	//Button‚ÆTitle‚Å‚µ‚Ég‚Á‚Ä‚é
	void PlaySoundMa(AUDIO_ID i);
	void StopSoundMa(AUDIO_ID i);

	//‰¹‚Ì”‚¾‚¯‚±‚ê‚ğì‚Á‚Ä‚»‚ê‚ğŒÄ‚Ô
	void PlaySoundEnterCurSor();
	void StopSoundEnterCursor();
	void PlaySoundPointCurSor();
	void StopSoundPointCursor();


};