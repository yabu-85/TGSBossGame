#include "Global.h"
#include "Model.h"
#include <algorithm>

//3Dモデル（FBXファイル）を管理する
namespace Model
{
	//ロード済みのモデルデータ一覧
	std::vector<ModelData*>	_datas;

	//初期化
	void Initialize()
	{
		AllRelease();
	}

	//モデルをロード
	int Load(std::string fileName, int _order, int _type)
	{
		ModelData* pData = new ModelData;

		if (_order >= 0) {

			pData->drawOrder_ = _order;
			pData->shadeType_ = _type;

		}

		//開いたファイル一覧から同じファイル名のものが無いか探す
		bool isExist = false;
		for (int i = 0; i < _datas.size(); i++)
		{
			//すでに開いている場合
			if (_datas[i] != nullptr && _datas[i]->fileName == fileName)
			{
				pData->pFbx = _datas[i]->pFbx;
				isExist = true;
				break;
			}
		}

		//新たにファイルを開く
		if (isExist == false)
		{
			pData->pFbx = new Fbx;
			if (FAILED(pData->pFbx->Load(fileName)))
			{
				//開けなかった
				SAFE_DELETE(pData->pFbx);
				SAFE_DELETE(pData);
				return -1;
			}

			//無事開けた
			pData->fileName = fileName;
		}

		//使ってない番号が無いか探す
		for (int i = 0; i < _datas.size(); i++)
		{
			if (_datas[i] == nullptr)
			{
				_datas[i] = pData;
				return i;
			}
		}

		//新たに追加
		_datas.push_back(pData);
		return (int)_datas.size() - 1;
	}

	//描画
	void Draw(int handle, int type)
	{
		if (handle < 0 || handle >= _datas.size() || _datas[handle] == nullptr)
		{
			return;
		}

		//アニメーションを進める
		if(_datas[handle]->isAimeStop == false) _datas[handle]->nowFrame += _datas[handle]->animSpeed;

		//最後までアニメーションしたら戻す
		if (_datas[handle]->nowFrame > (float)_datas[handle]->endFrame)
			_datas[handle]->nowFrame = (float)_datas[handle]->startFrame;

		/* Blend
		if (e->isBlending) {
			//アニメーションを進める
			e->blendNowFrame += e->animSpeed;

			//最後までアニメーションしたら戻す
			if (e->blendNowFrame > (float)e->blendEndFrame)
				e->blendNowFrame = (float)e->blendStartFrame;

			e->pFbx->Draw(e->transform, (int)e->nowFrame,
				(int)e->blendNowFrame, (float)e->blendWeight);

			return;
		}
		*/

		if (_datas[handle]->pFbx)
		{
			// 現在のアニメーションフレームでモデルを描画
			_datas[handle]->pFbx->Draw(_datas[handle]->transform, (int)_datas[handle]->nowFrame, type);

		}
	}

	void DrawOrder()
	{
		for (ModelData *e : _datas) {
			if (e->drawOrder_ != 0) continue;

			if (e->drawOrder_ == -1 || e == nullptr) continue;

			//アニメーションを進める
			if (e->isAimeStop == false) e->nowFrame += e->animSpeed;

			//最後までアニメーションしたら戻す
			if (e->nowFrame > (float)e->endFrame)
				e->nowFrame = (float)e->startFrame;

			if (e->pFbx)
			{
				// 現在のアニメーションフレームでモデルを描画
				e->pFbx->Draw(e->transform, (int)e->nowFrame, e->shadeType_);

			}
		}

		for (ModelData* e : _datas) {
			if (e->drawOrder_ != 1) continue;

			if (e->drawOrder_ == -1 || e == nullptr) continue;

			//アニメーションを進める
			if (e->isAimeStop == false) e->nowFrame += e->animSpeed;

			//最後までアニメーションしたら戻す
			if (e->nowFrame > (float)e->endFrame)
				e->nowFrame = (float)e->startFrame;

			if (e->pFbx)
			{
				// 現在のアニメーションフレームでモデルを描画
				e->pFbx->Draw(e->transform, (int)e->nowFrame, e->shadeType_);

			}
		}


	}


	//任意のモデルを開放
	void Release(int handle)
	{
		if (handle < 0 || handle >= _datas.size() || _datas[handle] == nullptr)
		{
			return;
		}

		//同じモデルを他でも使っていないか
		bool isExist = false;
		for (int i = 0; i < _datas.size(); i++)
		{
			//すでに開いている場合
			if (_datas[i] != nullptr && i != handle && _datas[i]->pFbx == _datas[handle]->pFbx)
			{
				isExist = true;
				break;
			}
		}

		//使ってなければモデル解放
		if (isExist == false )
		{
			SAFE_DELETE(_datas[handle]->pFbx);
		}


		SAFE_DELETE(_datas[handle]);
	}


	//全てのモデルを解放
	void AllRelease()
	{
		for (int i = 0; i < _datas.size(); i++)
		{
			if (_datas[i] != nullptr)
			{
				Release(i);
			}
		}
		_datas.clear();
	}

	void SetAnimeStop(int handle, bool b)
	{
		_datas[handle]->SetAnimeStop(b);
	}

	//アニメーションのフレーム数をセット
	void SetBlendingAnimFrame(int handle, int startFrame1, int endFrame1, int startFrame2, int endFrame2, float animSpeed, float blendWeight)
	{
		_datas[handle]->SetBlendingAnimFrame(startFrame1, endFrame1, startFrame2, endFrame2, animSpeed, blendWeight);
	}

	//アニメーションのフレーム数をセット
	void SetAnimFrame(int handle, int startFrame, int endFrame, float animSpeed)
	{
		_datas[handle]->SetAnimFrame(startFrame, endFrame, animSpeed);
	}


	//現在のアニメーションのフレームを取得
	int GetAnimFrame(int handle)
	{
		return (int)_datas[handle]->nowFrame;
	}


	//任意のボーンの位置を取得
	XMFLOAT3 GetBonePosition(int handle, std::string boneName)
	{
		XMFLOAT3 pos = _datas[handle]->pFbx->GetBonePosition(boneName);
		XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&pos), _datas[handle]->transform.GetWorldMatrix());
		XMStoreFloat3(&pos, vec);
		return pos;
	}

	XMFLOAT3 GetBoneAnimPosition(int handle, std::string boneName)
	{
		XMFLOAT3 pos = _datas[handle]->pFbx->GetBoneAnimPosition(boneName, _datas[handle]->nowFrame);
		XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&pos), _datas[handle]->transform.GetWorldMatrix());
		XMStoreFloat3(&pos, vec);
		return pos;
	}

	//ワールド行列を設定
	void SetTransform(int handle, Transform & transform)
	{
		if (handle < 0 || handle >= _datas.size())
		{
			return;
		}

		_datas[handle]->transform = transform;
	}


	//ワールド行列の取得
	XMMATRIX GetMatrix(int handle)
	{
		return _datas[handle]->transform.GetWorldMatrix();
	}


	//レイキャスト（レイを飛ばして当たり判定）
	void RayCast(int handle, RayCastData *data)
	{
			XMFLOAT3 target = Transform::Float3Add(data->start, data->dir);
			XMMATRIX matInv = XMMatrixInverse(nullptr, _datas[handle]->transform.GetWorldMatrix());
			XMVECTOR vecStart = XMVector3TransformCoord(XMLoadFloat3(&data->start), matInv);
			XMVECTOR vecTarget = XMVector3TransformCoord(XMLoadFloat3(&target), matInv);
			XMVECTOR vecDir = vecTarget - vecStart;

			XMStoreFloat3(&data->start, vecStart);
			XMStoreFloat3(&data->dir, vecDir);

			_datas[handle]->pFbx->RayCast(data); 
	}
}