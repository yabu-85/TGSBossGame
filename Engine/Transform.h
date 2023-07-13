#pragma once
#include <DirectXMath.h>

using namespace DirectX;


//-----------------------------------------------------------
//位置、向き、拡大率などを管理するクラス
//-----------------------------------------------------------
class Transform
{
public:
	XMMATRIX matTranslate_;	//移動行列
	XMMATRIX matRotate_;	//回転行列	
	XMMATRIX matScale_;		//拡大行列
	XMFLOAT3 position_;		//位置
	XMFLOAT3 rotate_;		//向き
	XMFLOAT3 scale_;		//拡大率
	Transform * pParent_;	//親オブジェクトの情報

	//コンストラクタ
	Transform();

	//デストラクタ
	~Transform();

	//各行列の計算
	//引数：なし
	//戻値：なし
	void Calclation();

	//ワールド行列を取得
	//引数：なし
	//戻値：その時点でのワールド行列
	XMMATRIX GetWorldMatrix();


	static XMFLOAT3 Float3Add(XMFLOAT3 a, XMFLOAT3 b)
	{
		return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
	}
};

//追加部分
inline XMFLOAT3 operator += (XMFLOAT3& p, const XMVECTOR& v) {
	XMVECTOR pv = XMLoadFloat3(&p);
	pv += v;
	XMStoreFloat3(&p, pv);
	return p;
}

inline XMFLOAT3 operator + (const XMFLOAT3& v1, const XMVECTOR& v2) {
	XMVECTOR pv = XMLoadFloat3(&v1);
	pv += v2;
	XMFLOAT3 out;
	XMStoreFloat3(&out, pv);
	return out;
}

inline float Length(XMVECTOR v) {
	XMVECTOR lenVec = XMVector3Length(v);
	return XMVectorGetX(lenVec);
}

inline XMVECTOR operator - (const XMFLOAT3& p1, const XMFLOAT3& p2) {
	XMVECTOR p1v = XMLoadFloat3(&p1);
	XMVECTOR p2v = XMLoadFloat3(&p2);
	return p1v - p2v;
}
