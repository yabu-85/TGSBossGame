#include "Timer.h"
#include "Engine/Input.h"

Timer::Timer(GameObject* parent)
    :GameObject(parent, "Timer"), pNum_(nullptr), frame_(0), active_(false), drawX_(10), drawY_(20)
{
}

Timer::~Timer()
{
}

void Timer::Initialize()
{
    pNum_ = new Text;
    pNum_->Initialize();
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
}

//�J��
void Timer::Release()
{
}
