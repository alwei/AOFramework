/*************************************************************
*	@file   	Keyboard.hpp
*	@brief  	Keyboard�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2013/07/23
*************************************************************/

#ifndef _Include_CKeyboard_hpp_	// �C���N���[�h�K�[�h
#define _Include_CKeyboard_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include"../Utility/NonCopyable.hpp"

enum class KeyCode : DWORD
{
	A = 'A',
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	F1 = VK_F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,
	TAB = VK_TAB,
	SPACE = VK_SPACE,
	RETURN = VK_RETURN,
	ESCAPE = VK_ESCAPE,
	BACK = VK_BACK,
	LCONTROL = VK_LCONTROL,
	RCONTROL = VK_RCONTROL,
	LMENU = VK_LMENU,
	RMENU = VK_RMENU,
	LSHIFT = VK_LSHIFT,
	RSHIFT = VK_RSHIFT,
	DEL = VK_DELETE
};


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  Keyboard
	//	@brief  �L�[�{�[�h���͊Ǘ��N���X
	//	@author	Tatsunori Aoyama
	//	@date	2013/07/23
	------------------------------------------------------------*/
	class Keyboard : private NonCopyable
	{
	public:
		~Keyboard();

		/*!-----------------------------------------------------------
		//	@brief		�V���O���g���ɂ����̌Ăяo��
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		static	Keyboard*	GetInstance(){
			static	Keyboard	singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		�v���X�L�[�擾
		//	@param[in]	_keyCode	�L�[�R�[�h
		//	@return		0�ȊO�F������Ă���@0�F������Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		BYTE	GetPrsKey(const KeyCode& _keyCode){
			return keyData.prsState[static_cast<DWORD>(_keyCode)];
		}

		/*!-----------------------------------------------------------
		//	@brief		�g���K�[�L�[�擾
		//	@param[in]	_keyCode	�L�[�R�[�h
		//	@return		0�ȊO�F�����ꂽ�@0�F������Ă��Ȃ�����
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		BYTE	GetTrgKey(const KeyCode& _keyCode){
			return keyData.trgState[static_cast<DWORD>(_keyCode)];
		}

		/*!-----------------------------------------------------------
		//	@brief		�����[�X�L�[�擾
		//	@param[in]	_keyCode	�L�[�R�[�h
		//	@return		0�ȊO�F�����ꂽ�@0�F������Ȃ�����
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		BYTE	GetRlsKey(const KeyCode& _keyCode){
			return keyData.rlsState[static_cast<DWORD>(_keyCode)];
		}

		/*!-----------------------------------------------------------
		//	@brief		�L�[�{�[�h�̏�ԍX�V���\�b�h
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		void	Update();

	private:	/// ��`
		static	const int	MAX_KEY_CODE = 256;

		/*!-----------------------------------------------------------
		//	@struct	KeyboardState
		//	@brief	�L�[�{�[�h�i�[���\����
		//	@author	Tatsunori Aoyama
		//	@date	2013/07/23
		------------------------------------------------------------*/
		typedef struct _InputKeyboard
		{
			BYTE	prsState[256];
			BYTE	trgState[256];
			BYTE	rlsState[256];
		}InputKeyboard;

	private:
		InputKeyboard	keyData;	/// �L�[�{�[�h�i�[���

	private:
		Keyboard();
	};

}

#endif // _Include_CKeyboard_hpp_