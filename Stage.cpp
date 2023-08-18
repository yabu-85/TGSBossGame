#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"

const int startX = 10;
const int startZ = 10;

Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), hModel_{ -1, -1, -1, -1, -1 }, table_(nullptr)
{
    //CSV�t�@�C���ǂݍ���
    CsvReader csv;
    csv.Load("Map.csv");

    //�X�e�[�W�̕��ƍ�����ݒ�
    width_ = (int)csv.GetWidth();
    height_ = (int)csv.GetHeight();

    //CSV�f�[�^���e�[�u���Ɋi�[
    table_ = new int* [width_];
    for (int x = 0; x < width_; x++) 
    {
        table_[x] = new int[height_];
        for (int y = 0; y < height_; y++)
        {
            table_[x][height_ - 1 - y] = csv.GetValue(x, y);
        }
    }
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    const char* fileName[] = 
    { 
        "Floor.fbx",
        "Wall.fbx",
        "Wall2.fbx",
        "HightWall.fbx",
        "Slope.fbx"
    };

    //���f���f�[�^�̃��[�h
    for (int i = 0; i < TYPE_MAX; i++) 
    { 
        hModel_[i] = Model::Load(fileName[i]);
        assert(hModel_[i] >= 0);
    }
}

void Stage::Update()
{
}

void Stage::Draw()
{
    //transform.position ���g���ƃo�O��̂�blocktrans�ϐ����g��
    Transform blockTrans;

    //���Stage�I�u�W�F�N�g�Ō����\��������
    for (int x = 0; x < width_; x++) 
    {
        for (int z = 0; z < height_; z++) 
        {
            blockTrans.position_.x = (float)x;
            blockTrans.position_.z = (float)z;
            int type = table_[x][z];

            //���f���`��
            Model::SetTransform(hModel_[type], blockTrans);
            Model::Draw(hModel_[type]);
        }
    }
}

void Stage::Release()
{
    for (int x = 0; x < width_; x++) 
    {  
        //���̒��̃|�C���^�������Ă��� b������
        delete[] table_[x];
    }
    delete[] table_;
}

float Stage::GetFloorHeight(int x, int z)
{
    if (x < 0 || z < 0 || x >= width_ || z >= height_)
    {
        return -10.0f;      //�����ȍ��W�̏ꍇ�������^�[��
    }

    int type = table_[x][z];

    switch (type)
    {
    case 0:
        return 0.0f;
    case 1:
        return 1.0f;
    case 2:
        return 2.0f;
    case 3:
        return 5.0f;
    default:
        return -10.0f;
    }
}

bool Stage::IsWall(int x, int z)
{
    if (x >= 0 && z >= 0 && x < width_ && z < height_)
    {
        int type = table_[x][z];

        if (type == 1 || type == 2 || type == 3)
        {
            return true;
        }
    }
    return false;
}

XMFLOAT3 Stage::GetPlaPos()
{
    for (int x = 0; x < width_; x++) 
    {
        for (int y = 0; y < height_; y++) 
        {
            if (table_[x][y] == 10) 
            {
                table_[x][y] = 0;
                return XMFLOAT3((float)x + 0.5f, 0.0f, (float)y + 0.5f);
            }
        }
    }
    return XMFLOAT3{ 0,0,0 };
}
