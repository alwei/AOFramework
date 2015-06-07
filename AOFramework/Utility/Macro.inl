#ifndef __AO_MACRO_HPP__
#define	__AO_MACRO_HPP__

#include<stdlib.h>
#include<Windows.h>
#include<tchar.h>
#include"../Define/SystemDefine.hpp"


namespace ao
{
	namespace macro
	{
		/*!-----------------------------------------------------------
		//	@brief		自プロセスのHINSTANCE取得
		//	@return		自プロセスのHINSTANCE
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/23
		------------------------------------------------------------*/
		inline	HINSTANCE	GetHInstance()
		{
			return	((HINSTANCE)GetModuleHandle(0));
		}

		/*!-----------------------------------------------------------
		//	@brief		float型からDWORD型へ変換するマクロ
		//	@param[in]	_f 変換したfloat型の浮動小数
		//	@return		変換されたDWORD型の固定小数
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/23
		------------------------------------------------------------*/
		inline	DWORD	FtoDW(float _f)
		{
			return *((DWORD*)&_f);
		}

		/*!-----------------------------------------------------------
		//	@brief		エラー警告表示
		//	@param[in]	_errStr エラー内容の文字列
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/23
		------------------------------------------------------------*/
		inline	void	ERROR_MASSAGE(AOLPCSTR _errStr)
		{
			MessageBox(NULL, _errStr, _T("ERROR"), MB_ICONWARNING);
		}

		/*!-----------------------------------------------------------
		//	@brief		色取得マクロ
		//	@param[in]	_r	赤
		//	@param[in]	_g	緑
		//	@param[in]	_b	青
		//	@return		色情報をビットシフトした値
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/30
		------------------------------------------------------------*/
		inline	DWORD	GetColor(BYTE _r, BYTE _g, BYTE _b)
		{
			DWORD color = 0xFF000000;
			color |= _r << 16;
			color |= _g << 8;
			color |= _b << 0;

			return color;
		}

		/*!-----------------------------------------------------------
		//	@brief		色取得マクロ
		//	@param[in]	_r	赤
		//	@param[in]	_g	緑
		//	@param[in]	_b	青
		//	@param[in]	_a	透過
		//	@return		色情報をビットシフトした値
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/30
		------------------------------------------------------------*/
		inline	DWORD	GetColorA(BYTE _r, BYTE _g, BYTE _b, BYTE _a)
		{
			DWORD color = 0;
			color |= _a << 24;
			color |= _r << 16;
			color |= _g << 8;
			color |= _b << 0;

			return color;
		}

		/*!-----------------------------------------------------------
		//	@brief		色の浮動小数変換
		//	@param[in]	color		32bit化した色情報
		//	@param[in]	shiftBit	シフトするビット数
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/15
		------------------------------------------------------------*/
		inline	float	ColorToFloat(DWORD _color, int _shiftBit)
		{
			return ((_color >> _shiftBit) & 0xFF) / 255.0f;
		}

		/*!-----------------------------------------------------------
		//	@brief		32bitの色情報を浮動小数の色情報に変換
		//	@param[out]	_pOut		出力する色配列
		//	@param[in]	_dwColor	色情報
		//	@return		float配列ポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/29
		------------------------------------------------------------*/
		inline	CONST FLOAT* GetColorDtoF(FLOAT* _pOut, CONST DWORD _dwColor)
		{
			_pOut[0] = ColorToFloat(_dwColor, 24);
			_pOut[1] = ColorToFloat(_dwColor, 16);
			_pOut[2] = ColorToFloat(_dwColor, 8);
			_pOut[3] = ColorToFloat(_dwColor, 0);

			return _pOut;
		}


		/*!-----------------------------------------------------------
		//	@brief		文字コード変換マクロ
		//	@note		2バイト文字か1バイト文字かチェックしてくれる
		//	@param[out]	_pOutCharCode		出力する文字コード
		//	@param[in]	_pInCharData		入力する文字データ
		//	@return		TRUE：2バイト文字	FALSE：1バイト文字
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/25
		------------------------------------------------------------*/
		inline	BOOL	CharToUINT(UINT* _pOutCharCode, const char* _pInCharData)
		{
			//マルチバイト文字による処理
			if (IsDBCSLeadByte(*_pInCharData))
			{
				(*_pOutCharCode) = (BYTE)(*_pInCharData) << 8 | (BYTE)_pInCharData[1];
				return TRUE;
			}

			(*_pOutCharCode) = (UINT)(*_pInCharData);

			return FALSE;
		}


		/*!-----------------------------------------------------------
		//	@brief		文字が半角か全角かを識別
		//	@param[in]	_wChar	UNICODE文字
		//  @return     TRUE：全角　FALSE：半角
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/25
		------------------------------------------------------------*/
		inline	BOOL	IsCheckEm(const AOCHAR& _char)
		{
			WORD type = 0;
			GetStringTypeEx(
				LOCALE_SYSTEM_DEFAULT,	// ユニコード版プロジェクトでは、無視される引数       
				CT_CTYPE3,				// 全角、半角、ひらがな等を判定するタイプ
				&_char,					// 判定対象となる文字配列
				1,						// 判定対象となる文字配列のサイズ
				&type);					// 判定結果の配列

			if (type & C3_HALFWIDTH)
			{
				return FALSE;
			}

			return TRUE;
		}

		/*!-----------------------------------------------------------
		//	@brief		ワイド文字列からマルチバイト文字に変換
		//	@param[out]	_pOut	出力する文字列バッファ
		//	@param[in]	_src	入力する文字列バッファ
		//	return		変換された文字数
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		inline	size_t	StringWideToMulti(std::string* _pOut, const std::wstring& _src)
		{
			size_t num;
			size_t size = _src.length()*MB_CUR_MAX + 1;
			char* cnv = new char[size];
			wcstombs_s(&num, cnv, size, _src.c_str(), size);
			_pOut->assign(cnv);
			delete[] cnv;
			return num;
		}

		/*!-----------------------------------------------------------
		//	@brief		マルチバイト文字列からワイド文字に変換
		//	@param[out]	_pOut	出力する文字列バッファ
		//	@param[in]	_src	入力する文字列バッファ
		//	return		変換された文字数
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		inline	size_t	StringMultiToWide(std::wstring* _pOut, const std::string& _src)
		{
			size_t num;
			size_t size = _src.length() + 1;
			wchar_t* cnv = new wchar_t[size];
			mbstowcs_s(&num, cnv, size, _src.c_str(), size);
			_pOut->assign(cnv);
			delete[] cnv;
			return num;
		}

		/*!-----------------------------------------------------------
		//	@brief		文字列パスからファイル名を取得
		//	@param[out]	_pOut		出力する文字列バッファポインタ
		//	@param[in]	_bufSize	出力する文字列バッファサイズ
		//	@param[in]	_pSrc		抽出する文字列バッファポインタ
		//	@param[in]	_isAddExt	拡張子を付加するかどうか[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		inline	void GetFileNameFromPath(AOLPSTR _pOut, size_t _bufSize, LPCTSTR _pSrc, BOOL _isAddExt = TRUE)
		{
			int		offset, nowOffset;
			AOCHAR	endCode = (_isAddExt) ? _T('\0') : _T('.');

			//終端検索->階層検索
			for (offset = 0; _pSrc[offset] != endCode; ++offset);
			for (; _pSrc[offset] != '\\'&&_pSrc[offset] != '/'&&offset != -1; --offset);
			for (nowOffset = 0; _pSrc[offset + 1] != endCode || _T('\0'); ++offset, ++nowOffset)
			{
				if (nowOffset >= (int)_bufSize)
				{
					return;
				}

				_pOut[nowOffset] = _pSrc[offset + 1];
			}

			_pOut[nowOffset] = _T('\0');
		}

		/*!-----------------------------------------------------------
		//	@brief		文字列パスからファイルパス
		//	@note		_pOut		出力する文字列バッファポインタ
		//	@param[in]	_pSrc		抽出する文字列バッファ
		//	@author		Tatsunori Aoyama
		//	@date		2013/11/02
		------------------------------------------------------------*/
		inline	void	GetFilePathFromPath(AOLPSTR _pOut, AOLPCSTR _pSrc)
		{
			//ファイル拡張子階層のファイル名を削除
			int copySize, offset;
			for (offset = 0; _pSrc[offset] != _T('\0'); ++offset);
			for (; _pSrc[offset] != _T('\\')&&_pSrc[offset] != _T('/'); --offset);

			copySize = offset * sizeof(AOCHAR) + sizeof(AOCHAR);
			::memcpy(_pOut, _pSrc, copySize);
			_pOut[offset+1] = _T('\0');
		}

		/*!-----------------------------------------------------------
		//	@brief		文字列パスからファイルパス
		//	@note		マルチバイト専用
		//	@note		_pOut		出力する文字列バッファポインタ
		//	@param[in]	_pSrc		抽出する文字列バッファ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/03
		------------------------------------------------------------*/
		inline	void	GetFilePathFromPath_MultiByte(LPSTR _pOut, LPCTSTR _pSrc)
		{
			//ファイル拡張子階層のファイル名を削除
			int copySize, offset;
			for (offset = 0; _pSrc[offset] != '\0'; ++offset);
			for (; _pSrc[offset] != '\\'&&_pSrc[offset] != '/'; --offset);

			copySize = sizeof(AOCHAR)*offset + 1;
			::memcpy(_pOut, _pSrc, copySize);
			_pOut[copySize] = '\0';
		}

		/*!-----------------------------------------------------------
		//	@brief		文字列パスから拡張子抽出
		//	@param[out]	_pOut		出力する文字列バッファポインタ
		//	@param[in]	_bufSize	出力する文字列バッファサイズ
		//	@param[in]	_pSrc		抽出する文字列バッファポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		inline	void	GetFileExtension(AOLPSTR _pOut, size_t _bufSize, AOLPCSTR _pSrc)
		{
			int		offset;
			int		numChar;

			//終端検索->階層検索
			for (offset = 0; _pSrc[offset] != '\0'; ++offset);
			for (numChar = 0; _pSrc[offset] != '.'; --offset, ++numChar);

			//ファイル名抽出
			ao_strcpy_s(_pOut, _bufSize, &_pSrc[offset + 1]);
		}

		/*!-----------------------------------------------------------
		//	@brief		float配列用の色取得マクロ
		//	@param[in]	_r		赤色値
		//	@param[in]	_g		緑色値
		//	@param[in]	_b		青色値
		//	@param[in]	_a		透過値
		//	@param[out]	_pOut	出力するfloat配列ポインタ
		//	@return		floatの配列ポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/25
		------------------------------------------------------------*/
		inline	const float* GetColorF(float* _pOut, unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a)
		{
			_pOut[0] = (_r & 0xFF) / 255.0f;
			_pOut[1] = (_g & 0xFF) / 255.0f;
			_pOut[2] = (_b & 0xFF) / 255.0f;
			_pOut[3] = (_a & 0xFF) / 255.0f;
			return _pOut;
		}

		/*!-----------------------------------------------------------
		//	@brief		float配列用の色取得マクロ
		//	@param[out]	_pOut		出力するfloat配列ポインタ
		//	@param[in]	_dwColor	32bitカラー
		//	@return		floatの配列ポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/25
		------------------------------------------------------------*/
		inline	const float* GetColorF(float* _pOut, unsigned long _dwColor)
		{
			_pOut[0] = ((_dwColor >> 16) & 0xFF) / 255.0f;
			_pOut[1] = ((_dwColor >> 8) & 0xFF) / 255.0f;
			_pOut[2] = ((_dwColor)& 0xFF) / 255.0f;
			_pOut[3] = ((_dwColor >> 24) & 0xFF) / 255.0f;

			return _pOut;
		}

		/*!-----------------------------------------------------------
		//	@brief		指定した型に変換して値を戻す
		//	@param[in]	_p		対象となるメモリ先頭ポインタ
		//	@param[in]	_size	抽出するメモリサイズ
		//	@return		メモリ内から抽出した数値（絶対0は返す）
		//	@author		Tatsunori Aoyama
		//	@date		2013/11/27
		------------------------------------------------------------*/
		template<class T>
		inline	T	GetMemoryInValue(void* _p, int _size)
		{
			switch (_size)
			{
			case 1:	return (T)(*(__int8*)_p);
			case 2: return (T)(*(__int16*)_p);
			case 4: return (T)(*(__int32*)_p);
			case 8: return (T)(*(__int64*)_p);
			}

			return 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		ポインタのキャスト
		//	@param[in]	_p	キャストするポインタ
		//	@return		キャストする型のポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		template<class T>
		inline	T	pointer_cast(void* _p)
		{
			return static_cast<T>(_p);
		}

		/*!-----------------------------------------------------------
		//	@brief		COMポインタの安全解放
		//	@param[in]	_p	解放するCOMポインタ
		//	@return		S_OK：解放成功　S_FALSE：解放失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/26
		------------------------------------------------------------*/
		template<class T>
		inline HRESULT SAFE_RELEASE(T& _p)
		{
			if (_p)
			{
				_p->Release();
				_p = nullptr;
				return S_OK;
			}

			return S_FALSE;
		}

		/*!-----------------------------------------------------------
		//	@brief		メモリの安全解放
		//	@param[in]	_p	解放するCOMポインタ
		//	@return		S_OK：解放成功　S_FALSE：解放失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/26
		------------------------------------------------------------*/
		template<class T>
		inline HRESULT SAFE_DELETE(T& _p)
		{
			if (_p)
			{
				delete _p;
				_p = nullptr;
				return S_OK;
			}

			return S_FALSE;
		}


		/*!-----------------------------------------------------------
		//	@brief		配列メモリの安全解放
		//	@param[in]	_p	解放するCOMポインタ
		//	@return		S_OK：解放成功　S_FALSE：解放失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/26
		------------------------------------------------------------*/
		template<class T>
		inline HRESULT SAFE_DELETE_ARRAY(T& _p)
		{
			if (_p)
			{
				delete[] _p;
				_p = nullptr;
				return S_OK;
			}

			return S_FALSE;
		}
	}
}

#endif