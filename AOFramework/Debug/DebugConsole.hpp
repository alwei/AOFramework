#ifndef __DEBUG_CONSOLE_HPP__
#define	__DEBUG_CONSOLE_HPP__

#include<tchar.h>
#include<Windows.h>
#include"../Define/SystemDefine.hpp"


class ConsoleColor
{
public:
	enum Enum 
	{
		L_BLACK = 0,
		L_BLUE,
		L_GREEN,
		L_CYAN,
		L_RED,
		L_PURPLE,
		L_YELLOW,
		L_WHITE,

		H_BLACK,
		H_BLUE,
		H_GREEN,
		H_CYAN,
		H_RED,
		H_PURPLE,
		H_YELLOW,
		H_WHITE
	};
};


/*!-----------------------------------------------------------
//	@class  DebugConsole
//	@brief  �f�o�b�O�p�̃R���\�[��
//	@author	Tatsunori Aoyama
//	@date	2013/04/17
------------------------------------------------------------*/
class DebugConsole
{
public:
	/*!-----------------------------------------------------------
	//	@brief		�C���X�^���X�Ăяo��
	//	@return		���g�̃|�C���^
	//	@author		Tatsunori Aoyama
	//	@date		2014/01/13
	------------------------------------------------------------*/
	static	DebugConsole*	GetInstance()
	{
		static DebugConsole singleton;
		return &singleton;
	}

	~DebugConsole();

	/*!-----------------------------------------------------------
	//	@brief		�t�@�C���|�C���^�I�[�v��
	//	@param[in]	_fileName	�쐬����t�@�C����
	//	@author		Tatsunori Aoyama
	//	@date		2013/08/05
	------------------------------------------------------------*/
	void CreateDebugLog(AOLPCSTR _fileName)
	{
		ao_fopen_s(&this->pFile, _fileName, _T("w"));
	}

	/*!-----------------------------------------------------------
	//	@brief		�J�[�\���̑傫���Ɖ��ݒ�
	//	@param[in]	_dwSize    �J�[�\���̑傫��
	//	@param[in]	_isVisible ���ݒ�
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void SetCursorInfo(DWORD _dwSize = 25, BOOL _isVisible = TRUE)
	{
		CONSOLE_CURSOR_INFO info = { _dwSize, _isVisible };
		::SetConsoleCursorInfo(this->hStdOut, &info);
	}

	/*!-----------------------------------------------------------
	//	@brief		�R���\�[���J�[�\���̈ʒu�ݒ�
	//	@param[in]	_x X���W
	//	@return		_y Y���W
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void SetCursorPosition(int _x, int _y)
	{
		COORD coord = { (short)_x, (short)_y };
		::SetConsoleCursorPosition(this->hStdOut, coord);
	}

	/*!-----------------------------------------------------------
	//	@brief		�w�i�F��ύX
	//	@param[in]	backColor �w�i�F
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void SetColorB(ConsoleColor::Enum _backColor = ConsoleColor::L_BLACK)
	{
		this->SetColor(_backColor, this->textColor);
	}

	/*!-----------------------------------------------------------
	//	@brief		�����F��ύX
	//	@param[in]	textColor �����F
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void SetColorF(ConsoleColor::Enum _textColor = ConsoleColor::L_WHITE)
	{
		this->SetColor(this->backColor, _textColor);
	}

	/*!-----------------------------------------------------------
	//	@brief		�w�i�E�����F��ύX
	//	@param[in]	backColor �w�i�F
	//	@param[in]	textColor �����F
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void SetColor(
		ConsoleColor::Enum _backColor = ConsoleColor::L_BLACK,
		ConsoleColor::Enum _textColor = ConsoleColor::L_WHITE);

	/*!-----------------------------------------------------------
	//	@brief		������̕`��
	//	@param[in]	_str	������
	//	@param[in]	...		�ψ���
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	u32 Printf(CONST ao::string _str, ...);

	/*!-----------------------------------------------------------
	//	@brief		������̕`��
	//	@note		���O�e�L�X�g�ɂ��o�͂���
	//	@param[in]	str ������
	//	@param[in]	... �ψ���
	//	@author		Tatsunori Aoyama
	//	@date		2013/05/20
	------------------------------------------------------------*/
	u32 PrintfLog(CONST ao::string _str, ...);

	/*!-----------------------------------------------------------
	//	@brief		�`���ʂ̃N���A
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void Clear(void)
	{
		::system("cls");
	}

	/*!-----------------------------------------------------------
	//	@brief		����{�^���𖳌���
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void RemoveClose(void)
	{
		HMENU hMenu = GetSystemMenu(::GetConsoleWindow(), FALSE);
		::RemoveMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
	}

	/*!-----------------------------------------------------------
	//	@brief		������Ԃɖ߂�
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void Reset();

	/*!-----------------------------------------------------------
	//	@brief		�ψ����ɕϊ�
	//	@param[in]  _str		������
	//	@param[in]  _argList	�ϗv�f
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	static void	OutPutStringArgList(CONST AOLPSTR _str, va_list _argList);

	/*!-----------------------------------------------------------
	//	@brief		�ψ����ɕϊ�
	//	@note		���O�o�͔�
	//	@param[in]  _str		������
	//	@param[in]	_argList	�ϗv�f
	//	@author		Tatsunori Aoyama
	//	@date		2013/05/20
	------------------------------------------------------------*/
	static	void	OutPutStringArgListLog(CONST AOLPSTR _str, va_list _argList);

private:
	FILE*	pFile;			///< �t�@�C���|�C���^	
	HANDLE	hStdOut;		///< �o�͐�n���h��
	AOCHAR	strBuf[512];	///< ������o�b�t�@
	ConsoleColor::Enum	backColor;	///< �w�i�F
	ConsoleColor::Enum	textColor;	///< �����F

private:
	DebugConsole();
};

#endif