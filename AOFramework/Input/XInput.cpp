#include"stdafx.hpp"
#include"XInput.hpp"


using namespace ao;


XInput::XInput()
{
	::SecureZeroMemory(&this->joyData, sizeof(this->joyData));
}


XInput::~XInput()
{

}



//------------------------------------------------------------------------------
/**	@brief		�X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/02	23:33	T.Aoyama	�쐬
//------------------------------------------------------------------------------
DWORD XInput::Update()
{
	DWORD dwResult;
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		//�o�C�u���[�V�����l�ݒ�->������
		XInputSetState(i, &this->joyData[i].vibration);
		this->joyData[i].vibration.wLeftMotorSpeed = 0;
		this->joyData[i].vibration.wRightMotorSpeed = 0;
		//�L�[���̏������ƋL���i�g���K�[�����[�X�̂��߁j
		WORD logInputState = this->joyData[i].inputState.Gamepad.wButtons;
		WORD logAxisState = this->joyData[i].prsAxis;
		BYTE logTrgState = this->joyData[i].trgPress;

		::SecureZeroMemory(&joyData[i].inputState, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XJoyStick.
		dwResult = XInputGetState(i, &joyData[i].inputState);

		if (dwResult != ERROR_SUCCESS){
			return dwResult;
		}

		// Controller is connected
		if (-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE < this->joyData[i].inputState.Gamepad.sThumbLX &&
			-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE < this->joyData[i].inputState.Gamepad.sThumbLY &&
			this->joyData[i].inputState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  &&
			this->joyData[i].inputState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			this->joyData[i].inputState.Gamepad.sThumbLX = 0;
			this->joyData[i].inputState.Gamepad.sThumbLY = 0;
		}

		if (-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE < this->joyData[i].inputState.Gamepad.sThumbRX &&
			-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE < this->joyData[i].inputState.Gamepad.sThumbRY &&
			this->joyData[i].inputState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			this->joyData[i].inputState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			this->joyData[i].inputState.Gamepad.sThumbRX = 0;
			this->joyData[i].inputState.Gamepad.sThumbRY = 0;
		}

		this->joyData[i].prsAxis = 0;
		this->joyData[i].prsAxis |= (this->joyData[i].inputState.Gamepad.sThumbLX != 0) << 3;
		this->joyData[i].prsAxis |= (this->joyData[i].inputState.Gamepad.sThumbLY != 0) << 2;
		this->joyData[i].prsAxis |= (this->joyData[i].inputState.Gamepad.sThumbRX != 0) << 1;
		this->joyData[i].prsAxis |= (this->joyData[i].inputState.Gamepad.sThumbRY != 0);

		this->joyData[i].trgPress = 0;
		this->joyData[i].trgPress |= (this->joyData[i].inputState.Gamepad.bLeftTrigger != 0) << 1;
		this->joyData[i].trgPress |= (this->joyData[i].inputState.Gamepad.bRightTrigger != 0);

		//�g���K�[�̃g���K�[�����[�X���o
		this->joyData[i].trgTR = 0;
		this->joyData[i].trgTR |= ((this->joyData[i].trgPress ^ logTrgState) & this->joyData[i].trgPress) << 2;
		this->joyData[i].trgTR |= (this->joyData[i].trgPress ^ logTrgState) & logTrgState;

		//���̃g���K�[�����[�X���o
		this->joyData[i].trgAxis = (this->joyData[i].prsAxis ^ logAxisState) & this->joyData[i].prsAxis;
		this->joyData[i].rlsAxis = (this->joyData[i].prsAxis ^ logAxisState) & logAxisState;

		//�{�^���̃g���K�[�����[�X���o
		this->joyData[i].trgState = (this->joyData[i].inputState.Gamepad.wButtons ^ logInputState) & this->joyData[i].inputState.Gamepad.wButtons;
		this->joyData[i].rlsState = (this->joyData[i].inputState.Gamepad.wButtons ^ logInputState) & logInputState;
	}

	return dwResult;
}


//------------------------------------------------------------------------------
/**	@brief		�o�C�u���[�V�����̐ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:22	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void XInput::SetVibration(
	BYTE _joyId,
	USHORT _leftVibRate,
	USHORT _rightVibRate
	)
{
	//�I�[�o�[�t���[�h�~
	_leftVibRate &= 0xFFFF;
	_rightVibRate &= 0xFFFF;
	this->joyData[_joyId].vibration.wLeftMotorSpeed = min(65535, _leftVibRate + this->joyData[_joyId].vibration.wLeftMotorSpeed);
	this->joyData[_joyId].vibration.wRightMotorSpeed = min(65535, _rightVibRate + this->joyData[_joyId].vibration.wRightMotorSpeed);
}