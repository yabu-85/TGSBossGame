#include "AudioManager.h"
#include "Engine/Audio.h"
#include <cassert>
#include <vector>

#include <fstream>
#include <string>
#include <sstream>
using namespace std;

namespace AudioManager
{
    std::vector<int> hSound_;
    float gameVolue_ = 1.0f; //0.0f ～1.0f

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
                {"Sound/BeamShot.wav", false, 1},

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

    void InitVolue()
    {
        //ファイル読み込み
        std::ifstream ifs2("GameAudioValue");
        std::string data2;
        ifs2 >> data2;
        //stringからintへ変換し、そのあと値をセット
        std::istringstream ss2 = std::istringstream(data2);
        ss2 >> gameVolue_;
        gameVolue_ /= 100.0f;
    }

	void Release()
	{
		Audio::Release();
	}

	void PlaySoundMa(TITLE_AUDIO i, float volume)
	{
		Audio::Play(hSound_[i], volume * gameVolue_);
	}

	void PlaySoundMa(PLAY_AUDIO i, float volume)
	{
        Audio::Play(hSound_[i], volume * gameVolue_);
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


