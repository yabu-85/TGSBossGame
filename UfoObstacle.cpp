#include "UfoObstacle.h"
#include "Engine/SphereCollider.h"
#include "Engine/Model.h"
#include "Engine/Audio.h"
#include "Engine/VFX.h"
#include "Engine/Direct3D.h"
#include "Player.h"

static const float height_ = 15.0f;

UfoObstacle::UfoObstacle(GameObject* parent)
	:Obstacle(parent), hModelSub_{-1, -1}, state_(S_ENTER), stateEnter_(true), targetPos_(0, 0, 0), moveSpeed_(0.0f), time_(0),
	moveDist_(0), leavVec_{0,1,0,0}, leavYmoveSpeed_(0.0f), attack_(false), hSound_{-1, -1}
{
	objectName_ = "UfoObstacle";
}

UfoObstacle::~UfoObstacle()
{
}

void UfoObstacle::Initialize()
{
	hModel_ = Model::Load("Model/Ufo.fbx");
	assert(hModel_ >= 0);

	std::string fileName[] = { "RaserCenter", "RaserOuter", "RaserDetect" };
	for (int i = 0; i < 3; i++) {
		hModelSub_[i] = Model::Load("Model/" + fileName[i] + ".fbx");
		assert(hModelSub_[i] >= 0);
	}

	moveSpeed_ = 1.0f;
	moveDist_ = 50.0f;
	leavYmoveSpeed_ = 0.15f;

	//サウンドデータのロード
	hSound_[0] = Audio::Load("Sound/UFO_Charging.wav");
	assert(hSound_[0] >= 0);

	hSound_[1] = Audio::Load("Sound/UFO_Attack.wav");
	assert(hSound_[1] >= 0);

}

void UfoObstacle::Update()
{
	if (!active_) return;

	switch (state_) {
	case STATE::S_ENTER:
		UpdateEnter();
		break;
	case STATE::S_DETECT:
		UpdateDetection();
		break;
	case STATE::S_PREARATION:
		UpdatePreparation();
		break;
	case STATE::S_SHOT:
		UpdateShot();
		break;
	case STATE::S_LEAVING:
		UpdateLeaving();
		break;
	}

}

void UfoObstacle::Draw()
{
	if (!isDraw_) return;

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	if (state_ == S_DETECT) {
		Transform pos = transform_;
		pos.position_.y -= 1.0f;
		Model::SetTransform(hModelSub_[2], pos);
		Model::Draw(hModelSub_[2], 5);
		
	}

	if (state_ == S_SHOT || (state_ == S_LEAVING && attack_) ) {
		Transform pos = transform_;
		pos.position_.y -= 1.5f;
		Model::SetTransform(hModelSub_[0], pos);
		Model::Draw(hModelSub_[0], 4);

		Model::SetTransform(hModelSub_[1], pos);
		Model::Draw(hModelSub_[1], 5);

	}

}

void UfoObstacle::Release()
{
}

void UfoObstacle::UpdateEnter()
{
	if (stateEnter_) {
		stateEnter_ = false;

		targetPos_ = transform_.position_;
		targetPos_.y = height_;

		transform_.position_.y = height_;
		transform_.position_.x += (float)( (rand() % 2 - 1) * (rand() % 5 + 10) );
		transform_.position_.y += (float)(rand() % 2 - 5);		//なるべく見えやすいよう下の方から登場させる
		transform_.position_.z += -5.0f + (float)( rand() % 5 + 20);
	}

	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vTar = XMLoadFloat3(&targetPos_);
	XMVECTOR vMovePos = vTar - vPos;
	vMovePos = XMVector3Normalize(vMovePos) * moveSpeed_;
	vMovePos += vPos;
	XMFLOAT3 fPos;
	XMStoreFloat3(&fPos, vMovePos);
	transform_.position_ = fPos;

	float leng = XMVectorGetX(XMVector3Length(vMovePos - vTar));
	if (leng <= 1.0f) ChangeState(S_DETECT);

}

void UfoObstacle::UpdateDetection()
{
	//時間切れで去る
	int shotTime = 500;
	if (time_ > shotTime) ChangeState(S_LEAVING);
	time_++;

	//こっちは範囲内に入ったら攻撃の準備へ
	Player* pPla = (Player*)FindObject("Player");

	//円錐出来ないから円と円の衝突判定をn回繰り返す
	const int num = 7;
	const float dist[num] = { 0.7f, 0.9f, 1.15f, 1.38f, 1.65f, 2.0f, 2.35f };		//円の半径
	const float heig[num] = { -2.0f, -3.5f, -5.0f, -7.0f, -9.0f, -11.0f, -13.5f };	//円の高さ
	const float rad = 0.2f;

	for (int i = 0; i < num; i++) {
		XMFLOAT3 fPos = { transform_.position_.x, transform_.position_.y + -1.0f + heig[i], transform_.position_.z };
		XMFLOAT3 fTar = pPla->GetPosition();
		fTar.y += 0.5f;
		XMVECTOR tar = XMLoadFloat3(&fTar);
		XMVECTOR pos = XMLoadFloat3(&fPos);
		float distance = XMVectorGetX(XMVector3Length(pos - tar));

		//ここはTargetの場所の範囲に入ったら
		if (distance < (dist[i] + rad)) {
			attack_ = true;
			Audio::Play(hSound_[0]);
			ChangeState(S_PREARATION);
		}
	}
}

void UfoObstacle::UpdatePreparation()
{
	if (stateEnter_) {
		stateEnter_ = false;
		time_ = 0;

	}

	//見やすいようにテストエフェクト
	Player* pPla = (Player*)FindObject("Player");
	XMFLOAT3 weapTop = pPla->GetPosition();
	weapTop.y = 0.0f;
	XMFLOAT3 weapRoot = transform_.position_;
	weapRoot.y = 0.0f;

	XMVECTOR vTop = XMLoadFloat3(&weapTop);
	XMVECTOR vRoot = XMLoadFloat3(&weapRoot);
	XMVECTOR vMove = vTop - vRoot;
	vMove = XMVector3Normalize(vMove) * 0.9f;
	XMFLOAT3 move;
	XMStoreFloat3(&move, vMove);
	weapRoot = XMFLOAT3(weapRoot.x + (move.x * 0.1f), weapRoot.y + (move.y * 0.1f), weapRoot.z + (move.z * 0.1f));
	float randomPos = (float)(rand() % 99 + 1) * 0.2f;

	EmitterData data1;
	data1.textureFileName = "Particle/cloudA.png";
	data1.position = XMFLOAT3(weapRoot.x + move.x * randomPos, weapRoot.y + move.y * randomPos, weapRoot.z + move.z * randomPos);
	data1.positionRnd = XMFLOAT3(0.01f, 0.01f, 0.01f);
	data1.direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	data1.delay = 0;
	data1.gravity = 0.001f;
	data1.number = 1;
	data1.lifeTime = 10;
	data1.size = XMFLOAT2(0.3f, 0.3f);
	data1.color = XMFLOAT4(0.0f, 0.7f, 1.0f, 1.0f);
	VFX::Start(data1);


	int shotTime = 220;
	if (time_ > shotTime) {

		//第二のTargetPosition決定
		Player* pPla = (Player*)FindObject("Player");
		targetPos_ = pPla->GetPosition();
		targetPos_.y = height_;
		XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
		XMVECTOR vTar = XMLoadFloat3(&targetPos_);
		XMVECTOR vMovePos = vTar - vPos;
		vMovePos = XMVector3Normalize(vMovePos) * moveDist_;
		vMovePos += vPos;
		XMStoreFloat3(&targetPos_, vMovePos);
		targetPos_.y = height_;

		Audio::Play(hSound_[1]);
		ChangeState(S_SHOT);
	}
		
	time_++;

}

void UfoObstacle::UpdateShot()
{
	if (stateEnter_) {
		stateEnter_ = false;
		time_ = 0;

	}

	//Targetへの移動
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vTar = XMLoadFloat3(&targetPos_);
	XMVECTOR vMovePos = vTar - vPos;
	vMovePos = XMVector3Normalize(vMovePos) * moveSpeed_;
	leavVec_ = vMovePos;
	vMovePos += vPos;
	XMFLOAT3 fPos;
	XMStoreFloat3(&fPos, vMovePos);
	transform_.position_ = fPos;

	//エフェクト
	EmitterData data;
	data.textureFileName = "Particle/missilePar.png";
	data.position = transform_.position_;
	data.position.y -= 8.7f;
	data.positionRnd = XMFLOAT3(1.0f, 7.2f, 1.0f);
	data.direction = XMFLOAT3(10.0f, -10.0f, 10.0f);
	data.directionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
	data.speed = 0.1f;
	data.speedRnd = 0.0f;
	data.accel = 1.0f;
	data.delay = 0;
	data.number = 30;
	data.gravity = 0.0f;
	data.lifeTime = 5;
	data.color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	data.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	data.size = XMFLOAT2(0.5f, 0.4f);
	data.sizeRnd = XMFLOAT2(0.4f, 0.4f);
	data.scale = XMFLOAT2(0.8f, 0.8f);
	data.isBillBoard = true;
	VFX::Start(data);

	int shotTime = 30;
	if (time_ > shotTime) ChangeState(S_LEAVING);
	time_++;

}

void UfoObstacle::UpdateLeaving()
{
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	vPos += leavVec_;
	XMStoreFloat3(&transform_.position_, vPos);
	transform_.position_.y += leavYmoveSpeed_;

	if (attack_) {
		EmitterData data;
		data.textureFileName = "Particle/cloudA.png";
		data.position = transform_.position_;
		data.position.y -= 8.7f;
		data.positionRnd = XMFLOAT3(0.5f, 7.2f, 0.5f);
		data.direction = XMFLOAT3(10.0f, -10.0f, 10.0f);
		data.directionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
		data.speed = 0.1f;
		data.speedRnd = 0.0f;
		data.accel = 1.0f;
		data.delay = 0;
		data.number = 30 + (rand() % 5);
		data.gravity = 0.0f;
		data.lifeTime = 5;
		data.color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		data.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		data.size = XMFLOAT2(0.2f, 0.2f);
		data.sizeRnd = XMFLOAT2(0.4f, 0.4f);
		data.scale = XMFLOAT2(0.8f, 0.8f);
		data.isBillBoard = true;
		VFX::Start(data);
	}

	if (transform_.position_.y >= 20.0f) KillMe();

}

void UfoObstacle::ChangeState(STATE s)
{
	state_ = s;
	stateEnter_ = true;
}