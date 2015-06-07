/*************************************************************
*	@file   	Window.hpp
*	@brief  	Windowクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/07/30
*************************************************************/

#ifndef _Include_Window_hpp_	// インクルードガード
#define _Include_Window_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<tchar.h>
#include<sstream>
#include<Windows.h>
#include<mutex>
#include"ModeLessDialog.hpp"
#include"../Define/SystemDefine.hpp"

namespace ao
{
	class ModeLessDialog;

	/*!-----------------------------------------------------------
	//	@class  Window
	//	@brief  ウィンドウクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/30
	------------------------------------------------------------*/
	class Window
	{
	public:
		Window();
		virtual	~Window();

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]  _hInst			インスタンスハンドル
		//	@param[in]  _nWidth			ウィンドウ横幅[省略可]
		//	@param[in]  _nHeight		ウィンドウ縦幅[省略可]
		//	@param[in]  _isFullScreen	フルスクリーンフラグ[省略可]
		//	@param[in]  _hWndParent		親ウィンドウハンドル[省略可]
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual HRESULT	Initialize(
			CONST HINSTANCE _hInst,
			CONST UINT _nWidth = 640,
			CONST UINT _nHeight = 480,
			CONST BOOL _isFullWindow = FALSE,
			CONST HWND _hWndParent = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウのメッセージ処理
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	void	ProcessMessage();

		/*!-----------------------------------------------------------
		//	@brief		継承させて使用する仮想コールバック関数
		//	@param[in]	_hWnd   ウィンドウハンドル
		//	@param[in]	_msg    翻訳されたメッセージ
		//	@param[in]	_wParam 状態によってパラメーターが変わる情報ビット変数
		//	@param[in]	_lParam 状態によってパラメーターが変わる情報ビット変数
		//	@return		ウィンドウの状態
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	LRESULT	CALLBACK	WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

		/*!-----------------------------------------------------------
		//	@brief		共通コールバック
		//	@param[in]	_hWnd   ウィンドウハンドル
		//	@param[in]	_msg    翻訳されたメッセージ
		//	@param[in]	_wParam 状態によってパラメーターが変わる情報ビット変数
		//	@param[in]	_lParam 状態によってパラメーターが変わる情報ビット変数
		//	@return		ウィンドウの状態
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		static	LRESULT	CALLBACK	CommonWndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

		/*!-----------------------------------------------------------
		//	@brief		モードレスダイアログウィンドウ登録
		//	@param[in]	_pModeLessDialog	登録するモードレスダイアログ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/11
		------------------------------------------------------------*/
		virtual void ResiterModeLessDialog(ModeLessDialog* _pModeLessDialog)
		{
			this->pModeLessDialog = _pModeLessDialog;
		}


		/*!-----------------------------------------------------------
		//	@brief		モードレスダイアログの解放
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/20
		------------------------------------------------------------*/
		virtual void ReleaseModeLessDialog()
		{
			this->pModeLessDialog = nullptr;
		}

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウタイトル設定
		//	@param[in]	_titleName	タイトル名
		//	@return		0以外：成功　0：失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	BOOL	SetWindowTitle(AOLPCSTR _titleName = _T("NO NAME")) const 
		{
			return ::SetWindowText(this->hWnd, _titleName);
		}

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウハンドル取得
		//	@return		ウィンドウハンドル
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	CONST HWND	GetHWnd() const 
		{
			return this->hWnd;
		}

		/*!-----------------------------------------------------------
		//	@brief		インスタンスハンドル取得
		//	@return		インスタンスハンドル
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/11
		------------------------------------------------------------*/
		virtual CONST HINSTANCE GetHInstance() const 
		{
			return this->winEx.hInstance;
		}

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウ横幅取得
		//	@return		ウィンドウの横幅
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	UINT	GetWidth() const 
		{
			return this->window.width;
		}

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウ高幅取得
		//	@return		ウィンドウの高さ
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	UINT	GetHeight() const 
		{
			return this->window.height;
		}

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウのクライアント領域の横幅取得
		//	@return		クライアント領域の横幅
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/21
		------------------------------------------------------------*/
		virtual	UINT	GetClientWidth() const 
		{
			return this->client.width;
		}

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウのクライアント領域の縦幅取得
		//	@return		クライアント領域の縦幅
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/21
		------------------------------------------------------------*/
		virtual	UINT	GetClientHeight() const 
		{
			return this->client.height;
		}

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウがアクティブかどうか
		//	@return		TRUE：アクティブ　FALSE：非アクティブ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		BOOL IsActive()
		{
			return (this->hWnd == ::GetForegroundWindow());
		}

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウが破棄されたかどうか
		//	@return		TRUE：破棄された　FALSE：破棄されていない
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	BOOL	IsDestroyed()
		{
			return this->isDead;
		}

		/*!-----------------------------------------------------------
		//	@brief		現在稼働しているウィンドウ数を取得
		//	@return		現在稼働しているウィンドウ数
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		static	UINT	GetNumWindow()
		{
			return Window::sNumWindows;
		}

	private:
		typedef struct _SIZE
		{
			UINT width;
			UINT height;
		}SIZE;

	protected:
		HWND hWnd;		///< ウィンドウハンドル
		SIZE window;	///< ウィンドウ領域のサイズ
		SIZE client;	///< クライアント領域のサイズ
		BOOL isDead;	///< 破棄されたかどうか
		BOOL isParent;	///< 親フラグ
		MSG			msg;		///< メッセージ
		WNDCLASSEX	winEx;		///< ウィンドウレジスタ
		ao::string	regName;	///< レジストリ名
		ao::ModeLessDialog* pModeLessDialog;	///< モードレスダイアログ

		static std::mutex sMutex;	///< 排他処理用のmutex
		static UINT sNumWindows;	///< 現在稼働しているウィンドウ数

	protected:
		/*!-----------------------------------------------------------
		//	@brief		レジストリ登録
		//	@param[in]	_hInst	インスタンスハンドル
		//	@return		S_OK：登録成功　それ以外：登録失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	HRESULT	Register(CONST HINSTANCE _hInst);

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウ作成
		//	@param[in]	_nWidth			ウィンドウ横幅
		//	@param[in]  _nHeight		ウィンドウ縦幅
		//	@param[in]  _isFullScreen	フルスクリーンフラグ
		//	@param[in]  _hWndParent		親ウィンドウハンドル
		//	@return		TRUE：作成成功　FALSE：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		virtual	INT_PTR CreateMyWindow(
			UINT _nWidth,
			UINT _nHeight,
			BOOL _isFullScreen,
			HWND _hWndParent
			);
	};
}

#endif // _Include_Window_hpp_
