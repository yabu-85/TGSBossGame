#include "Logo.h"
#include "Engine/Image.h"

namespace {
	float totalTime;	//����sequence�̊|���鎞��
	float currentTime;	//���݂̎���
	int seq_line;		//�����s���Ă���s
	float seq_time;		//�V�[�P���X�����s���Ă��鎞��

	enum ACT {
		A_SLIDEIN = 1,
		A_WAIT1,
		A_SLIDEOUT,
		A_WAIT2,
		A_TITLE,
	};

	struct Sequence {
		float time;  //����
		int action;  //��邱��
		float alpha;   //�K�v�Ȓl

	}
	tbl[] = {
		{0.0f, A_SLIDEIN, 1.0f},	 //�\��
		{1.0f, A_WAIT1, 1.0f},	     //�҂�
		{3.0f, A_SLIDEOUT, 0.0f},    //�f���Ȃ�
		{4.0f, A_WAIT2, 0.0f},		 //�҂�
		{4.3f, A_TITLE, 0.0f}		 //�����ŏ�����
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

	//���Ԓ������玟�̃V�[�P���X��
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
