#include "AudioManager.h"
#include "Engine/Audio.h"
#include <cassert>
#include <vector>

namespace AudioManager
{
    std::vector<int> hSound_;

    void Initialize(INIT_TYPE type) {

        struct MyStruct
        {
            std::string fileName;
            bool loop;
            int max;
        };
        std::vector<MyStruct> sceneTables;

        //ヘッダーの順番と合わせないといけない
        switch (type) {
        case TITLE:
            sceneTables = {
                {"Sound/EnterCursor.wav", false, 3},
                {"Sound/PointCursor.wav", false, 10},
                {"Sound/EnterStage.wav",  false, 3},
            };
            break;
        case PLAY:
            sceneTables = {
                //敵のAudio
                {"Sound/RobotHit.wav", false, 3},
                {"Sound/MissileExplode.wav", false, 3},
                {"Sound/MissileShot.wav", false, 1},
                {"Sound/UFO_Charging.wav", false, 1},
                {"Sound/UFO_Attack.wav", false, 1},

                //PlayerのAudio
                {"Sound/Running.wav", true, 1},
                {"Sound/Missile_Reflection.wav", false, 1},
                {"Sound/JumpingEnd.wav", false, 1}, //ここ変えよう
                {"Sound/JumpingEnd.wav", false, 1},

            };
            break;
        case PLAYMENUE:
            sceneTables = {
                {"Sound/EnterCursor.wav", false, 3},
                {"Sound/PointCursor.wav", false, 10},
            };
            break;
        case RESULT:
            sceneTables = {
                { "Sound/EnterCursor.wav", false, 3 },
                { "Sound/PointCursor.wav", false, 10 },
            };
            break;
        }

        hSound_.resize(sceneTables.size()); //hSound_ベクターのサイズを設定

        for (int i = 0; i < sceneTables.size(); i++) {
            hSound_[i] = Audio::Load(sceneTables[i].fileName, sceneTables[i].loop, sceneTables[i].max);
            assert(hSound_[i] >= 0);
        }
    }


	void Release()
	{
		Audio::Release();
	}

	void PlaySoundMa(TITLE_AUDIO i, float volume)
	{
		Audio::Play(hSound_[i], volume * 0.5f);
	}

	void PlaySoundMa(PLAY_AUDIO i, float volume)
	{
        Audio::Play(hSound_[i], volume * 0.5f);
	}

	void StopSoundMa(TITLE_AUDIO i)
	{
		Audio::Stop(hSound_[i]);
	}

	void StopSoundMa(PLAY_AUDIO i)
	{
        Audio::Stop(hSound_[i]);
	}

}


