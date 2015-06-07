/*************************************************************
*	@file   	Window.hpp
*	@brief  	Window�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/07/30
*************************************************************/

#ifndef _Include_Window_hpp_	// �C���N���[�h�K�[�h
#define _Include_Window_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<tchar.h>
#include<sstream>
#include<Windows.h>
#include<mutex>
#include"ModeLessDialog.hpp"
#include"../Define/SystemDefine.hpp"

namespace ao
{
	class ModeLessDialog;

	/*!-----------------------------------------------------------
	//	@class  Window
	//	@brief  �E�B���h�E�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/30
	------------------------------------------------------------*/
	class Window
	{
	public:
		Window();
		virtual	~Window();

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]  _hInst			�C���X�^���X�n���h��
		//	@param[in]  _nWidth			�E�B���h�E����[�ȗ���]
		//	@param[in]  _nHeight		�E�B���h�E�c��[�ȗ���]
		//	@param[in]  _isFullScreen	�t���X�N���[���t���O[�ȗ���]
		//	@param[in]  _hWndParent		�e�E�B���h�E�n���h��[�ȗ���]
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual HRESULT	Initialize(
			CONST HINSTANCE _hInst,
			CONST UINT _nWidth = 640,
			CONST UINT _nHeight = 480,
			CONST BOOL _isFullWindow = FALSE,
			CONST HWND _hWndParent = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E�̃��b�Z�[�W����
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	void	ProcessMessage();

		/*!-----------------------------------------------------------
		//	@brief		�p�������Ďg�p���鉼�z�R�[���o�b�N�֐�
		//	@param[in]	_hWnd   �E�B���h�E�n���h��
		//	@param[in]	_msg    �|�󂳂ꂽ���b�Z�[�W
		//	@param[in]	_wParam ��Ԃɂ���ăp�����[�^�[���ς����r�b�g�ϐ�
		//	@param[in]	_lParam ��Ԃɂ���ăp�����[�^�[���ς����r�b�g�ϐ�
		//	@return		�E�B���h�E�̏��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	LRESULT	CALLBACK	WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

		/*!-----------------------------------------------------------
		//	@brief		���ʃR�[���o�b�N
		//	@param[in]	_hWnd   �E�B���h�E�n���h��
		//	@param[in]	_msg    �|�󂳂ꂽ���b�Z�[�W
		//	@param[in]	_wParam ��Ԃɂ���ăp�����[�^�[���ς����r�b�g�ϐ�
		//	@param[in]	_lParam ��Ԃɂ���ăp�����[�^�[���ς����r�b�g�ϐ�
		//	@return		�E�B���h�E�̏��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		static	LRESULT	CALLBACK	CommonWndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

		/*!-----------------------------------------------------------
		//	@brief		���[�h���X�_�C�A���O�E�B���h�E�o�^
		//	@param[in]	_pModeLessDialog	�o�^���郂�[�h���X�_�C�A���O
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/11
		------------------------------------------------------------*/
		virtual void ResiterModeLessDialog(ModeLessDialog* _pModeLessDialog)
		{
			this->pModeLessDialog = _pModeLessDialog;
		}


		/*!-----------------------------------------------------------
		//	@brief		���[�h���X�_�C�A���O�̉��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/20
		------------------------------------------------------------*/
		virtual void ReleaseModeLessDialog()
		{
			this->pModeLessDialog = nullptr;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E�^�C�g���ݒ�
		//	@param[in]	_titleName	�^�C�g����
		//	@return		0�ȊO�F�����@0�F���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	BOOL	SetWindowTitle(AOLPCSTR _titleName = _T("NO NAME")) const 
		{
			return ::SetWindowText(this->hWnd, _titleName);
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E�n���h���擾
		//	@return		�E�B���h�E�n���h��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	CONST HWND	GetHWnd() const 
		{
			return this->hWnd;
		}

		/*!-----------------------------------------------------------
		//	@brief		�C���X�^���X�n���h���擾
		//	@return		�C���X�^���X�n���h��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/11
		------------------------------------------------------------*/
		virtual CONST HINSTANCE GetHInstance() const 
		{
			return this->winEx.hInstance;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E�����擾
		//	@return		�E�B���h�E�̉���
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	UINT	GetWidth() const 
		{
			return this->window.width;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E�����擾
		//	@return		�E�B���h�E�̍���
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	UINT	GetHeight() const 
		{
			return this->window.height;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E�̃N���C�A���g�̈�̉����擾
		//	@return		�N���C�A���g�̈�̉���
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/21
		------------------------------------------------------------*/
		virtual	UINT	GetClientWidth() const 
		{
			return this->client.width;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E�̃N���C�A���g�̈�̏c���擾
		//	@return		�N���C�A���g�̈�̏c��
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/21
		------------------------------------------------------------*/
		virtual	UINT	GetClientHeight() const 
		{
			return this->client.height;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E���A�N�e�B�u���ǂ���
		//	@return		TRUE�F�A�N�e�B�u�@FALSE�F��A�N�e�B�u
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		BOOL IsActive()
		{
			return (this->hWnd == ::GetForegroundWindow());
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E���j�����ꂽ���ǂ���
		//	@return		TRUE�F�j�����ꂽ�@FALSE�F�j������Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	BOOL	IsDestroyed()
		{
			return this->isDead;
		}

		/*!-----------------------------------------------------------
		//	@brief		���݉ғ����Ă���E�B���h�E�����擾
		//	@return		���݉ғ����Ă���E�B���h�E��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		static	UINT	GetNumWindow()
		{
			return Window::sNumWindows;
		}

	private:
		typedef struct _SIZE
		{
			UINT width;
			UINT height;
		}SIZE;

	protected:
		HWND hWnd;		///< �E�B���h�E�n���h��
		SIZE window;	///< �E�B���h�E�̈�̃T�C�Y
		SIZE client;	///< �N���C�A���g�̈�̃T�C�Y
		BOOL isDead;	///< �j�����ꂽ���ǂ���
		BOOL isParent;	///< �e�t���O
		MSG			msg;		///< ���b�Z�[�W
		WNDCLASSEX	winEx;		///< �E�B���h�E���W�X�^
		ao::string	regName;	///< ���W�X�g����
		ao::ModeLessDialog* pModeLessDialog;	///< ���[�h���X�_�C�A���O

		static std::mutex sMutex;	///< �r�������p��mutex
		static UINT sNumWindows;	///< ���݉ғ����Ă���E�B���h�E��

	protected:
		/*!-----------------------------------------------------------
		//	@brief		���W�X�g���o�^
		//	@param[in]	_hInst	�C���X�^���X�n���h��
		//	@return		S_OK�F�o�^�����@����ȊO�F�o�^���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	HRESULT	Register(CONST HINSTANCE _hInst);

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E�쐬
		//	@param[in]	_nWidth			�E�B���h�E����
		//	@param[in]  _nHeight		�E�B���h�E�c��
		//	@param[in]  _isFullScreen	�t���X�N���[���t���O
		//	@param[in]  _hWndParent		�e�E�B���h�E�n���h��
		//	@return		TRUE�F�쐬�����@FALSE�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		virtual	INT_PTR CreateMyWindow(
			UINT _nWidth,
			UINT _nHeight,
			BOOL _isFullScreen,
			HWND _hWndParent
			);
	};
}

#endif // _Include_Window_hpp_
