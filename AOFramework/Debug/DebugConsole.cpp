#include"stdafx.hpp"
#include"DebugConsole.hpp"


DebugConsole::DebugConsole()
{
	this->pFile = nullptr;
	SMALL_RECT	rc = { 0, 0, 130, 30 };
	COORD		cd = { 150, 1024 };

	::AllocConsole();	//�R���\�[���̊m��
	this->hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);

	::SetConsoleScreenBufferSize(this->hStdOut, cd);
	::SetConsoleWindowInfo(this->hStdOut, TRUE, &rc);

	this->backColor = ConsoleColor::L_BLACK;
	this->textColor = ConsoleColor::L_WHITE;
	this->SetColorF(ConsoleColor::H_RED);

	this->Printf(_T("[==================== Start Up Debug Console ====================]\n\n"));
	this->SetColorF(this->textColor);
}


DebugConsole::~DebugConsole()
{
	if (this->hStdOut == nullptr)
	{
		return;
	}

	if (this->pFile != nullptr)
	{
		fclose(this->pFile);
	}

	//�R���\�[���̉��
	::FreeConsole();

	this->hStdOut = nullptr;
}


//------------------------------------------------------------------------------
/**	@brief		�w�i�E�����F��ύX */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:52	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DebugConsole::SetColor(
	ConsoleColor::Enum _backColor,
	ConsoleColor::Enum _textColor)
{
	this->backColor = _backColor;
	this->textColor = _textColor;
	::SetConsoleTextAttribute(this->hStdOut, this->textColor | (this->backColor << 4));
}


//------------------------------------------------------------------------------
/**	@brief		������̕`�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:54	T.Aoyama	�쐬
//------------------------------------------------------------------------------
u32 DebugConsole::Printf(CONST ao::string _str, ...)
{
	DWORD len;
	va_list args;
	va_start(args, _str);
	ao_vsprintf_s(this->strBuf, _countof(this->strBuf), _str.c_str(), args);
	va_end(args);
	::WriteConsole(this->hStdOut, this->strBuf, (DWORD)(ao_strlen(this->strBuf)), &len, nullptr);
	return static_cast<u32>(len);
}


//------------------------------------------------------------------------------
/**	@brief		������̕`�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:54	T.Aoyama	�쐬
//------------------------------------------------------------------------------
u32 DebugConsole::PrintfLog(CONST ao::string _str, ...)
{
	DWORD len;
	va_list args;
	va_start(args, _str);
	ao_vsprintf_s(this->strBuf, _countof(this->strBuf), _str.c_str(), args);
	va_end(args);
	::WriteConsole(this->hStdOut, this->strBuf, (DWORD)(ao_strlen(this->strBuf)), &len, NULL);
	fprintf_s(this->pFile, "%s", this->strBuf);
	return static_cast<u32>(len);
}


//------------------------------------------------------------------------------
/**	@brief		������Ԃɖ߂� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:55	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DebugConsole::Reset()
{
	this->SetColor(ConsoleColor::L_BLACK, ConsoleColor::L_WHITE);
	this->Clear();
	this->SetCursorPosition(0, 0);
}


//------------------------------------------------------------------------------
/**	@brief		�ψ����ɕϊ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:57	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DebugConsole::OutPutStringArgList(CONST AOLPSTR _str, va_list _argList)
{
	AOCHAR buf[2048];
	_ao_vsnprintf_s(buf, _countof(buf), _str, _argList);
	OutputDebugString(buf);
	DebugConsole::GetInstance()->Printf(buf);
}


//------------------------------------------------------------------------------
/**	@brief		�ψ����ɕϊ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:58	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DebugConsole::OutPutStringArgListLog(CONST AOLPSTR _str, va_list _argList)
{
	AOCHAR buf[2048];
	_ao_vsnprintf_s(buf, _countof(buf), _str, _argList);
	OutputDebugString(buf);
	DebugConsole::GetInstance()->PrintfLog(buf);
}