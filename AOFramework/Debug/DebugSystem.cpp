#include"stdafx.hpp"
#include"DebugSystem.hpp"


#if defined(DEBUG)||defined(_DEBUG)||defined(RELEASE_DEBUG)
//------------------------------------------------------------------------------
/**	@brief		������̕`�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:59	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void ao::debug::Printf(CONST AOLPSTR _str, ...)
{
	DebugConsole::GetInstance()->SetColor();

	// �������`��
	va_list args;
	va_start(args, _str);
	DebugConsole::OutPutStringArgList(_str, args);
	va_end(args);
}


//------------------------------------------------------------------------------
/**	@brief		�w��F�ŕ�����̕`�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	01:00	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void ao::debug::PrintfColor(ConsoleColor::Enum _textColor, CONST AOLPSTR _str, ...)
{
	// �F���Z�b�g
	DebugConsole::GetInstance()->SetColorF(_textColor);

	va_list args;
	va_start(args, _str);
	DebugConsole::OutPutStringArgList(_str, args);
	va_end(args);
}


//------------------------------------------------------------------------------
/**	@brief		������̕`�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	01:00	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void ao::debug::PrintfLog(CONST AOLPSTR _str, ...)
{
	DebugConsole::GetInstance()->SetColor();

	// �������`��
	va_list args;
	va_start(args, _str);
	DebugConsole::OutPutStringArgListLog(_str, args);
	va_end(args);
}


//------------------------------------------------------------------------------
/**	@brief		�w��F�ŕ�����̕`�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	01:01	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void ao::debug::PrintfColorLog(ConsoleColor::Enum _textColor, CONST AOLPSTR _str, ...)
{
	// �F���Z�b�g
	DebugConsole::GetInstance()->SetColorF(_textColor);

	va_list args;
	va_start(args, _str);
	DebugConsole::OutPutStringArgListLog(_str, args);
	va_end(args);
}

#endif