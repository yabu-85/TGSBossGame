#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"

//�R���X�g���N�^
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), hModel_{ -1, -1 }, table_(nullptr)
{
    CsvReader csv;
    csv.Load("Map.csv");

    width_ = csv.GetWidth();
    height_ = csv.GetHeight();

    table_ = new int* [width_];

    for (int x = 0; x < width_; x++) {
        table_[x] = new int[height_];

        for (int y = 0; y < height_; y++) {
            table_[x][height_ - 1 - y] = csv.GetValue(x, y);
        }
    }
}

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
    const char* fileName[] = { "Floor.fbx","Wall.fbx" };

    //���f���f�[�^�̃��[�h
    for (int i = 0; i < TYPE_MAX; i++) { //�ǂƏ��̃��[�h
        hModel_[i] = Model::Load(fileName[i]);
        assert(hModel_[i] >= 0);
    }

    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (table_[x][y] == 10) {
                Player* pPlayer = (Player*)FindObject("Player");
                pPlayer->SetPosition(x + 0.5, 0, y + 0.5);

                table_[x][y] = 0;
            }
        }
    }
}

//�X�V
void Stage::Update()
{
}

//�`��
void Stage::Draw()
{
    Transform blockTrans;

    for (int x = 0; x < width_; x++) {                   //���Stage�I�u�W�F�N�g�ŉ����\��������
        for (int z = 0; z < height_; z++) {                 //transform.posxtxon.x ���g���ƃo�O�邩��blocktrans�̕ϐ����g��
            blockTrans.position_.x = x;
            blockTrans.position_.z = z;

            int type = table_[x][z];

            Model::SetTransform(hModel_[type], blockTrans);
            Model::Draw(hModel_[type]);

        }
    }
}

//�J��
void Stage::Release()
{
    for (int x = 0; x < width_; x++) {  //���̒��̃|�C���^�������Ă��� b������
        delete[] table_[x];
    }

    delete[] table_;
}

int Stage::IsWall(int x, int z)
{
    if (table_[x][z] == 0) {
        return 0;
    }
    else if (table_[x][z] == 1) {
        return 1;
    }
    else if (table_[x][z] == 2) {
        return 2;
    }

    return 0;
}
