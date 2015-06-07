#include"stdafx.hpp"
#include"Window.hpp"
#include"../Input/Mouse.hpp"


using namespace ao;
UINT Window::sNumWindows = 0;
std::mutex Window::sMutex;


Window::Window() :
hWnd(nullptr),
isDead(FALSE),
isParent(FALSE),
pModeLessDialog(nullptr)
{
	::SecureZeroMemory(&this->msg, sizeof(this->msg));
	::SecureZeroMemory(&this->winEx, sizeof(this->winEx));
}


Window::~Window()
{
	::DestroyWindow(this->hWnd);
	::UnregisterClass(this->regName.c_str(), this->winEx.hInstance);
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/30	22:30	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT Window::Initialize(
	CONST HINSTANCE _hInst,
	CONST UINT _nWidth,
	CONST UINT _nHeight,
	CONST BOOL _isFullWindow,
	CONST HWND _hWndParent)
{
	HRESULT hr = this->Register(_hInst);
#ifdef __AO_DEBUG__
	if (FAILED(hr))
		return hr;
#endif
	//ウィンドウ作成
	if (!this->CreateMyWindow(_nWidth, _nHeight, _isFullWindow, _hWndParent))
		return E_FAIL;

	//ウィンドウが親かどうか
	this->isParent = (_hWndParent == nullptr) ? TRUE : FALSE;

	//ウィンドウハンドルを自身と関連付ける
	::SetWindowLong(hWnd, AO_GW_USERDATA, (LONG)this);

	//ウィンドウ表示・更新
	::ShowWindow(this->hWnd, SW_SHOWNORMAL);
	::UpdateWindow(this->hWnd);

	//マウスにウィンドウを登録
	Mouse::GetInstance()->SetWindowHandle(this->hWnd);

	//ウィンドウ稼働数加算
	std::lock_guard< std::mutex > lock(sMutex);
	++sNumWindows;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		プロセス処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	00:46	T.Aoyama	作成
//------------------------------------------------------------------------------
void Window::ProcessMessage()
{
	//メッセージ取出し
	if (PeekMessage(&this->msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (this->pModeLessDialog && ::IsDialogMessage(this->pModeLessDialog->GetHDialog(), &this->msg))
		{
			//モードレスダイアログ処理
			this->pModeLessDialog->Update(&this->msg);
		}
		else
		{
			//メッセージ処理
			TranslateMessage(&this->msg);
			DispatchMessage(&this->msg);
		}
	}
}


//------------------------------------------------------------------------------
/**	@brief		共通のコールバック関数 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07:31	00:43	T.Aoyama	作成
//------------------------------------------------------------------------------
LRESULT	CALLBACK	Window::CommonWndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	//ポインタ取得
	Window* pWindow = (Window*)::GetWindowLong(_hWnd, AO_GW_USERDATA);

	//ポインタ取得
	if (pWindow)
	{
		//オーバーライドしたウィンドウプロシージャの処理
		return pWindow->WndProc(_hWnd, _msg, _wParam, _lParam);
	}

	//ポインタ取得に失敗したら通常処理
	return	DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}


//------------------------------------------------------------------------------
/**	@brief		基底クラスのコールバックメソッド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07:31	00:43	T.Aoyama	作成
//------------------------------------------------------------------------------
LRESULT	CALLBACK	Window::WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_MOUSEWHEEL:
		Mouse::GetInstance()->UpdateWheel(GET_WHEEL_DELTA_WPARAM(_wParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		this->isDead = TRUE;
		break;
	case WM_ACTIVATE:
		switch (LOWORD(_wParam))
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			break;
		}
		break;
	default:
		return DefWindowProc(hWnd, _msg, _wParam, _lParam);
	}

	return	0;
}


//------------------------------------------------------------------------------
/**	@brief		レジストリ登録 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/30	23:41	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT Window::Register(CONST HINSTANCE _hInst)
{
	ao::ostringstream stream;
	stream << "_" << sNumWindows;

	//ウィンドウレジストリ名生成
	this->regName.assign(_T("WindowRegisterName"));
	this->regName.append(stream.str());

	//Windows登録情報設定
	this->winEx.cbSize = sizeof(WNDCLASSEX);
	this->winEx.style = CS_HREDRAW | CS_VREDRAW;
	this->winEx.hInstance = _hInst;
	this->winEx.lpszClassName = this->regName.c_str();
	this->winEx.lpfnWndProc = (WNDPROC)Window::CommonWndProc;
	this->winEx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	this->winEx.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	this->winEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	this->winEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	this->winEx.lpszMenuName = nullptr;
	this->winEx.cbClsExtra = 0;
	this->winEx.cbWndExtra = 0;

	//情報登録
	if (!RegisterClassEx(&this->winEx))
	{
		MessageBox(nullptr, _T("Design error"), _T("ERROR"), MB_OK);
		return	E_FAIL;
	}

	return	S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		ウィンドウ作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	00:35	T.Aoyama	作成
//------------------------------------------------------------------------------
INT_PTR Window::CreateMyWindow(
	UINT _nWidth,
	UINT _nHeight,
	BOOL _isFullScreen,
	HWND _hWndParent
	)
{
	DWORD dwStyle = 0;

	//ウィンドウ画面設計
	if (_isFullScreen)
	{
		dwStyle = WS_VISIBLE | WS_POPUP;
	}
	else
	{
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	//ウィンドウとクライアント領域算出
	RECT rc = { 0, 0, _nWidth, _nHeight };
	::AdjustWindowRect(&rc, dwStyle, FALSE);
	this->client.width = _nWidth;
	this->client.height = _nHeight;
	this->window.width = rc.right - rc.left;
	this->window.height = rc.bottom - rc.top;

	POINT showPos;
	showPos.x = (GetSystemMetrics(SM_CXSCREEN) - (this->window.width)) / 2;
	showPos.y = (GetSystemMetrics(SM_CYSCREEN) - (this->window.height)) / 2;

	//ウィンドウ作成
	this->hWnd = ::CreateWindow(
		this->winEx.lpszClassName,
		_T("DefaultWindowTitle"),
		dwStyle,
		showPos.x,
		showPos.y,
		this->window.width,
		this->window.height,
		_hWndParent,
		nullptr,
		this->winEx.hInstance,
		this);

	//エラーチェック
#ifdef __AO_DEBUG__
	if (this->hWnd == nullptr)
	{
		MessageBox(nullptr, _T("\nFailed create window"), _T("ERROR"), MB_ABORTRETRYIGNORE);
		return	FALSE;
	}
#endif

	return TRUE;
}