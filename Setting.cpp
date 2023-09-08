#include "Setting.h"
#include "Engine/Input.h"
#include "ButtonFactory.h"
#include "SliderButton.h"
#include "ExitMenu.h"
#include "Engine/SceneManager.h"
#include "AudioManager.h"
#include "PlayScene.h"
#include <future>

#include <fstream>
#include <string>
#include <sstream>
using namespace std;

Setting::Setting(GameObject* parent)
	:GameObject(parent, "Setting"), hPict_{ -1,-1 }, pButtonFactory_(nullptr), pSlider_(nullptr), pSlider2_(nullptr)
{
}

Setting::~Setting()
{
}

void Setting::Initialize()
{
	pButtonFactory_ = Instantiate<ButtonFactory>(this);
	pButtonFactory_->ButtonCreate(0.0f, -500.0f, 1.0f, 1.0f, "ReturnTitle");
	pButtonFactory_->SetAlpha(200);
	pButtonFactory_->SetFrameAlpha(200);
	pButtonFactory_->SetBlendMode(0);

	//テスト用-----------------------------------------
	pSlider_ = Instantiate<SliderButton>(this);
	pSlider_->SetValue(0.0f, 150.0f, 1.0f, 1.0f, "MouseSensitivity");
	pSlider_->SetAlpha(200);
	pSlider_->SetFrameAlpha(200);
	pSlider_->SetBlendMode(0);

	//ファイル読み込み
	std::ifstream ifs("GameValue");
	std::string data;
	ifs >> data;
	//stringからintへ変換し、そのあと値をセット
	std::istringstream ss = std::istringstream(data);
	int num = 0;
	ss >> num;
	pSlider_->SetNum(num, 100);

	////テスト用２−−−−−−−−−−−−−−−
	pSlider2_ = Instantiate<SliderButton>(this);
	pSlider2_->SetValue(0.0f, -150.0f, 1.0f, 1.0f, "AudioVolue");
	pSlider2_->SetAlpha(200);
	pSlider2_->SetFrameAlpha(200);
	pSlider2_->SetBlendMode(0);

	//ファイル読み込み
	std::ifstream ifs2("GameAudioValue");
	std::string data2;
	ifs2 >> data2;
	//stringからintへ変換し、そのあと値をセット
	std::istringstream ss2 = std::istringstream(data2);
	num = 0;
	ss2 >> num;
	pSlider2_->SetNum(num, 100);

	/////////--------------------終わり

	hPict_[0] = Image::Load("Png/Black.png");
	assert(hPict_[0] >= 0);
	hPict_[1] = Image::Load("Png/cross.png");
	assert(hPict_[1] >= 0);

}

void Setting::Update()
{
	//マウス感度設定->Factory作ってまとめたほうがいいne
	if (pSlider_->IsWithinBound() && Input::IsMouseButtonUp(0)) {
		std::string data;
		int i = pSlider_->GetValue();
		std::ostringstream oss;
		oss << i;
		data = oss.str();

		std::ofstream ofs("GameValue");
		ofs << data << std::endl;
		ofs.close();
	}

	if (pSlider2_->IsWithinBound() && Input::IsMouseButtonUp(0)) {
		std::string data;
		int i = pSlider2_->GetValue();
		std::ostringstream oss;
		oss << i;
		data = oss.str();

		std::ofstream ofs("GameAudioValue");
		ofs << data << std::endl;

		AudioManager::gameVolue_ = i;
		ofs.close();

	}


	if (pButtonFactory_->CheckButtonPressed() == "ReturnTitle"){
		GameObject* gs2 = GetParent()->FindObject("ButtonFactory");
		ButtonFactory* pB = (ButtonFactory*)gs2;
		pB->SetActive(true);
		pB->SetAlpha(200);
		pB->SetFrameAlpha(200);
		KillMe();
	}

}

void Setting::Draw()
{
	Image::SetAlpha(hPict_[0], 100);
	Image::SetTransform(hPict_[0], transform_);
	Image::Draw(hPict_[0]);

	Transform abb;
	abb.position_ = Input::GetMousePositionSub();
	static float screenWidth = (float)GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	static float screenHeight = (float)GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ
	abb.position_ = { abb.position_.x / screenWidth, -abb.position_.y / screenHeight , 0 };

	Image::SetTransform(hPict_[1], abb);
	Image::Draw(hPict_[1]);
}

void Setting::Release()
{
}