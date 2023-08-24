#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"

class Player;

class Missile :
    public GameObject
{	
	XMVECTOR position;				//座標
	XMVECTOR velocity;				//速度
	XMVECTOR target;				//ターゲット
	float maxCentripetalAccel;		//最大向心加速度
	float propulsion;				// 推進力
	float countPerMeter;			//メートルあたりのカウント
	float speed;					//スピード
	float damping;					//減衰
	float impact;					//衝突距離
	int hModel_;
	Player* pPlayer_;
    EmitterData data;
    EmitterData dataExp;


public:
	Missile(GameObject* parent);
	~Missile();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetTarget(float x, float y, float z);




    struct Vector3
    {
        float x, y, z;
    };

    struct Random32
    {
        float GetFloat(float min, float max)
        {
            // ランダムな値を返すロジックを実装
            return rand() % 100 * 0.001f;
        }
    };

    struct ParticleParameters
    {
        float attenuation;
        float randomWalk;
        float damping;
        float gravity;
        float radius;
    };

    struct Particle
    {
        void Emit(Vector3 position, Vector3 velocity, Vector3 constantForce, float randomAccelStrengh, float damping, 
            float attenuation, float radius)
        {
            this->position = position;
            this->velocity = velocity;
            this->constantAccel = constantForce;
            this->randomAccelStrengh = randomAccelStrengh;
            this->damping = damping;
            this->attenuation = attenuation;
            this->radius = radius;
            time = 0.0f;
        }

        void Update(float deltaTime, Vector3 billboardUp, Vector3 billboardRight, Random32& random)
        {
            velocity.x *= 1.0f - (damping * deltaTime);
            velocity.y *= 1.0f - (damping * deltaTime);
            velocity.z *= 1.0f - (damping * deltaTime);
            float min = -0.5f * randomAccelStrengh;
            float max = -min;
            Vector3 a = {
                random.GetFloat(min, max),
                random.GetFloat(min, max),
                random.GetFloat(min, max) };
            Add(a, a, constantAccel);
            Madd(velocity, a, deltaTime);
            Madd(position, velocity, deltaTime);
            radius *= 1.0f - (attenuation * deltaTime);
            time += deltaTime;

            // 頂点座標計算
            const float cos30 = 0.866025403784439f;
            float rCos30x2 = radius * cos30 * 2.0f;
            Vector3 tmpUp, tmpRight, upCenter;
            SetMul(tmpUp, billboardUp, radius);
            SetMul(tmpRight, billboardRight, rCos30x2);
            SetAdd(upCenter, position, tmpUp);
            SetMsub(p0, position, tmpUp, 2.0f);
            SetSub(p1, upCenter, tmpRight);
            SetAdd(p2, upCenter, tmpRight);
        }

        Vector3 position;
        Vector3 velocity;
        float time;
        float radius;
        Vector3 p0, p1, p2;
        Vector3 constantAccel;
        float randomAccelStrengh;
        float damping;
        float attenuation;
        float deltaTime = 1.0f;

    private:
        static void Add(Vector3& result, const Vector3& a, const Vector3& b)
        {
            result.x = a.x + b.x;
            result.y = a.y + b.y;
            result.z = a.z + b.z;
        }

        static void Madd(Vector3& result, const Vector3& a, float scalar)
        {
            result.x = a.x + scalar;
            result.y = a.y + scalar;
            result.z = a.z + scalar;
        }

        static void SetMul(Vector3& result, const Vector3& vec, float scalar)
        {
            result.x = vec.x * scalar;
            result.y = vec.y * scalar;
            result.z = vec.z * scalar;
        }

        static void SetAdd(Vector3& result, const Vector3& a, const Vector3& b)
        {
            result.x = a.x + b.x;
            result.y = a.y + b.y;
            result.z = a.z + b.z;
        }

        static void SetMsub(Vector3& result, const Vector3& a, const Vector3& b, float scalar)
        {
            result.x = a.x - b.x * scalar;
            result.y = a.y - b.y * scalar;
            result.z = a.z - b.z * scalar;
        }

        static void SetSub(Vector3& result, const Vector3& a, const Vector3& b)
        {
            result.x = a.x - b.x;
            result.y = a.y - b.y;
            result.z = a.z - b.z;
        }
    };




};

