#pragma once
#include "global.h"
#include "GameObject.h"

//ゲームに登場するシーン
enum SCENE_ID
{
	SCENE_ID_TITLE = 0,
	SCENE_ID_PLAY = 1,
	SCENE_ID_RESULT = 2,
	
	SCENE_ID_MAX,
};

//-----------------------------------------------------------
//シーン切り替えを担当するオブジェクト
//-----------------------------------------------------------
class SceneManager : public GameObject
{
public:

	//コンストラクタ
	//引数：parent	親オブジェクト（基本的にゲームマネージャー）
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//シーン切り替え（実際に切り替わるのはこの次のフレーム）
	//引数：next	次のシーンのID
	void ChangeScene(SCENE_ID next);
	
	//すでにロゴを表示したか/falseまだ
	bool IsLogoShown() { return logoShown_; };
	void SetLogoShown(bool b) { logoShown_ = b; };
	void SetResult(bool b) { result_ = b; };
	bool GetResult() { return result_; };

private:
	SCENE_ID currentSceneID_;	//現在のシーン
	SCENE_ID nextSceneID_;		//次のシーン

	bool logoShown_;			//ロゴ表示したか（falseまだ
	bool result_;				//クリアの場合はtrue
};