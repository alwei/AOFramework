/*************************************************************
*	@file   	ModeLessDialog.hpp
*	@brief  	ModeLessDialog�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/10
*************************************************************/

#ifndef _Include_ModeLessDialog_hpp_	// �C���N���[�h�K�[�h
#define _Include_ModeLessDialog_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include"Window.hpp"
#include"../Define/SystemDefine.hpp"

namespace ao
{
	class Window;

	/*!-----------------------------------------------------------
	//	@class  ModeLessDialog
	//	@brief  ���[�h���X�_�C�A���O
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/10
	------------------------------------------------------------*/
	class ModeLessDialog
	{
	public:
		ModeLessDialog();
		virtual ~ModeLessDialog();

		/*!-----------------------------------------------------------
		//	@brief		�_�C�A���O�{�b�N�X�̍쐬
		//	@param[in]	_pWindow	�e�E�B���h�E
		//	@param[in]	_resName	���\�[�X������
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		virtual	HRESULT	Initialize(
			Window* _pWindow,
			CONST AOLPCSTR _resourceId);

		/*!-----------------------------------------------------------
		//	@brief		�_�C�A���O�j��
		//	@return		0�F�j�������@0�ȊO�F�j�����s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		virtual	BOOL Destory();

		/*!-----------------------------------------------------------
		//	@brief		���b�Z�[�W�̍X�V����
		//	@param[in]	_pMsg	���b�Z�[�W�|�C���^
		//	@return		0�ȊO�F�����@0�F���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		virtual	BOOL Update(LPMSG _pMsg);

		/*!-----------------------------------------------------------
		//	@brief		�p�������Ďg�p���鉼�z�R�[���o�b�N�֐�
		//	@param[in]	_hDialog	�_�C�A���O�n���h��
		//	@param[in]	_msg		���b�Z�[�W
		//	@param[in]	_wParam		�p�����[�^�[�P
		//	@param[in]	_lParam		�p�����[�^�[�Q
		//	@return		TRUE�F���폈���@FALSE�F�f�t�H���g����
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		virtual	INT_PTR DialogProc(
			HWND _hDialog,
			UINT _msg,
			WPARAM _wParam,
			LPARAM _lParam);

		/*!-----------------------------------------------------------
		//	@brief		���ʃ_�C�A���O�v���V�[�W��
		//	@param[in]	_hDialog	�_�C�A���O�n���h��
		//	@param[in]	_mgs		���b�Z�[�W
		//	@param[in]	_wParam		�p�����[�^�[�P
		//	@param[in]	_lParam		�p�����[�^�[�Q
		//	@return		TRUE�F�_�C�A���O���b�Z�[�W����	FALSE�F�_�C�A���O���b�Z�[�W�ł͂Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		static	INT_PTR CALLBACK CommonDialogProc(
			HWND _hDialog,
			UINT _msg,
			WPARAM _wParam,
			LPARAM _lParam);

		/*!-----------------------------------------------------------
		//	@brief		�_�C���O�n���h���擾
		//	@return		�_�C���O�n���h��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	CONST HWND	GetHDialog() const {
			return this->hDialog;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E���A�N�e�B�u���ǂ���
		//	@return		TRUE�F�A�N�e�B�u�@FALSE�F��A�N�e�B�u
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/13
		------------------------------------------------------------*/
		BOOL IsActive(){
			return (this->hDialog == ::GetForegroundWindow());
		}

	protected:
		HWND hDialog;
	};
}

#endif // _Include_ModeLessDialog_hpp_