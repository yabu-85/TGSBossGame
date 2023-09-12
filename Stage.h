#pragma once
#include "Engine/GameObject.h"

class Player;

//�I�u�W�F�N�g�̎�ނ𔻕ʂ���ԍ�
enum OBJ
{
    TYPE_FLOOR,         //��
    TYPE_MAX,            //�ő�l
    
    TYPE_PLAYER = 10
};

//�X�e�[�W���Ǘ�����N���X
class Stage : public GameObject
{    
    int width_;                 //�X�e�[�W��
    int height_;                //�X�e�[�W����
    int hModel_;      //���f���ԍ�
    int** table_;               //�X�e�[�W�Z���̃f�[�^
    XMFLOAT3 plaPos_;
    Player* pPlayer_;

public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    float GetFloorHeight(int x, int z);         //���̍������擾����֐�
    bool IsWall(int x, int z);                  //�ǂł��邩���肷��֐�
    int GetModelHandle(int i) { return hModel_; } //���f���ԍ����擾����֐�
    XMFLOAT3 GetPlaPos() { return plaPos_; };   //CSV��ł̃v���C���[���W���擾����֐�
    int GetHeight() { return height_; };        //�}�b�v�̍����i�S�[���܂Łj
    
    //�v���C���[�����������̈�ԋ߂���
    XMFLOAT3 NearestFloorLocation(XMFLOAT3 pos);

    //�}�b�v�f�[�^������
    void InitMapData(std::string mp);

};

