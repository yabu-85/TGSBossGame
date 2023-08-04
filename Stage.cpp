#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"

//コンストラクタ
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), hModel_{ -1, -1, -1 }, table_(nullptr)
{
    CsvReader csv;
    csv.Load("Map.csv");

    width_ = (int)csv.GetWidth();
    height_ = (int)csv.GetHeight();

    table_ = new int* [width_];

    for (int x = 0; x < width_; x++) {
        table_[x] = new int[height_];

        for (int y = 0; y < height_; y++) {
            table_[x][height_ - 1 - y] = csv.GetValue(x, y);
        }
    }
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    const char* fileName[] = { "Floor.fbx","Wall.fbx", "HightWall.fbx"};

    //モデルデータのロード
    for (int i = 0; i < TYPE_MAX; i++) { //壁と床のロード
        hModel_[i] = Model::Load(fileName[i]);
        assert(hModel_[i] >= 0);
    }
}

//更新
void Stage::Update()
{
}

//描画
void Stage::Draw()
{
    Transform blockTrans;

    for (int x = 0; x < width_; x++) {                   //一つのStageオブジェクトで何個も表示させる
        for (int z = 0; z < height_; z++) {                 //transform.posxtxon.x を使うとバグるからblocktransの変数を使う
            blockTrans.position_.x = x;
            blockTrans.position_.z = z;

            int type = table_[x][z];

            Model::SetTransform(hModel_[type], blockTrans);
            Model::Draw(hModel_[type]);

        }
    }
}

//開放
void Stage::Release()
{
    for (int x = 0; x < width_; x++) {  //ｂの中のポインタを消してから bを消す
        delete[] table_[x];
    }

    delete[] table_;
}

float Stage::GetFloorHeight(int x, int z)
{
    if (x < width_ && z < height_ && x >= 0 && z >= 0) {
        switch (table_[x][z])
        {
        case 0:
            return 0.0f;
        case 1:
            return 1.0f;
        case 2:
            return 5.0f;
        default:
            return 0.0f;
        }
    }

    return 0.0f;
}

XMFLOAT3 Stage::GetPlaPos()
{
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (table_[x][y] == 10) {
                table_[x][y] = 0;
                return XMFLOAT3((float)x + 0.5f, 0.0f, (float)y + 0.5f);
            }
        }
    }

    return XMFLOAT3{ 0,0,0 };
}
