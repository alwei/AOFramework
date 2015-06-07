#include"stdafx.hpp"
#include"ModeLessDialog.hpp"


using namespace ao;


ModeLessDialog::ModeLessDialog() : 
hDialog(nullptr)
{

}


ModeLessDialog::~ModeLessDialog()
{
	::EndDialog(this->hDialog, 0);
	::DestroyWindow(this->hDialog);
}


//------------------------------------------------------------------------------
/**	@brief		ダイアログボックス作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	20:11	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT	ModeLessDialog::Initialize(
	ao::Window* _pWindow,
	CONST AOLPCSTR _reourceName)
{
	_pWindow->ResiterModeLessDialog(this);
	this->hDialog = ::CreateDialog(_pWindow->GetHInstance(), _reourceName, nullptr, ModeLessDialog::CommonDialogProc);
	::SetWindowLong(this->hDialog, AO_GW_USERDATA, (LONG)this);
	::ShowWindow(this->hDialog, SW_SHOW);

	if (this->hDialog == nullptr)
	{
		return E_FAIL;
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		ダイアログ破棄 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	20:11	T.Aoyama	作成
//------------------------------------------------------------------------------
BOOL ModeLessDialog::Destory()
{
	BOOL res = ::DestroyWindow(this->hDialog);
	this->hDialog = nullptr;

	return res;
}


//------------------------------------------------------------------------------
/**	@brief		更新処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	20:11	T.Aoyama	作成
//------------------------------------------------------------------------------
BOOL ModeLessDialog::Update(LPMSG pMsg)
{
	if (this->hDialog == nullptr)
	{
		return FALSE;
	}

	return TRUE;
}


//------------------------------------------------------------------------------
/**	@brief		コールバック処理（オーバーライドして使用する） */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	20:11	T.Aoyama	作成
//------------------------------------------------------------------------------
INT_PTR ModeLessDialog::DialogProc(HWND _hDialog, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (this->hDialog == nullptr)
	{
		return FALSE;
	}

	switch (_msg)
	{
	case WM_COMMAND:	//コマンド入力時（派生先で処理）
		return TRUE;

	case WM_CLOSE:		//ダイアログボックスを消す
		::ShowWindow(this->hDialog, SW_HIDE);
		return TRUE;
	}

	return FALSE;
}


//------------------------------------------------------------------------------
/**	@brief		共通のコールバック関数 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	20:11	T.Aoyama	作成
//------------------------------------------------------------------------------
INT_PTR CALLBACK ModeLessDialog::CommonDialogProc(
	HWND _hDialog,
	UINT _msg,
	WPARAM _wParam,
	LPARAM _lParam)
{
	//ポインタ取得
	ModeLessDialog* pDialog = (ModeLessDialog*)::GetWindowLong(_hDialog, AO_GW_USERDATA);

	//ポインタ取得
	if (pDialog)
	{
		//オーバーライドしたウィンドウプロシージャの処理
		return pDialog->DialogProc(_hDialog,_msg, _wParam, _lParam);
	}

	return FALSE;
}