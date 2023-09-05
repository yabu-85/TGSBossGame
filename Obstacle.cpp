#include "Obstacle.h"
#include "Engine/Model.h"

Obstacle::Obstacle(GameObject* parent)
	:GameObject(parent), hModel_(-1), active_(false), isDraw_(false), csvPos_(0,0,0)
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
