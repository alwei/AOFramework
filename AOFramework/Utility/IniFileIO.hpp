/*************************************************************
*	@file   	IniFileIO.hpp
*	@brief  	IniFileIOクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/12/16
*************************************************************/

#ifndef _Include_IniFileIO_hpp_	// インクルードガード
#define _Include_IniFileIO_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include<stdio.h>
#include<tchar.h>
#include<array>
#include"../Define/SystemDefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  IniFileIO
	//	@brief  iniファイルを読み込むクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/12/16
	------------------------------------------------------------*/
	class IniFileIO
	{
	public:
		IniFileIO();
		IniFileIO(const ao::string& _filePath);
		~IniFileIO();

		/*!-----------------------------------------------------------
		//	@brief		ファイルオープン
		//	@param[in]	_filePath	ファイルパス
		//  @return     成功：ファイルパス文字数　失敗：-1
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/16
		------------------------------------------------------------*/
		int Open(const ao::string& _filePath);

		/*!-----------------------------------------------------------
		//	@brief		指定セクションの情報取得
		//	@param[in]	_sectionName  セクションネーム
		//	@param[in]	_keyName      キー名
		//	@param[in]	_defaultParam 既定に設定する値[省略可]
		//	@return		指定した値
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/16
		------------------------------------------------------------*/
		const float GetFloat(
			const ao::string& _sectionName,
			const ao::string& _keyName,
			const float _defaultParam = 0);

		/*!-----------------------------------------------------------
		//	@brief		指定セクションの情報取得
		//	@param[in]	_sectionName  セクションネーム
		//	@param[in]	_keyName      キー名
		//	@param[in]	_defaultParam 既定に設定する値[省略可]
		//	@return		指定した値
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/16
		------------------------------------------------------------*/
		const int	GetInt(
			const ao::string& _sectionName,
			const ao::string& _keyName,
			const int _defaultParam = 0);

		/*!-----------------------------------------------------------
		//	@brief		指定セクションの情報取得
		//  @param[out] _pOut			出力するバッファ
		//  @param[in]  _size			出力先のバッファサイズ
		//	@param[in]	sectionName		セクションネーム
		//	@param[in]	keyName			キー名
		//	@param[in]	defaultParam	既定に設定する値[省略可]
		//	@return		指定した値
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/16
		------------------------------------------------------------*/
		const AOCHAR* GetString(
			AOCHAR* _pOut,
			const size_t _size,
			const ao::string& _sectionName,
			const ao::string& _keyName,
			const ao::string& _defaultParam = _T(""));


		/*!-----------------------------------------------------------
		//	@brief		指定セクションの情報取得
		//	@param[in]	sectionName		セクションネーム
		//	@param[in]	keyName			キー名
		//	@param[in]	defaultParam	既定に設定する値[省略可]
		//  @return     TRUE：文字列がTRUE　FALSE：文字列がFALSE
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/16
		------------------------------------------------------------*/
		const BOOL GetBool(
			const ao::string& _sectionName,
			const ao::string& _keyName,
			const BOOL _defaultParam = FALSE);

	private:
		enum Buffer
		{
			Temp = 128,
			Path = 512
		};

	private:
		std::array<AOCHAR, Buffer::Path> filePath;
		std::array<AOCHAR, Buffer::Temp> buffer;
	};
}

#endif // _Include_IniFileIO_hpp_