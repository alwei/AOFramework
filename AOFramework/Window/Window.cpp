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
/**	@brief		������ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/30	22:30	T.Aoyama	�쐬
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
	//�E�B���h�E�쐬
	if (!this->CreateMyWindow(_nWidth, _nHeight, _isFullWindow, _hWndParent))
		return E_FAIL;

	//�E�B���h�E���e���ǂ���
	this->isParent = (_hWndParent == nullptr) ? TRUE : FALSE;

	//�E�B���h�E�n���h�������g�Ɗ֘A�t����
	::SetWindowLong(hWnd, AO_GW_USERDATA, (LONG)this);

	//�E�B���h�E�\���E�X�V
	::ShowWindow(this->hWnd, SW_SHOWNORMAL);
	::UpdateWindow(this->hWnd);

	//�}�E�X�ɃE�B���h�E��o�^
	Mouse::GetInstance()->SetWindowHandle(this->hWnd);

	//�E�B���h�E�ғ������Z
	std::lock_guard< std::mutex > lock(sMutex);
	++sNumWindows;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�v���Z�X���� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	00:46	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void Window::ProcessMessage()
{
	//���b�Z�[�W��o��
	if (PeekMessage(&this->msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (this->pModeLessDialog && ::IsDialogMessage(this->pModeLessDialog->GetHDialog(), &this->msg))
		{
			//���[�h���X�_�C�A���O����
			this->pModeLessDialog->Update(&this->msg);
		}
		else
		{
			//���b�Z�[�W����
			TranslateMessage(&this->msg);
			DispatchMessage(&this->msg);
		}
	}
}


//------------------------------------------------------------------------------
/**	@brief		���ʂ̃R�[���o�b�N�֐� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07:31	00:43	T.Aoyama	�쐬
//------------------------------------------------------------------------------
LRESULT	CALLBACK	Window::CommonWndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	//�|�C���^�擾
	Window* pWindow = (Window*)::GetWindowLong(_hWnd, AO_GW_USERDATA);

	//�|�C���^�擾
	if (pWindow)
	{
		//�I�[�o�[���C�h�����E�B���h�E�v���V�[�W���̏���
		return pWindow->WndProc(_hWnd, _msg, _wParam, _lParam);
	}

	//�|�C���^�擾�Ɏ��s������ʏ폈��
	return	DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}


//------------------------------------------------------------------------------
/**	@brief		���N���X�̃R�[���o�b�N���\�b�h */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07:31	00:43	T.Aoyama	�쐬
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
/**	@brief		���W�X�g���o�^ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/30	23:41	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT Window::Register(CONST HINSTANCE _hInst)
{
	ao::ostringstream stream;
	stream << "_" << sNumWindows;

	//�E�B���h�E���W�X�g��������
	this->regName.assign(_T("WindowRegisterName"));
	this->regName.append(stream.str());

	//Windows�o�^���ݒ�
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

	//���o�^
	if (!RegisterClassEx(&this->winEx))
	{
		MessageBox(nullptr, _T("Design error"), _T("ERROR"), MB_OK);
		return	E_FAIL;
	}

	return	S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�E�B���h�E�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	00:35	T.Aoyama	�쐬
//------------------------------------------------------------------------------
INT_PTR Window::CreateMyWindow(
	UINT _nWidth,
	UINT _nHeight,
	BOOL _isFullScreen,
	HWND _hWndParent
	)
{
	DWORD dwStyle = 0;

	//�E�B���h�E��ʐ݌v
	if (_isFullScreen)
	{
		dwStyle = WS_VISIBLE | WS_POPUP;
	}
	else
	{
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	//�E�B���h�E�ƃN���C�A���g�̈�Z�o
	RECT rc = { 0, 0, _nWidth, _nHeight };
	::AdjustWindowRect(&rc, dwStyle, FALSE);
	this->client.width = _nWidth;
	this->client.height = _nHeight;
	this->window.width = rc.right - rc.left;
	this->window.height = rc.bottom - rc.top;

	POINT showPos;
	showPos.x = (GetSystemMetrics(SM_CXSCREEN) - (this->window.width)) / 2;
	showPos.y = (GetSystemMetrics(SM_CYSCREEN) - (this->window.height)) / 2;

	//�E�B���h�E�쐬
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

	//�G���[�`�F�b�N
#ifdef __AO_DEBUG__
	if (this->hWnd == nullptr)
	{
		MessageBox(nullptr, _T("\nFailed create window"), _T("ERROR"), MB_ABORTRETRYIGNORE);
		return	FALSE;
	}
#endif

	return TRUE;
}