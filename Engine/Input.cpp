#include "Input.h"
#include "Global.h"

namespace Input
{
	//�E�B���h�E�n���h��
	HWND	hWnd_;

	//DirectInput�I�u�W�F�N�g
	LPDIRECTINPUT8			pDInput_;

	//�L�[�{�[�h
	LPDIRECTINPUTDEVICE8	pKeyDevice_;	//�f�o�C�X�I�u�W�F�N�g
	BYTE keyState_[256];					//���݂̊e�L�[�̏��
	BYTE prevKeyState_[256];				//�O�t���[���ł̊e�L�[�̏��

	//�}�E�X
	LPDIRECTINPUTDEVICE8	pMouseDevice_;	//�f�o�C�X�I�u�W�F�N�g
	DIMOUSESTATE mouseState_;				//�}�E�X�̏��
	DIMOUSESTATE prevMouseState_;			//�O�t���[���̃}�E�X�̏��
	POINT mousePos_;							//�}�E�X�J�[�\���̈ʒu

	//������
	void Initialize(HWND hWnd)
	{
		//�E�B���h�E�n���h��
		hWnd_ = hWnd;

		//DirectInput�{��
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&pDInput_, nullptr);

		//�L�[�{�[�h
		pDInput_->CreateDevice(GUID_SysKeyboard, &pKeyDevice_, nullptr);
		pKeyDevice_->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice_->SetCooperativeLevel(NULL, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		//�}�E�X
		pDInput_->CreateDevice(GUID_SysMouse, &pMouseDevice_, nullptr);
		pMouseDevice_->SetDataFormat(&c_dfDIMouse);
		pMouseDevice_->SetCooperativeLevel(hWnd_, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	}


	//�X�V
	void Update()
	{
		//�L�[�{�[�h
		pKeyDevice_->Acquire();
		memcpy(prevKeyState_, keyState_, sizeof(keyState_));
		pKeyDevice_->GetDeviceState(sizeof(keyState_), &keyState_);

		//�}�E�X
		pMouseDevice_->Acquire();
		memcpy(&prevMouseState_, &mouseState_, sizeof(mouseState_));
		pMouseDevice_->GetDeviceState(sizeof(mouseState_), &mouseState_);

	}



	//�J��
	void Release()
	{
		SAFE_RELEASE(pMouseDevice_);
		SAFE_RELEASE(pKeyDevice_);
		SAFE_RELEASE(pDInput_);
	}



	/////////////////////////////�@�L�[�{�[�h���擾�@//////////////////////////////////

	//�L�[��������Ă��邩���ׂ�
	bool IsKey(int keyCode)
	{
		//�����Ă�
		if (keyState_[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}


	//�L�[���������������ׂ�i�������ςȂ��͖����j
	bool IsKeyDown(int keyCode)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if (IsKey(keyCode) && !(prevKeyState_[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}


	//�L�[���������������ׂ�
	bool IsKeyUp(int keyCode)
	{
		//�������ĂȂ��āA�O��͉����Ă�
		if (!IsKey(keyCode) && prevKeyState_[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}


	/////////////////////////////�@�}�E�X���擾�@//////////////////////////////////

	//�}�E�X�̃{�^����������Ă��邩���ׂ�
	bool IsMouseButton(int buttonCode)
	{
		//�����Ă�
		if (mouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//�}�E�X�̃{�^�����������������ׂ�i�������ςȂ��͖����j
	bool IsMouseButtonDown(int buttonCode)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if (IsMouseButton(buttonCode) && !(prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//�}�E�X�̃{�^�����������������ׂ�
	bool IsMouseButtonUp(int buttonCode)
	{
		//�������ĂȂ��āA�O��͉����Ă�
		if (!IsMouseButton(buttonCode) && prevMouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//�}�E�X�J�[�\���̈ʒu���擾
	XMFLOAT3 GetMousePosition()
	{
		XMFLOAT3 result = XMFLOAT3((float)mousePos_.x, (float)mousePos_.y, 0);
		return result;
	}

	//�}�E�X�J�[�\���̈ʒu���Z�b�g
	void SetMousePosition(int x, int y)
	{
		mousePos_.x = x;
		mousePos_.y = y;
	}

	//���̃t���[���ł̃}�E�X�̈ړ��ʂ��擾
	XMFLOAT3 GetMouseMove()
	{
		XMFLOAT3 result = XMFLOAT3((float)mouseState_.lX, (float)mouseState_.lY, (float)mouseState_.lZ);
		return result;
	}


}