#pragma once
#include "Engine/GameObject.h"
#include "Player.h"

//�I�u�W�F�N�g�̎�ނ𔻕ʂ���ԍ�
enum OBJ
{
    TYPE_FLOOR,         //��
    TYPE_MAX            //�ő�l
};

enum CHARACTER
{
    TYPE_PLAYER = 10    //�v���C���[
};

//�X�e�[�W���Ǘ�����N���X
class Stage : public GameObject
{    
    int width_;                 //�X�e�[�W��
    int height_;                //�X�e�[�W����
    int hModel_[TYPE_MAX];      //���f���ԍ�
    int** table_;               //�X�e�[�W�Z���̃f�[�^

public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    
    float GetFloorHeight(int x, int z);     //���̍������擾����֐�
    bool IsWall(int x, int z);              //�ǂł��邩���肷��֐�
    int GetModelHandle(int i) { return hModel_[i]; } //���f���ԍ����擾����֐�
    XMFLOAT3 GetPlaPos();                   //CSV��ł̃v���C���[���W���擾����֐�
    int GetHeight() { return height_; };    //�}�b�v�̍����i�S�[���܂Łj
    
    //�v���C���[�����������̈�ԋ߂���
    XMFLOAT3 NearestFloorLocation(XMFLOAT3 pos);

    //
    void InitMapData(std::string mp);

};

