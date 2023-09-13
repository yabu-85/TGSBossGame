#pragma once
enum TITLE_AUDIO
{
	AUDIO_ENTERCURSOR = 0,
	AUDIO_POINTCURSOR,
	AUDIO_ENTERSTAGE,

};

enum PLAY_AUDIO
{
	AUDIO_ROBOT_HIT,
	AUDIO_MISSILE_EXPLODE,
	AUDIO_MISSILE_SHOT,
	AUDIO_UFO_CHARGING,
	AUDIO_UFO_ATTACK,
	AUDIO_BEAM_SHOT,

	AUDIO_RUNNING,
	AUDIO_REFLECTION,
	AUDIO_JUNPING_START,
	AUDIO_JUNPING_END,
};

namespace AudioManager {
	enum INIT_TYPE {
		TITLE = 0,
		PLAY,
		PLAYMENUE,
		RESULT,
		MAX,

	};

	extern float gameVolue_;

	void Initialize(INIT_TYPE i);
	void InitVolue();
	void Release();

	void PlaySoundMa(TITLE_AUDIO i, float volume = 1.0f);
	void PlaySoundMa(PLAY_AUDIO i, float volume = 1.0f);

	void StopSoundMa(TITLE_AUDIO i);
	void StopSoundMa(PLAY_AUDIO i);

};