#pragma once
#include "Engine/GameObject.h"
#include "Player.h"

//�I�u�W�F�N�g�̎�ނ𔻕ʂ���ԍ�
enum 
{
    TYPE_FLOOR,         //��
    TYPE_WALL,          //��
    TYPE_WALL2,         //��(��)
    TYPE_HIGHTWALL,     //��(����)
    TYPE_SLOPE,         //�X��
    TYPE_MAX            //�ő�l
};

//�X�e�[�W���Ǘ�����N���X
class Stage : public GameObject
{    
    int width_;                 //�X�e�[�W��
    int height_;                //�X�e�[�W����
    int hModel_[TYPE_MAX];      //���f���ԍ�
    int** table_;               //�X�e�[�W�Z���̃f�[�^

public:
    //�R���X�g���N�^
    Stage(GameObject* parent);

    //�f�X�g���N�^
    ~Stage();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;


    //���̍������擾����֐�
    float GetFloorHeight(int x, int z);

    //�Z�����ǂł��邩���肷��֐�
    bool IsWall(int x, int z);

    //���f���ԍ����擾����֐�
    int GetModelHandle(int i) { return hModel_[i]; };

    //CSV��ł̃v���C���[���W���擾����֐�
    XMFLOAT3 GetPlaPos(); 
};

