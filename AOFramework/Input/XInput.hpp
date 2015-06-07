/*************************************************************
*	@file   	XInput.hppao
*	@brief  	XInput�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2013/07/22
*************************************************************/

#ifndef _Include_XInput_hpp_	// �C���N���[�h�K�[�h
#define _Include_XInput_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include<winnt.h>
#include<XInput.h>
#include"../Utility/NonCopyable.hpp"

//1.4����Window8�������s�ł��Ȃ�����
//��O�̃o�[�W�����̃��C�u�������g�p
#pragma comment(lib,"Xinput9_1_0.lib")


enum class JoypadId : DWORD
{
	UP = XINPUT_GAMEPAD_DPAD_UP,
	DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
	LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
	RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
	START = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK,
	LT = XINPUT_GAMEPAD_LEFT_THUMB,
	RT = XINPUT_GAMEPAD_RIGHT_THUMB,
	LS = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RS = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	A = XINPUT_GAMEPAD_A,
	B = XINPUT_GAMEPAD_B,
	X = XINPUT_GAMEPAD_X,
	Y = XINPUT_GAMEPAD_Y
};


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  XInput
	//	@brief  �W���C�X�e�B�b�N���̓N���X
	//	@author	Tatsunori Aoyama
	//	@date	2013/07/22
	------------------------------------------------------------*/
	class XInput : private NonCopyable
	{
	public:
		~XInput();

		/*!-----------------------------------------------------------
		//	@brief		�V���O���g���ɂ����̌Ăяo��
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		static	XInput*	GetInstance(){
			static XInput singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		�R���g���[���[�̏�ԍX�V
		//	@return		ERROR_SUCCESS�F�X�V�����@����ȊO�F�X�V���s
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/22
		------------------------------------------------------------*/
		DWORD	Update();

		/*!-----------------------------------------------------------
		//	@brief		�w�肵���{�^���ԍ��̉�����Ԏ擾
		//	@param[in]	_joyId		�W���C�X�e�B�b�NID
		//	@param[in]	_buttonId	�}�X�N����{�^��ID
		//	@return		���������{�^���ԍ��̃v���X���
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		DWORD	GetPrsButton(BYTE _joyId, const JoypadId& _buttonId){
			return (this->joyData[_joyId].inputState.Gamepad.wButtons)&static_cast<DWORD>(_buttonId);
		}

		/*!-----------------------------------------------------------
		//	@brief		�w�肵���{�^���ԍ��̃g���K�[��Ԏ擾
		//	@param[in]	_joyId		�W���C�X�e�B�b�NID
		//	@param[in]	_buttonId	�}�X�N����{�^��ID
		//	@return		���������{�^���ԍ��̃g���K�[���
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		DWORD	GetTrgButton(BYTE _joyId, const JoypadId& _buttonId){
			return (this->joyData[_joyId].trgState)&static_cast<DWORD>(_buttonId);
		}

		/*!-----------------------------------------------------------
		//	@brief		�w�肵���{�^���ԍ��̃����[�X��Ԏ擾
		//	@param[in]	_joyId		�W���C�X�e�B�b�NID
		//	@param[in]	_buttonId	�}�X�N����{�^��ID
		//	@return		���������{�^���ԍ��̃����[�X���
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		DWORD	GetRlsButton(BYTE _joyId, const JoypadId& _buttonId){
			return (this->joyData[_joyId].rlsState)&static_cast<DWORD>(_buttonId);
		}

		/*!-----------------------------------------------------------
		//	@brief		���g���K�[�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		�������܂�Ă���[���i0�`255�j
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		BYTE	GetPrsLT(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.bLeftTrigger;
		}

		/*!-----------------------------------------------------------
		//	@brief		���g���K�[�̃g���K�[�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		0�ȊO�F���͂���@0�F���͂Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/03
		------------------------------------------------------------*/
		BYTE	GetTrgLT(BYTE _joyId){
			return (this->joyData[_joyId].trgTR & 0x08);
		}

		/*!-----------------------------------------------------------
		//	@brief		���g���K�[�̃g���K�[�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		0�ȊO�F���͂���@0�F���͂Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/03
		------------------------------------------------------------*/
		BYTE	GetRlsLT(BYTE _joyId){
			return (this->joyData[_joyId].trgTR & 0x02);
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�g���K�[�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		�������܂�Ă���[���i0�`255�j
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		BYTE	GetPrsRT(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.bRightTrigger;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�g���K�[�̃g���K�[�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		0�ȊO�F���͂���@0�F���͂Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/03
		------------------------------------------------------------*/
		BYTE	GetTrgRT(BYTE _joyId){
			return (this->joyData[_joyId].trgTR & 0x04);
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�g���K�[�̃g���K�[�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		0�ȊO�F���͂���@0�F���͂Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/03
		------------------------------------------------------------*/
		BYTE	GetRlsRT(BYTE _joyId){
			return (this->joyData[_joyId].trgTR & 0x01);
		}

		/*!-----------------------------------------------------------
		//	@brief		���X�e�B�b�N��X���擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		���X�e�B�b�N��X���擾
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		SHORT	GetAxisLX(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbLX;
		}

		/*!-----------------------------------------------------------
		//	@brief		���X�e�B�b�N��Y���擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		���X�e�B�b�N��Y���擾
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		SHORT	GetAxisLY(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbLY;
		}

		/*!-----------------------------------------------------------
		//	@brief		���X�e�B�b�N�̐��K�����ꂽX���擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		���X�e�B�b�N�̐��K�����ꂽX���擾
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		FLOAT	GetAxisNormalizeLX(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbLX / 32768.0f;
		}

		/*!-----------------------------------------------------------
		//	@brief		���X�e�B�b�N�̐��K�����ꂽY���擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		���X�e�B�b�N�̐��K�����ꂽY���擾
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		FLOAT	GetAxisNormalizeLY(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbLY / 32768.0f;
		}

		/*!-----------------------------------------------------------
		//	@brief		���X�e�B�b�N��X���g���K�[�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		true�F�|���ꂽ�@false�F�|����Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetTrgAxisLX(BYTE _joyId){
			return (this->joyData[_joyId].trgAxis & 0x0008) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		���X�e�B�b�N��Y���g���K�[�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		true�F�|���ꂽ�@false�F�|����Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetTrgAxisLY(BYTE _joyId){
			return (this->joyData[_joyId].trgAxis & 0x0004) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		���X�e�B�b�N��X�������[�X�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		true�F�|���ꂽ�@false�F�|����Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetRlsAxisLX(BYTE _joyId){
			return (this->joyData[_joyId].rlsAxis & 0x0008) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		���X�e�B�b�N��Y�������[�X�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		true�F�|���ꂽ�@false�F�|����Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetRlsAxisLY(BYTE _joyId){
			return (this->joyData[_joyId].rlsAxis & 0x0004) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		���X�e�B�b�N��X���擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		�E�X�e�B�b�N��X���̌X��
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		SHORT	GetAxisRX(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbRX;
		}

		/*!-----------------------------------------------------------
		//	@brief		���X�e�B�b�N��Y���擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		���X�e�B�b�N��Y���̌X��
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		SHORT	GetAxisRY(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbRY;
		}

		/*!-----------------------------------------------------------
		//	@brief		���X�e�B�b�N�̐��K�����ꂽX���擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		�E�X�e�B�b�N�̐��K�����ꂽX���̌X��
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		FLOAT	GetAxisNormalizeRX(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbRX / 32768.0f;
		}

		/*!-----------------------------------------------------------
		//	@brief		���X�e�B�b�N�̐��K�����ꂽY���擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		���X�e�B�b�N�̐��K�����ꂽY���̌X��
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		FLOAT	GetAxisNormalizeRY(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbRY / 32768.0f;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�X�e�B�b�N��X���g���K�[�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		true�F�|���ꂽ	false�F�|����Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetTrgAxisRX(BYTE _joyId){
			return (this->joyData[_joyId].trgAxis & 0x0002) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�X�e�B�b�N��Y���g���K�[�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		true�F�|���ꂽ	false�F�|����Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetTrgAxisRY(BYTE _joyId){
			return (this->joyData[_joyId].trgAxis & 0x0001) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�X�e�B�b�N��X���g���K�[�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		true�F�|���ꂽ	false�F�|����Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetRlsAxisRX(BYTE _joyId){
			return (this->joyData[_joyId].rlsAxis & 0x0002) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�X�e�B�b�N��Y���g���K�[�擾
		//	@param[in]	_joyId	�W���C�X�e�B�b�NID
		//	@return		true�F�|���ꂽ	false�F�|����Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetRlsAxisRY(BYTE _joyId){
			return (this->joyData[_joyId].rlsAxis & 0x0001) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		�o�C�u���[�V�����̐ݒ�
		//	@param[in]	_joyId			�W���C�X�e�B�b�NID
		//	@param[in]	_leftVibRate	���U���̒l(0�`65535)
		//	@param[in]	_rightVibRate	�E�U���̒l(0�`65535)
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/24
		------------------------------------------------------------*/
		void	SetVibration(
			BYTE _joyId,
			USHORT _leftVibRate,
			USHORT _rightVibRate);

	private:	//��`
		static	const int MAX_CONTROLLERS = 4;

		/*!-----------------------------------------------------------
		//	@struct	TXJoyStickJoyStick
		//	@brief	�R���g���[���[�i�[�\����
		//	@author	Tatsunori Aoyama
		//	@date	2013/07/23
		------------------------------------------------------------*/
		typedef struct _JoyStickData
		{
			DWORD				id;				///< �W���C�X�e�B�b�NID
			XINPUT_STATE		inputState;		///< ���͏��
			XINPUT_VIBRATION	vibration;		///< �U�����
			WORD				prsAxis;		///< �����͏��
			BYTE				trgPress;		///< �g���K�[�̓��͏��
			BYTE				trgTR;			///< �g���K�[�̃g���K�[�����[�X
			WORD				trgAxis;		///< �g���K�[��
			WORD				rlsAxis;		///< �����[�X��
			WORD				trgState;		///< �g���K�[���
			WORD				rlsState;		///< �����[�X���
		}JoyStickData;

	private:	/// �W���C�X�e�B�b�N�i�[���
		JoyStickData	joyData[MAX_CONTROLLERS];	///< �W���C�X�e�B�b�N�f�[�^�i�S�j

	private:
		XInput();
	};
}

#endif // _Include_XInput_hpp_