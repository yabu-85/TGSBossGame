#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"
#include "Player.h"

namespace {
    const int drawLeng = 90; //前描画しない距離
    const int backDrawLeng = 7; //後ろ描画しない距離

}

Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), table_(nullptr), width_(0), height_(0), pPlayer_(nullptr), plaPos_(0,0,0), hModel_(-1)
{

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
    
    hModel_ = Model::Load(fileName[0]);
    assert(hModel_ >= 0);
}

void Stage::Update()
{
}

void Stage::Draw()
{
    //transform.position を使うとバグるのでblocktrans変数を使う
    Transform blockTrans;

    if (pPlayer_ == nullptr) {
        if (FindObject("Player"))pPlayer_ = (Player*)FindObject("Player");

        return;
    }

    float plaPosZ = pPlayer_->GetPosition().z;

    //一つのStageオブジェクトで個数分表示させる
    for (int x = 0; x < width_; x++) 
    {
        for (int z = 0; z < height_; z++) 
        {
            if (plaPosZ + drawLeng < z || plaPosZ - backDrawLeng > z)
                continue;

            blockTrans.position_.x = (float)x;
            blockTrans.position_.z = (float)z;
            if (table_[x][z] != 0) continue;

            //モデル描画
            Model::SetTransform(hModel_, blockTrans);
            Model::Draw(hModel_);
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

XMFLOAT3 Stage::NearestFloorLocation(XMFLOAT3 pos)
{
    const float leng = 15.0f;

    if (pos.z < leng) pos.z = leng;
    for (int z = (int)pos.z - leng; z > 0; z--) {
        for (int x = 0; x < width_; x++)
        {
            if (table_[x][z] == 0) {
                return XMFLOAT3(3.5f, 0.0f, (float)z);
            }
        }
    }

    return XMFLOAT3(3, 0, 0);
}

void Stage::InitMapData(std::string mp)
{
    //CSVファイル読み込み
    CsvReader csv;
    csv.Load(mp + ".csv");

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
                table_[x][height_ - 1 - y] = 0;
                plaPos_ = XMFLOAT3((float)x + 0.5f, 0.0f, (float)height_ - 1 - y + 0.5f);
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
