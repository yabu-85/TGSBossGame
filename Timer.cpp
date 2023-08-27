#include "Timer.h"
#include "Engine/Input.h"

Timer::Timer(GameObject* parent)
    :GameObject(parent, "Timer"), pNum_(nullptr),
   frame_(0), active_(false), drawX_(10), drawY_(20), hPict_{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
{
}

Timer::~Timer()
{
}

void Timer::Initialize()
{
    pNum_ = new Text;
    pNum_->Initialize();

    for (int i = 0; i < 10; i++) {
        std::string fileName = "Time";
        fileName = fileName + std::to_string( (int)(i % 2 + 1) );
        fileName = fileName + ".png";
        hPict_[i] = Image::Load(fileName);
        assert(hPict_[i] >= 0);
    }

}

void Timer::Update()
{
    //�N�����
    if (active_)
    {
        //�J�E���g�_�E��
        if (frame_ > 0)
            frame_--;
    }
}

//�`��
void Timer::Draw()
{
    //�e�L�X�g�\��
    pNum_->SetScale(1.0f);                              //�e�L�X�g�̃T�C�Y
    pNum_->Draw(drawX_, drawY_, "Time:");               //�`��

    int frameCount = frame_ % FPS;                      //��b���ƃ��Z�b�g
    //�^�C�}�[���l�\��
    if (frameCount < 10)                                //������
        pNum_->SetScale(frameCount * 0.2f + 1.0f);      //�K��t���[���ȉ����ɓK��
    else
        pNum_->SetScale(1.0f);

    int sec = frame_ / FPS;                             //�b��
    pNum_->Draw(drawX_ + 100, drawY_, sec);

    //�ꌅ�ڎ擾
    int firstDigit = sec % 10;
    int secondDigit = (sec / 10) % 10; // 2���ڂ̐������擾
    int thirdDigit = sec / 100; // 3���ڂ̐������擾

    Transform pic;
    pic.scale_.x = 0.2f;
    pic.scale_.y = 0.2f;
    Image::SetTransform(hPict_[firstDigit], pic);
    Image::Draw(hPict_[firstDigit]);

    static int time = 100;
    time -= (100.0 / FPS);
    if (time < 0) time = 100;

    pNum_->Draw(drawX_ + 500, drawY_, time );
}

//�J��
void Timer::Release()
{
}
