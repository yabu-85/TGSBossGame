#include "Timer.h"
#include "Engine/Input.h"

Timer::Timer(GameObject* parent)
    :GameObject(parent, "Timer"), frame_(0), active_(false), drawX_(10), drawY_(20),
    hPict_{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, calcTime_(0)
{
}

Timer::~Timer()
{
}

void Timer::Initialize()
{
    for (int i = 0; i < 10; i++) {
        std::string fileName = "Png/Time/Time";
        fileName = fileName + std::to_string(i);
        fileName = fileName + ".png";
        hPict_[i] = Image::Load(fileName);
        assert(hPict_[i] >= 0);
    }
    hPict_[10] = Image::Load("Png/Time/Colon.png");
    assert(hPict_[10] >= 0);

    hPict_[11] = Image::Load("Png/Time/Period.png");
    assert(hPict_[11] >= 0);
}

void Timer::Update()
{
    //�N�����
    if (active_)
    {
        //�J�E���g�_�E��
        if (frame_ > 0)
            frame_--;

        calcTime_ -= (100.0 / FPS);
        if (calcTime_ < 0) calcTime_ = 100;

    }
}

//�`��
void Timer::Draw()
{
    int frameCount = frame_ % FPS;                      //��b���ƃ��Z�b�g
    float size = frameCount * 0.2f + 1.0f;
    //�^�C�}�[���l�\��
    if (frameCount < 10)                                //������
        transform_.scale_ = { size, size, size };       //�K��t���[���ȉ����ɓK��
    else
        transform_.scale_ = { 1.0f, 1.0f, 1.0f};
    int sec = frame_ / FPS;                             //�b��

    int firstDigit = sec % 10;                          //��̈�
    int secondDigit = (sec / 10) % 10;                  //�\�̈ʂ̐������擾

    //���̈�̈�
    Transform pic1 = transform_;
    pic1.scale_.x += 1.2f;
    pic1.scale_.y += 1.2f;
    pic1.position_ = { -0.1f, 0.8, 0.0f };
    Image::SetTransform(hPict_[firstDigit], pic1);
    Image::Draw(hPict_[firstDigit]);
    
    //���̏\�̈�
    Transform pic2 = pic1;
    pic2.position_.x -= 0.09f;
    Image::SetTransform(hPict_[secondDigit], pic2);
    Image::Draw(hPict_[secondDigit]);

    //�s���I�h
    Transform picPeriod = pic1;
    picPeriod.scale_.x = 1.3f;
    picPeriod.scale_.y = 1.3f;
    picPeriod.position_.x += 0.06f;
    picPeriod.position_.y -= 0.03f;
    Image::SetTransform(hPict_[11], picPeriod);
    Image::Draw(hPict_[11]);

    firstDigit = calcTime_ % 10;                             //�P�s��
    secondDigit = (calcTime_ / 10) % 10;                     //2���ڂ̐������擾

    //�E�̈�̈�
    pic2.position_.x += 0.18f;
    pic2.position_.y += -0.03f;
    pic2.scale_.x = 1.3f;
    pic2.scale_.y = 1.3f;
    Image::SetTransform(hPict_[secondDigit], pic2);
    Image::Draw(hPict_[secondDigit]);

    //�E�̏\�̈�
    pic1.position_.x += 0.14f;
    pic1.position_.y += -0.03f;
    pic1.scale_.x = 1.3f;
    pic1.scale_.y = 1.3f;
    Image::SetTransform(hPict_[firstDigit], pic1);
    Image::Draw(hPict_[firstDigit]);



}

//�J��
void Timer::Release()
{
}
