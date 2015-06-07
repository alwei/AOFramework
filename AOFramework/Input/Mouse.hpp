/*************************************************************
*	@file   	Mouse.hpp
*	@brief  	Mouse�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2013/07/31
*************************************************************/

#ifndef _Include_Mouse_hpp_	// �C���N���[�h�K�[�h
#define _Include_Mouse_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include<queue>
#include"../Utility/NonCopyable.hpp"

class MouseCode
{
public:
	enum class Press : long{
		Left = 0x0000004,
		Middle = 0x0000002,
		Right = 0x0000001
	};

	enum class Trigger : long{
		Left = 0x0000040,
		Middle = 0x0000020,
		Right = 0x0000010
	};

	enum class Release : long{
		Left = 0x0000400,
		Middle = 0x0000200,
		Right = 0x0000100
	};
};


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  Mouse
	//	@brief  �}�E�X���͊Ǘ��N���X
	//	@author	Tatsunori Aoyama
	//	@date	2013/07/31
	------------------------------------------------------------*/
	class Mouse : private NonCopyable
	{
	public:
		~Mouse();

		/*!-----------------------------------------------------------
		//	@brief		�V���O���g���ɂ��Ăяo��
		//	@return		���g�̎Q��
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/31
		------------------------------------------------------------*/
		static	Mouse*	GetInstance(){
			static	Mouse singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E�n���h���Z�b�g
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/31
		------------------------------------------------------------*/
		void	SetWindowHandle(HWND _hWnd){
			this->hWnd = _hWnd;
		}

		/*!-----------------------------------------------------------
		//	@brief		�}�E�X�̃X�N���[�����΍��W�擾
		//	@param[out]	_pPos	�o�͂���}�E�X�̈ړ����΍��W
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/05
		------------------------------------------------------------*/
		void	GetRelativeCursorPos(LPPOINT _pPos);

		/*!-----------------------------------------------------------
		//	@brief		�}�E�X�̃X�N���[����΍��W�擾
		//	@param[in]	_pPos	�o�͂���}�E�X�̃X�N���[����΍��W[�ȗ���]
		//	@return		�o�͂���}�E�X�̃X�N���[����΍��W
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/01
		------------------------------------------------------------*/
		CONST LPPOINT	GetAbsoluteCursorPos(LPPOINT _pPos = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�}�E�X�̒����z�C�[���̉�]�p�x�擾
		//	@return		�z�C�[���̉�]�p�x(-1or1)
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/01
		------------------------------------------------------------*/
		SHORT	GetWheelRotation();

		/*!-----------------------------------------------------------
		//	@brief		�}�E�X�̓��͏�Ԏ擾
		//	@param[in]	_mask	�}�X�N�R�[�h
		//	@return		���͏�Ԃ̃r�b�g
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/31
		------------------------------------------------------------*/
		BYTE	GetPressButton(MouseCode::Press _mask){
			return (BYTE)(this->inputState&(int)_mask);
		}

		/*!-----------------------------------------------------------
		//	@brief		�}�E�X�̓��͏�Ԏ擾
		//	@param[in]	_mask	�}�X�N�R�[�h
		//	@return		���͏�Ԃ̃r�b�g
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/31
		------------------------------------------------------------*/
		BYTE	GetTriggerButton(MouseCode::Trigger _mask){
			return (BYTE)(this->inputState&(int)_mask) >> 4;
		}

		/*!-----------------------------------------------------------
		//	@brief		�}�E�X�̓��͏�Ԏ擾
		//	@param[in]	_mask	�}�X�N�R�[�h
		//	@return		���͏�Ԃ̃r�b�g
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/31
		------------------------------------------------------------*/
		BYTE	GetReleaseButton(MouseCode::Release _mask){
			return (BYTE)((this->inputState&(int)_mask) >> 8);
		}

		/*!-----------------------------------------------------------
		//	@brief		�}�E�X�̓��͏�ԍX�V
		//	@param[in]	_hWnd	�E�B���h�E�n���h��(�v���V�[�W��������)[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/31
		------------------------------------------------------------*/
		void	Update(CONST HWND _hWnd = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�}�E�X�z�C�[���̍X�V
		//	@note		�v���V�[�W���ōX�V���Ȃ��Ɛ��x���K�^����
		//	@param[in]	_zDelta	�z�C�[���̏��
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/01
		------------------------------------------------------------*/
		void	UpdateWheel(SHORT _zDelta){
			this->zQueue.push(_zDelta / 120);
		}

	private:
		HWND	hWnd;
		BOOL	isNowActive;
		SHORT	inputState;
		POINT	logCursorPos;
		POINT	nowCursorPos;
		std::queue<SHORT>	zQueue;

	private:
		/*!-----------------------------------------------------------
		//	@brief		�}�E�X���W�̍X�V
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/05
		------------------------------------------------------------*/
		inline	void	UpdateCursorPos();

	private:
		Mouse();
	};
}

#endif // _Include_Mouse_hpp_