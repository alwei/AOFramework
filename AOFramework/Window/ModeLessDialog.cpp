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
/**	@brief		�_�C�A���O�{�b�N�X�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	20:11	T.Aoyama	�쐬
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
/**	@brief		�_�C�A���O�j�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	20:11	T.Aoyama	�쐬
//------------------------------------------------------------------------------
BOOL ModeLessDialog::Destory()
{
	BOOL res = ::DestroyWindow(this->hDialog);
	this->hDialog = nullptr;

	return res;
}


//------------------------------------------------------------------------------
/**	@brief		�X�V���� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	20:11	T.Aoyama	�쐬
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
/**	@brief		�R�[���o�b�N�����i�I�[�o�[���C�h���Ďg�p����j */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	20:11	T.Aoyama	�쐬
//------------------------------------------------------------------------------
INT_PTR ModeLessDialog::DialogProc(HWND _hDialog, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (this->hDialog == nullptr)
	{
		return FALSE;
	}

	switch (_msg)
	{
	case WM_COMMAND:	//�R�}���h���͎��i�h����ŏ����j
		return TRUE;

	case WM_CLOSE:		//�_�C�A���O�{�b�N�X������
		::ShowWindow(this->hDialog, SW_HIDE);
		return TRUE;
	}

	return FALSE;
}


//------------------------------------------------------------------------------
/**	@brief		���ʂ̃R�[���o�b�N�֐� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	20:11	T.Aoyama	�쐬
//------------------------------------------------------------------------------
INT_PTR CALLBACK ModeLessDialog::CommonDialogProc(
	HWND _hDialog,
	UINT _msg,
	WPARAM _wParam,
	LPARAM _lParam)
{
	//�|�C���^�擾
	ModeLessDialog* pDialog = (ModeLessDialog*)::GetWindowLong(_hDialog, AO_GW_USERDATA);

	//�|�C���^�擾
	if (pDialog)
	{
		//�I�[�o�[���C�h�����E�B���h�E�v���V�[�W���̏���
		return pDialog->DialogProc(_hDialog,_msg, _wParam, _lParam);
	}

	return FALSE;
}