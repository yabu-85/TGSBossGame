#include "TestObstacle.h"
#include "Engine/Model.h"

TestObstacle::TestObstacle(GameObject* parent)
	:Obstacle(parent)
{
}

TestObstacle::~TestObstacle()
{
}

void TestObstacle::Initialize()
{
	hModel_ = Model::Load("Wall.fbx");
	assert(hModel_ >= 0);
}

void TestObstacle::Update()
{
	static float a = 0.0f;
	static bool b = true;

	transform_.position_.z -= 0.1f;
	transform_.position_.x += a;

	if (b) {
		a += 0.01f;
		if (a >= 0.1f)
			b = false;
	}
	else {
		a -= 0.01f;
		if (a <= -0.1f)
			b = true;

	}
}

void TestObstacle::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void TestObstacle::Release()
{
}
