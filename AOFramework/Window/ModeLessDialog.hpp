/*************************************************************
*	@file   	ModeLessDialog.hpp
*	@brief  	ModeLessDialogクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/10/10
*************************************************************/

#ifndef _Include_ModeLessDialog_hpp_	// インクルードガード
#define _Include_ModeLessDialog_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include"Window.hpp"
#include"../Define/SystemDefine.hpp"

namespace ao
{
	class Window;

	/*!-----------------------------------------------------------
	//	@class  ModeLessDialog
	//	@brief  モードレスダイアログ
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/10
	------------------------------------------------------------*/
	class ModeLessDialog
	{
	public:
		ModeLessDialog();
		virtual ~ModeLessDialog();

		/*!-----------------------------------------------------------
		//	@brief		ダイアログボックスの作成
		//	@param[in]	_pWindow	親ウィンドウ
		//	@param[in]	_resName	リソース文字列
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		virtual	HRESULT	Initialize(
			Window* _pWindow,
			CONST AOLPCSTR _resourceId);

		/*!-----------------------------------------------------------
		//	@brief		ダイアログ破棄
		//	@return		0：破棄成功　0以外：破棄失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		virtual	BOOL Destory();

		/*!-----------------------------------------------------------
		//	@brief		メッセージの更新処理
		//	@param[in]	_pMsg	メッセージポインタ
		//	@return		0以外：成功　0：失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		virtual	BOOL Update(LPMSG _pMsg);

		/*!-----------------------------------------------------------
		//	@brief		継承させて使用する仮想コールバック関数
		//	@param[in]	_hDialog	ダイアログハンドル
		//	@param[in]	_msg		メッセージ
		//	@param[in]	_wParam		パラメーター１
		//	@param[in]	_lParam		パラメーター２
		//	@return		TRUE：正常処理　FALSE：デフォルト処理
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		virtual	INT_PTR DialogProc(
			HWND _hDialog,
			UINT _msg,
			WPARAM _wParam,
			LPARAM _lParam);

		/*!-----------------------------------------------------------
		//	@brief		共通ダイアログプロシージャ
		//	@param[in]	_hDialog	ダイアログハンドル
		//	@param[in]	_mgs		メッセージ
		//	@param[in]	_wParam		パラメーター１
		//	@param[in]	_lParam		パラメーター２
		//	@return		TRUE：ダイアログメッセージ処理	FALSE：ダイアログメッセージではない
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		static	INT_PTR CALLBACK CommonDialogProc(
			HWND _hDialog,
			UINT _msg,
			WPARAM _wParam,
			LPARAM _lParam);

		/*!-----------------------------------------------------------
		//	@brief		ダイログハンドル取得
		//	@return		ダイログハンドル
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/30
		------------------------------------------------------------*/
		virtual	CONST HWND	GetHDialog() const {
			return this->hDialog;
		}

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウがアクティブかどうか
		//	@return		TRUE：アクティブ　FALSE：非アクティブ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/13
		------------------------------------------------------------*/
		BOOL IsActive(){
			return (this->hDialog == ::GetForegroundWindow());
		}

	protected:
		HWND hDialog;
	};
}

#endif // _Include_ModeLessDialog_hpp_