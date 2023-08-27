#include "Obstacle.h"
#include "Engine/Model.h"

Obstacle::Obstacle(GameObject* parent)
	:GameObject(parent, "Obstacle"), hModel_(-1), active_(false)
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
