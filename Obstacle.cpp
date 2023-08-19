#include "Obstacle.h"
#include "Engine/Model.h"

Obstacle::Obstacle(GameObject* parent)
	:GameObject(parent, "Obstacle"), position_(0,0,0), hModel_(-1)
{
}

Obstacle::~Obstacle()
{
}

void Obstacle::Initialize()
{
}

void Obstacle::Update()
{
}

void Obstacle::Draw()
{
}

void Obstacle::Release()
{
}

void Obstacle::SetPosition(XMFLOAT3 _position)
{
    transform_.position_ = _position;
}
