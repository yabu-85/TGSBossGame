#pragma once
#include "global.h"
#include "GameObject.h"

//�Q�[���ɓo�ꂷ��V�[��
enum SCENE_ID
{
	SCENE_ID_TITLE = 0,
	SCENE_ID_PLAY = 1,
	SCENE_ID_RESULT = 2,
	
	SCENE_ID_MAX,
};

//-----------------------------------------------------------
//�V�[���؂�ւ���S������I�u�W�F�N�g
//-----------------------------------------------------------
class SceneManager : public GameObject
{
public:

	//�R���X�g���N�^
	//�����Fparent	�e�I�u�W�F�N�g�i��{�I�ɃQ�[���}�l�[�W���[�j
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�V�[���؂�ւ��i���ۂɐ؂�ւ��̂͂��̎��̃t���[���j
	//�����Fnext	���̃V�[����ID
	void ChangeScene(SCENE_ID next);
	
	//���łɃ��S��\��������/false�܂�
	bool IsLogoShown() { return logoShown_; };
	void SetLogoShown(bool b) { logoShown_ = b; };
	void SetResult(bool b) { result_ = b; };
	bool GetResult() { return result_; };

private:
	SCENE_ID currentSceneID_;	//���݂̃V�[��
	SCENE_ID nextSceneID_;		//���̃V�[��

	bool logoShown_;			//���S�\���������ifalse�܂�
	bool result_;				//�N���A�̏ꍇ��true
};