#include "Logo.h"
#include "Engine/Image.h"

namespace {
	float totalTime;	//そのsequenceの掛かる時間
	float currentTime;	//現在の時間
	int seq_line;		//今実行している行
	float seq_time;		//シーケンスを実行している時間

	enum ACT {
		A_SLIDEIN = 1,
		A_WAIT1,
		A_SLIDEOUT,
		A_WAIT2,
		A_TITLE,
	};

	struct Sequence {
		float time;  //時間
		int action;  //やること
		float alpha;   //必要な値

	}
	tbl[] = {
		{0.0f, A_SLIDEIN, 1.0f},	 //表示
		{1.0f, A_WAIT1, 1.0f},	     //待つ
		{3.0f, A_SLIDEOUT, 0.0f},    //映さない
		{4.0f, A_WAIT2, 0.0f},		 //待つ
		{4.3f, A_TITLE, 0.0f}		 //ここで消える
	};
}

Logo::Logo(GameObject* parent):
	GameObject(parent, "Logo"), hPict_{-1, -1}, end_(false)
{
}

Logo::~Logo()
{
}

void Logo::Initialize()
{
	hPict_[0] = Image::Load("Png/Team.png");
	assert(hPict_[0] >= 0);

	hPict_[1] = Image::Load("Png/Black.png");
	assert(hPict_[1] >= 0);

	totalTime = tbl[seq_line+1].time;
	currentTime = 0.0f;
	seq_line = 0;

	Image::SetAlpha(hPict_[0], 0);
	end_ = false;
}

void Logo::Update()
{
	seq_time += 1.0f / 60.0f;

	//時間超えたら次のシーケンスに
	if (seq_time >= tbl[seq_line + 1].time) {
		seq_line++;

		switch (tbl[seq_line].action) {
		case A_SLIDEIN:
			totalTime = tbl[seq_line + 1].time - seq_time;
			currentTime = 0.0f;
			break;

		case A_WAIT1:
			totalTime = tbl[seq_line + 1].time - seq_time;
			currentTime = 0.0f;
			break;

		case A_SLIDEOUT:
			totalTime = tbl[seq_line + 1].time - seq_time;
			currentTime = 0.0f;
			break;

		case A_WAIT2:
			totalTime = tbl[seq_line + 1].time - seq_time;
			currentTime = 0.0f;
			break;

		case A_TITLE:
			end_ = true;
			break;

		}
	}

	currentTime += 1.0f / 60.0f;
	if (currentTime > totalTime)
		currentTime = totalTime;

	if (seq_line + 1 == A_SLIDEIN || seq_line + 1 == A_SLIDEOUT) {
		float t = currentTime / totalTime;
		if (tbl[seq_line].alpha == 0.0f) t = 1.0f - t;
		Image::SetAlpha(hPict_[0], 255 * t );
	}



}

void Logo::Draw()
{
	Image::SetTransform(hPict_[0], transform_);
	Image::Draw(hPict_[0]);

	Transform pos;
	pos.scale_.x = 2;
	pos.scale_.y = 2;
	Image::SetAlpha(hPict_[1], 011);
	Image::SetTransform(hPict_[1], pos);
	Image::Draw(hPict_[1]);
}

void Logo::Release()
{
}
