#pragma once
#include "Engine/Text.h"
#include "Engine/GameObject.h"

//�t���[�����[�g
static const int FPS = 60;

//�^�C�}�[�Ǘ�����N���X
class Timer : public GameObject
{
    int hPict_[12];    //�摜�ԍ�
    bool active_;        //�N�����Ă��邩�ǂ���
    int frame_;          //��b�Ԃɉ���X�V����邩
    Text* pNum_;         //�b���e�L�X�g
    float drawX_;        //�`��ʒu(���E)
    float drawY_;        //�`��ʒu(�㉺)

public:
    //�R���X�g���N�^
    Timer(GameObject* parent);

    //�f�X�g���N�^
    ~Timer();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;


    //�^�C�}�[�ݒ�
    void SetLimit(float _sec) { frame_ = (int)(_sec * FPS); }

    //�^�C�}�[�J�n
    void Start() { active_ = true; }

    //�^�C�}�[��~
    void Stop() { active_ = false; }

    //�^�C�}�[�I������
    bool IsFinished() { return (frame_ <= 0); }

    //�^�C���擾
    int GetTime() { return frame_ / FPS; }
};