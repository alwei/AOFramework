/*************************************************************
*	@file   	DialogWindow.hpp
*	@brief  	DialogWindow�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/09
*************************************************************/

#ifndef _Include_DialogWindow_hpp_	// �C���N���[�h�K�[�h
#define _Include_DialogWindow_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<array>
#include"../../AOFramework/Window/Window.hpp"
#include"../../AOFramework/Utility/Macro.inl"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DialogWindow
	//	@brief  �R�����_�C�A���O�E�B���h�E
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/09
	------------------------------------------------------------*/
	class DialogWindow : public Window
	{
	public:
		/// �R���X�g���N�^
		DialogWindow();

		/// �f�X�g���N�^
		virtual	~DialogWindow();

		/*!-----------------------------------------------------------
		//	@brief		�_�C�A���O������
		//  @param[in]  _fileName	�t�@�C����
		//  @param[in]  _fileExt	�t�@�C���g���q
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	void	InitializeDialog(string _fileName, string _fileExt);

		/*!-----------------------------------------------------------
		//	@brief		�h���b�v����
		//	@note		�R�[���o�b�N���ɋL�q
		//	@param[in]	hWnd	�E�B���h�E�n���h��
		//	@param[in]	wParam	�h���b�v���̃p�����[�^
		//	@return		TRUE�F���������@FALSE�F�������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	BOOL	ProcessDropFile(HWND hWnd, DWORD wParam);

		/*!-----------------------------------------------------------
		//	@brief		�t�@�C���I�[�v�����\�b�h
		//	@return		S_OK�F�I�[�v�������@E_FAIL�F�I�[�v�����s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	HRESULT	OpenFile();

		/*!-----------------------------------------------------------
		//	@brief		�t�@�C���Z�[�u���\�b�h
		//	@return		S_OK�F�Z�[�u�����@E_FAIL�F�Z�[�u���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	HRESULT	SaveFile();

		/*!-----------------------------------------------------------
		//	@brief		�Ō�Ɏ擾�����������ݗp�̃t�@�C���p�X���擾
		//	@param[in]	_bufferSize	�R�s�[��o�͂ƂȂ镶����o�b�t�@�T�C�Y
		//	@param[out]	_pOut		�R�s�[�o�͐�ƂȂ镶����o�b�t�@�|�C���^
		//	@return		�R�s�[�o�͂ƂȂ镶����o�b�t�@�̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	TCHAR*	GetLastWriteFilePath(const size_t _bufferSize = 0, TCHAR * _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�Ō�Ɏ擾�����������ݗp�̃t�@�C�������擾
		//	@param[in]	_bufferSize	�R�s�[��o�͂ƂȂ镶����o�b�t�@�T�C�Y
		//	@param[out]	_pOut		�R�s�[�o�͐�ƂȂ镶����o�b�t�@�|�C���^
		//	@return		�R�s�[�o�͂ƂȂ镶����o�b�t�@�̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	TCHAR*	GetLastWriteFileName(const size_t _bufferSize = 0, TCHAR * _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�Ō�Ɏ擾�����ǂݍ��ݗp�̃t�@�C���p�X���擾
		//	@param[in]	_bufferSize	�R�s�[��o�͂ƂȂ镶����o�b�t�@�T�C�Y
		//	@param[out]	_pOut		�R�s�[�o�͐�ƂȂ镶����o�b�t�@�|�C���^
		//	@return		�R�s�[�o�͂ƂȂ镶����o�b�t�@�̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	TCHAR*	GetLastReadFilePath(const size_t _bufferSize = 0,TCHAR * _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�Ō�Ɏ擾�����ǂݍ��ݗp�̃t�@�C�������擾
		//	@param[in]	_bufferSize	�R�s�[��o�͂ƂȂ镶����o�b�t�@�T�C�Y
		//	@param[out]	_pOut		�R�s�[�o�͐�ƂȂ镶����o�b�t�@�|�C���^
		//	@return		�R�s�[�o�͂ƂȂ镶����o�b�t�@�̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	TCHAR*	GetLastReadFileName(const size_t _bufferSize = 0, TCHAR * _pOut = nullptr);

	protected:
		struct FILE_PATHNAME{
			std::array<TCHAR, 1024>	filePath;
			std::array<TCHAR, 256>	fileName;
			FILE_PATHNAME()
			{
				this->filePath.fill(0);
				this->fileName.fill(0);
			}
		};

		struct FILE_STRINFO
		{
			string filter;
			string title;
			string defExt;
		};

	protected:
		FILE_STRINFO	writeInfo;
		FILE_STRINFO	readInfo;
		FILE_PATHNAME	write;
		FILE_PATHNAME	read;
		OPENFILENAME	ofn;
		OPENFILENAME	ifn;
	};
}

#endif // _Include_DialogWindow_hpp_