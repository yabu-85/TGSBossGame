#include "Obstacle.h"
#include "Engine/Model.h"

Obstacle::Obstacle(GameObject* parent)
	:GameObject(parent, "Obstacle"), x(0),y(0),z(0), hModel_(-1)
{
}

Obstacle::~Obstacle()
{
}

void Obstacle::Initialize()
{
    hModel_ = Model::Load("Sphere.fbx");
    assert(hModel_ >= 0);
}

void Obstacle::Update()
{
    transform_.position_.z -= 0.1f;
}

void Obstacle::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Obstacle::Release()
{
}

void Obstacle::SetPosition(int _x, int _y, int _z)
{
    x = _x;
    y = _y;
    z = _z;
    transform_.position_ = XMFLOAT3((float)x, (float)y, (float)z);
}
