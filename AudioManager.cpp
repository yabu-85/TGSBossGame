#include "AudioManager.h"
#include "Engine/Audio.h"
#include <cassert>
#include <vector>

namespace AudioManager
{
	int hSound_[AUDIO_MAX];

	void Initialize(SCENE_ID type) {
		Audio::Release();

		struct MyStruct
		{
			std::string fileName;
			bool loop;
			int max;
		};

		// シーンごとのテーブルを初期化
		std::vector<MyStruct> sceneTables[SCENE_ID_MAX];

		if (type == SCENE_ID::SCENE_ID_TITLE) {
			sceneTables[0] = {
				{ "Sound/EnterCursor.wav", false, 3 },
				{ "Sound/PointCursor.wav", false, 16 },
			};
		}
		else if (type == SCENE_ID::SCENE_ID_PLAY) {
			sceneTables[1] = {
				{ "Sound/EnterCursor.wav", false, 3 },
			};
		}
		else if (type == SCENE_ID::SCENE_ID_RESULT) {
			sceneTables[2] = {
				{ "Sound/EnterCursor.wav", false, 3 },
			};
		}

		for (int i = 0; i < AUDIO_MAX; i++) {
			hSound_[i] = Audio::Load(sceneTables[type][i].fileName, sceneTables[type][i].loop, sceneTables[type][i].max);
			assert(hSound_[i] >= 0);
		}
	}

	void Release()
	{
		Audio::Release();
	}

	void PlaySoundMa(AUDIO_ID i)
	{
		Audio::Play(hSound_[i]);
	}

	void StopSoundMa(AUDIO_ID i)
	{
		Audio::Stop(hSound_[i]);
	}

}


