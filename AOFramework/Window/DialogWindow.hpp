/*************************************************************
*	@file   	DialogWindow.hpp
*	@brief  	DialogWindowクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/10/09
*************************************************************/

#ifndef _Include_DialogWindow_hpp_	// インクルードガード
#define _Include_DialogWindow_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<array>
#include"../../AOFramework/Window/Window.hpp"
#include"../../AOFramework/Utility/Macro.inl"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DialogWindow
	//	@brief  コモンダイアログウィンドウ
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/09
	------------------------------------------------------------*/
	class DialogWindow : public Window
	{
	public:
		/// コンストラクタ
		DialogWindow();

		/// デストラクタ
		virtual	~DialogWindow();

		/*!-----------------------------------------------------------
		//	@brief		ダイアログ初期化
		//  @param[in]  _fileName	ファイル名
		//  @param[in]  _fileExt	ファイル拡張子
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	void	InitializeDialog(string _fileName, string _fileExt);

		/*!-----------------------------------------------------------
		//	@brief		ドロップ処理
		//	@note		コールバック内に記述
		//	@param[in]	hWnd	ウィンドウハンドル
		//	@param[in]	wParam	ドロップ時のパラメータ
		//	@return		TRUE：処理成功　FALSE：処理失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	BOOL	ProcessDropFile(HWND hWnd, DWORD wParam);

		/*!-----------------------------------------------------------
		//	@brief		ファイルオープンメソッド
		//	@return		S_OK：オープン成功　E_FAIL：オープン失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	HRESULT	OpenFile();

		/*!-----------------------------------------------------------
		//	@brief		ファイルセーブメソッド
		//	@return		S_OK：セーブ成功　E_FAIL：セーブ失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	HRESULT	SaveFile();

		/*!-----------------------------------------------------------
		//	@brief		最後に取得した書き込み用のファイルパスを取得
		//	@param[in]	_bufferSize	コピー先出力となる文字列バッファサイズ
		//	@param[out]	_pOut		コピー出力先となる文字列バッファポインタ
		//	@return		コピー出力となる文字列バッファのポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	TCHAR*	GetLastWriteFilePath(const size_t _bufferSize = 0, TCHAR * _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		最後に取得した書き込み用のファイル名を取得
		//	@param[in]	_bufferSize	コピー先出力となる文字列バッファサイズ
		//	@param[out]	_pOut		コピー出力先となる文字列バッファポインタ
		//	@return		コピー出力となる文字列バッファのポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	TCHAR*	GetLastWriteFileName(const size_t _bufferSize = 0, TCHAR * _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		最後に取得した読み込み用のファイルパスを取得
		//	@param[in]	_bufferSize	コピー先出力となる文字列バッファサイズ
		//	@param[out]	_pOut		コピー出力先となる文字列バッファポインタ
		//	@return		コピー出力となる文字列バッファのポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/09
		------------------------------------------------------------*/
		virtual	TCHAR*	GetLastReadFilePath(const size_t _bufferSize = 0,TCHAR * _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		最後に取得した読み込み用のファイル名を取得
		//	@param[in]	_bufferSize	コピー先出力となる文字列バッファサイズ
		//	@param[out]	_pOut		コピー出力先となる文字列バッファポインタ
		//	@return		コピー出力となる文字列バッファのポインタ
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