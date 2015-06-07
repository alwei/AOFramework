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
//	@brief  デバッグ用のコンソール
//	@author	Tatsunori Aoyama
//	@date	2013/04/17
------------------------------------------------------------*/
class DebugConsole
{
public:
	/*!-----------------------------------------------------------
	//	@brief		インスタンス呼び出し
	//	@return		自身のポインタ
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
	//	@brief		ファイルポインタオープン
	//	@param[in]	_fileName	作成するファイル名
	//	@author		Tatsunori Aoyama
	//	@date		2013/08/05
	------------------------------------------------------------*/
	void CreateDebugLog(AOLPCSTR _fileName)
	{
		ao_fopen_s(&this->pFile, _fileName, _T("w"));
	}

	/*!-----------------------------------------------------------
	//	@brief		カーソルの大きさと可視設定
	//	@param[in]	_dwSize    カーソルの大きさ
	//	@param[in]	_isVisible 可視設定
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void SetCursorInfo(DWORD _dwSize = 25, BOOL _isVisible = TRUE)
	{
		CONSOLE_CURSOR_INFO info = { _dwSize, _isVisible };
		::SetConsoleCursorInfo(this->hStdOut, &info);
	}

	/*!-----------------------------------------------------------
	//	@brief		コンソールカーソルの位置設定
	//	@param[in]	_x X座標
	//	@return		_y Y座標
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void SetCursorPosition(int _x, int _y)
	{
		COORD coord = { (short)_x, (short)_y };
		::SetConsoleCursorPosition(this->hStdOut, coord);
	}

	/*!-----------------------------------------------------------
	//	@brief		背景色を変更
	//	@param[in]	backColor 背景色
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void SetColorB(ConsoleColor::Enum _backColor = ConsoleColor::L_BLACK)
	{
		this->SetColor(_backColor, this->textColor);
	}

	/*!-----------------------------------------------------------
	//	@brief		文字色を変更
	//	@param[in]	textColor 文字色
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void SetColorF(ConsoleColor::Enum _textColor = ConsoleColor::L_WHITE)
	{
		this->SetColor(this->backColor, _textColor);
	}

	/*!-----------------------------------------------------------
	//	@brief		背景・文字色を変更
	//	@param[in]	backColor 背景色
	//	@param[in]	textColor 文字色
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void SetColor(
		ConsoleColor::Enum _backColor = ConsoleColor::L_BLACK,
		ConsoleColor::Enum _textColor = ConsoleColor::L_WHITE);

	/*!-----------------------------------------------------------
	//	@brief		文字列の描画
	//	@param[in]	_str	文字列
	//	@param[in]	...		可変引数
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	u32 Printf(CONST ao::string _str, ...);

	/*!-----------------------------------------------------------
	//	@brief		文字列の描画
	//	@note		ログテキストにも出力する
	//	@param[in]	str 文字列
	//	@param[in]	... 可変引数
	//	@author		Tatsunori Aoyama
	//	@date		2013/05/20
	------------------------------------------------------------*/
	u32 PrintfLog(CONST ao::string _str, ...);

	/*!-----------------------------------------------------------
	//	@brief		描画画面のクリア
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void Clear(void)
	{
		::system("cls");
	}

	/*!-----------------------------------------------------------
	//	@brief		閉じるボタンを無効化
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void RemoveClose(void)
	{
		HMENU hMenu = GetSystemMenu(::GetConsoleWindow(), FALSE);
		::RemoveMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
	}

	/*!-----------------------------------------------------------
	//	@brief		初期状態に戻す
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	void Reset();

	/*!-----------------------------------------------------------
	//	@brief		可変引数に変換
	//	@param[in]  _str		文字列
	//	@param[in]  _argList	可変要素
	//	@author		Tatsunori Aoyama
	//	@date		2013/04/27
	------------------------------------------------------------*/
	static void	OutPutStringArgList(CONST AOLPSTR _str, va_list _argList);

	/*!-----------------------------------------------------------
	//	@brief		可変引数に変換
	//	@note		ログ出力版
	//	@param[in]  _str		文字列
	//	@param[in]	_argList	可変要素
	//	@author		Tatsunori Aoyama
	//	@date		2013/05/20
	------------------------------------------------------------*/
	static	void	OutPutStringArgListLog(CONST AOLPSTR _str, va_list _argList);

private:
	FILE*	pFile;			///< ファイルポインタ	
	HANDLE	hStdOut;		///< 出力先ハンドル
	AOCHAR	strBuf[512];	///< 文字列バッファ
	ConsoleColor::Enum	backColor;	///< 背景色
	ConsoleColor::Enum	textColor;	///< 文字色

private:
	DebugConsole();
};

#endif