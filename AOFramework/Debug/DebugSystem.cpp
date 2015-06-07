#include"stdafx.hpp"
#include"DebugSystem.hpp"


#if defined(DEBUG)||defined(_DEBUG)||defined(RELEASE_DEBUG)
//------------------------------------------------------------------------------
/**	@brief		•¶Žš—ñ‚Ì•`‰æ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:59	T.Aoyama	ì¬
//------------------------------------------------------------------------------
void ao::debug::Printf(CONST AOLPSTR _str, ...)
{
	DebugConsole::GetInstance()->SetColor();

	// •¶Žš—ñ‚ð•`‰æ
	va_list args;
	va_start(args, _str);
	DebugConsole::OutPutStringArgList(_str, args);
	va_end(args);
}


//------------------------------------------------------------------------------
/**	@brief		Žw’èF‚Å•¶Žš—ñ‚Ì•`‰æ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	01:00	T.Aoyama	ì¬
//------------------------------------------------------------------------------
void ao::debug::PrintfColor(ConsoleColor::Enum _textColor, CONST AOLPSTR _str, ...)
{
	// F‚ðƒZƒbƒg
	DebugConsole::GetInstance()->SetColorF(_textColor);

	va_list args;
	va_start(args, _str);
	DebugConsole::OutPutStringArgList(_str, args);
	va_end(args);
}


//------------------------------------------------------------------------------
/**	@brief		•¶Žš—ñ‚Ì•`‰æ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	01:00	T.Aoyama	ì¬
//------------------------------------------------------------------------------
void ao::debug::PrintfLog(CONST AOLPSTR _str, ...)
{
	DebugConsole::GetInstance()->SetColor();

	// •¶Žš—ñ‚ð•`‰æ
	va_list args;
	va_start(args, _str);
	DebugConsole::OutPutStringArgListLog(_str, args);
	va_end(args);
}


//------------------------------------------------------------------------------
/**	@brief		Žw’èF‚Å•¶Žš—ñ‚Ì•`‰æ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	01:01	T.Aoyama	ì¬
//------------------------------------------------------------------------------
void ao::debug::PrintfColorLog(ConsoleColor::Enum _textColor, CONST AOLPSTR _str, ...)
{
	// F‚ðƒZƒbƒg
	DebugConsole::GetInstance()->SetColorF(_textColor);

	va_list args;
	va_start(args, _str);
	DebugConsole::OutPutStringArgListLog(_str, args);
	va_end(args);
}

#endif