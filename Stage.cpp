#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"


Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), table_(nullptr)
{
    for (int i = 0; i < TYPE_MAX; i++) hModel_[i] = -1;

    //CSVファイル読み込み
    CsvReader csv;
    csv.Load("Obstacle.csv");

    //ステージの幅と高さを設定
    width_ = (int)csv.GetWidth();
    height_ = (int)csv.GetHeight();

    //CSVデータをテーブルに格納
    table_ = new int* [width_];
    for (int x = 0; x < width_; x++) 
    {
        table_[x] = new int[height_];
        for (int y = 0; y < height_; y++)
        {
            if (csv.GetValue(x, y) == 10)
            {
                table_[x][height_ - 1 - y] = 10;
            }
            else if (csv.GetValue(x, y) != -1)
            {
                table_[x][height_ - 1 - y] = 0;
            }
            else
            {
                table_[x][height_ - 1 - y] = -1;
            }
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
        "Model/StagePit2.fbx",
    };

    //モデルデータのロード
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
    //transform.position を使うとバグるのでblocktrans変数を使う
    Transform blockTrans;

    //一つのStageオブジェクトで個数分表示させる
    for (int x = 0; x < width_; x++) 
    {
        for (int z = 0; z < height_; z++) 
        {
            blockTrans.position_.x = (float)x;
            blockTrans.position_.z = (float)z;
            int type = table_[x][z];

            //モデル描画
            Model::SetTransform(hModel_[type], blockTrans);
            Model::Draw(hModel_[type]);
        }
    }
}

void Stage::Release()
{
    for (int x = 0; x < width_; x++) 
    {  
        //ｂの中のポインタを消してから bを消す
        delete[] table_[x];
    }
    delete[] table_;
    table_ = nullptr;
}

float Stage::GetFloorHeight(int x, int z)
{
    //無効な座標:床上ではない､早期リターンする
    if (x < 0 || z < 0 || x >= width_ || z >= height_)
        return -10.0f;      

    int type = table_[x][z];
    switch (type)
    {
    case TYPE_FLOOR:
        return 0.0f;
    default:
        return -10.0f;
    }
}

bool Stage::IsWall(int x, int z)
{
    //無効な座標:壁ではない､早期リターンする
    if (x < 0 || z < 0 || x >= width_ || z >= height_) return false;       

    int type = table_[x][z];
    switch (type)
    {
    case TYPE_FLOOR:
        return true;
    default:
        return false;
    }
}

XMFLOAT3 Stage::GetPlaPos()
{
    //フィールドを走査する
    for (int x = 0; x < width_; x++) 
    {
        for (int y = 0; y < height_; y++) 
        {
            //目的のナンバーを持つセル(Player)が見つかった
            if (table_[x][y] == TYPE_PLAYER)
            {
                table_[x][y] = 0;
                return XMFLOAT3((float)x + 0.5f, 0.0f, (float)y + 0.5f);
            }
        }
    }
    return XMFLOAT3{ 0,0,0 };
}

XMFLOAT3 Stage::NearestFloorLocation(XMFLOAT3 pos)
{
    if (pos.z < 0) pos.z = 0;
    for (int z = pos.z - 0; z > 0; z--) {
        for (int x = 0; x < width_; x++)
        {
            if (table_[x][z] == 0) {
                return XMFLOAT3((float)pos.x + 0.5f, 0.0f, (float)z + 0.5f);
            }
        }
    }

    return XMFLOAT3(3, 0, 0);
}