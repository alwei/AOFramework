#ifndef __DEBUG_SYSTEM_HPP__
#define	__DEBUG_SYSTEM_HPP__

#include<tchar.h>
#include"DebugConsole.hpp"

#ifdef __AO_DEBUG__
#include<crtdbg.h>
#endif

namespace ao
{
	namespace debug
	{
		/*!-----------------------------------------------------------
		//	@brief		メモリダンプの初期化
		//	@return		_crtDbgFlag の以前の状態
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/26
		------------------------------------------------------------*/
		inline	int	InitMemDump(int _flags = _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
		{
#ifdef __AO_DEBUG__
			return _CrtSetDbgFlag(_flags);
#endif
			return 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		メモリリーク検出
		//	@param[in]	_breakAlloc	ブレークポイントを設定する割り当て順序番号
		//	@return		ブレークポイントが設定されていた、以前のオブジェクト割り当て順序番号
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/26
		------------------------------------------------------------*/
		inline	int	BreakAlloc(long _breakAlloc){
#ifdef __AO_DEBUG__
			return _CrtSetBreakAlloc(_breakAlloc);
#endif
			return 0;
		}

		static const int DUMMY_ZERO = 0;

#if defined(_DEBUG) || defined(DEBUG) || defined(RELEASE_DEBUG)
		/*!-----------------------------------------------------------
		//	@brief		デバッグコンソールの初期化
		//	@author		Tatsunori Aoyama
		//	@date		2013/04/27
		------------------------------------------------------------*/
		inline	void	InitDebugConsole(void)
		{
			DebugConsole::GetInstance()->RemoveClose();
		}

		/*!-----------------------------------------------------------
		//	@brief		ログ作成初期化
		//	@param[in]	fileName	書き込むファイル名
		//	@author		Tatsunori Aoyama
		//	@date		2013/10/05
		------------------------------------------------------------*/
		inline	void	InitDebugLog(AOLPCSTR _fileName)
		{
			DebugConsole::GetInstance()->CreateDebugLog(_fileName);
		}

		/*!-----------------------------------------------------------
		//	@brief		×ボタンを無効化
		//	@author		Tatsunori Aoyama
		//	@date		2013/12/04
		------------------------------------------------------------*/
		inline	void	RemoveClose()
		{
			DebugConsole::GetInstance()->RemoveClose();
		}

		/*!-----------------------------------------------------------
		//	@brief		文字列の描画
		//	@param[in]	_str	文字列
		//	@param[in]	...		可変引数
		//	@author		Tatsunori Aoyama
		//	@date		2013/04/27
		------------------------------------------------------------*/
		void Printf(CONST AOLPSTR _str, ...);

		/*!-----------------------------------------------------------
		//	@brief		指定色で文字列の描画
		//	@param[in]	_textColor 文字色
		//	@param[in]	_str       文字列
		//	@param[in]	...       可変引数
		//	@author		Tatsunori Aoyama
		//	@date		2013/04/27
		------------------------------------------------------------*/
		void PrintfColor(ConsoleColor::Enum _textColor, CONST AOLPSTR _str, ...);

		/*!-----------------------------------------------------------
		//	@brief		文字列の描画
		//	@note		ログ出力版
		//	@param[in]	_str	文字列
		//	@param[in]	...		可変引数
		//	@author		Tatsunori Aoyama
		//	@date		2013/05/20
		------------------------------------------------------------*/
		void PrintfLog(CONST AOLPSTR _str, ...);

		/*!-----------------------------------------------------------
		//	@brief		指定色で文字列の描画
		//	@note		ログ出力版
		//	@param[in]	_textColor	文字色
		//	@param[in]	_str		文字列
		//	@param[in]	...			可変引数
		//	@author		Tatsunori Aoyama
		//	@date		2013/05/20
		------------------------------------------------------------*/
		void PrintfColorLog(ConsoleColor::Enum _textColor, CONST AOLPSTR _str, ...);

#else
#define InitDebugConsole()					DUMMY_ZERO
#define InitDebugLog(fileName)				DUMMY_ZERO
#define RemoveClose()						DUMMY_ZERO
#define Printf(str, ... )					DUMMY_ZERO
#define PrintfLog(str,... )					DUMMY_ZERO
#define PrintfColor(fontColor,str, ... )	DUMMY_ZERO
#define PrintfColorLog(textColor,str,...)	DUMMY_ZERO

#endif
	}	
}

#endif