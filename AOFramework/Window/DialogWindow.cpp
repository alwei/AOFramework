#include"stdafx.hpp"
#include"DialogWindow.hpp"


using namespace ao;


DialogWindow::DialogWindow()
{

}


DialogWindow::~DialogWindow()
{

}


//------------------------------------------------------------------------------
/**	@brief		�_�C�A���O������ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:27	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DialogWindow::InitializeDialog(string _fileName,string _fileExt)
{
	// �\���̂ɏ����Z�b�g
	::SecureZeroMemory(&this->ofn, sizeof(this->ofn));
	this->ofn.lStructSize = sizeof(this->ofn);		// �\���̂̃T�C�Y
	this->ofn.hwndOwner = this->hWnd;				// �R�����_�C�A���O�̐e�E�B���h�E�n���h��
	// �t�@�C���̎��
	this->readInfo.filter = _fileName + _T(" file(*.") + _fileExt + _T(")@*.") + _fileExt + _T("@All files(*.*)@*.*@@");
	for (auto& it : this->readInfo.filter){
		if (it == _T('@'))
			it = 0;
	}
	this->readInfo.title = _T("Open [") + _fileName + _T("] file");
	this->readInfo.defExt = _fileExt;
	this->ofn.lpstrFilter = this->readInfo.filter.c_str();
	this->ofn.lpstrFile = this->read.filePath.data();								// �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
	this->ofn.lpstrFileTitle = this->read.fileName.data();							// �I�����ꂽ�t�@�C�������󂯎��ϐ��̃A�h���X
	this->ofn.nMaxFile = (DWORD)(this->read.filePath.size()*sizeof(TCHAR));			// lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
	this->ofn.nMaxFileTitle = (DWORD)(this->read.fileName.size()*sizeof(TCHAR));	// lpstrFileTitle�Ɏw�肵���ϐ��̃T�C�Y
	this->ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;							// �t���O�w��
	this->ofn.lpstrTitle = this->readInfo.title.c_str();							// �R�����_�C�A���O�̃L���v�V����
	this->ofn.lpstrDefExt = this->readInfo.defExt.c_str();							// �f�t�H���g�̃t�@�C���̎��


	// �\���̂ɏ����Z�b�g
	::SecureZeroMemory(&this->ifn, sizeof(this->ifn));
	this->ifn.lStructSize = sizeof(this->ifn);	    // �\���̂̃T�C�Y
	this->ifn.hwndOwner = this->hWnd;			    // �R�����_�C�A���O�̐e�E�B���h�E�n���h��
	// �t�@�C���̎��
	this->writeInfo.filter = _fileName + _T(" file(*.") + _fileExt + _T(")@*.") + _fileExt + _T("@All files(*.*)@*.*@@");
	for (auto& it : this->writeInfo.filter){
		if (it == _T('@'))
			it = 0;
	}
	this->writeInfo.title = _T("Save [") + _fileName + _T("] file");
	this->writeInfo.defExt = _fileExt;
	this->ifn.lpstrFilter = this->writeInfo.filter.c_str();
	this->ifn.lpstrFile = this->write.filePath.data();								// �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
	this->ifn.lpstrFileTitle = this->write.fileName.data();							// �I�����ꂽ�t�@�C�������󂯎��ϐ��̃A�h���X
	this->ifn.nMaxFile = (DWORD)(this->write.filePath.size()*sizeof(TCHAR));		// lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
	this->ifn.nMaxFileTitle = (DWORD)(this->write.fileName.size()*sizeof(TCHAR));	// lpstrFileTitle�Ɏw�肵���ϐ��̃T�C�Y
	this->ifn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;						// �t���O�w��
	this->ifn.lpstrTitle = this->writeInfo.title.c_str();							// �R�����_�C�A���O�̃L���v�V����
	this->ifn.lpstrDefExt = this->writeInfo.defExt.c_str();							// �f�t�H���g�̃t�@�C���̎��

	//�h���b�O����������
	::DragAcceptFiles(this->hWnd, FALSE);
}


//------------------------------------------------------------------------------
/**	@brief		�h���b�v�t�@�C���̃v���Z�X���� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:27	T.Aoyama	�쐬
//------------------------------------------------------------------------------
BOOL DialogWindow::ProcessDropFile(HWND _hWnd, DWORD _wParam)
{
	//�t�@�C�����𒲂ׂ�
	DWORD fileNum = DragQueryFile((HDROP)_wParam, 0xFFFFFFFF, nullptr, NULL);

	//�������ƃG���[
	if (fileNum > 1){
		MessageBox(hWnd, _T("�����t�@�C���͕s�B"), _T("ERROR"), MB_ICONWARNING);
		return FALSE;
	}

	//�h���b�v���ꂽ�t�@�C���𒲂ׂ�
	::DragQueryFile((HDROP)_wParam, 0, this->read.filePath.data(),(UINT)(this->read.filePath.size()*sizeof(TCHAR)));
	//�t�@�C���p�X����t�@�C�������o
	macro::GetFileNameFromPath(this->read.fileName.data(), this->read.fileName.size()*sizeof(TCHAR), this->read.filePath.data());

	//�I������
	::DragFinish((HDROP)_wParam);

	return TRUE;
}


//------------------------------------------------------------------------------
/**	@brief		�t�@�C���I�[�v�����\�b�h */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:28	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DialogWindow::OpenFile()
{
	if (!::GetOpenFileName(&this->ofn))
		return E_FAIL;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�t�@�C���Z�[�u���\�b�h */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:29	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DialogWindow::SaveFile()
{
	if (!::GetSaveFileName(&this->ifn))
		return E_FAIL;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�Ō�Ɏ擾�����������݃p�X�擾 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:43	T.Aoyama	�쐬
//------------------------------------------------------------------------------
TCHAR* DialogWindow::GetLastWriteFilePath(const size_t _bufferSize, TCHAR* _pOut)
{
	if (_pOut == nullptr)
		return this->write.filePath.data();

	ao_strcpy_s(_pOut, _bufferSize, this->write.filePath.data());
	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		�Ō�Ɏ擾�����������ݗp�̃t�@�C�������擾 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:45	T.Aoyama	�쐬
//------------------------------------------------------------------------------
TCHAR* DialogWindow::GetLastWriteFileName(const size_t _bufferSize, TCHAR* _pOut)
{
	if (_pOut == nullptr)
		return this->write.fileName.data();

	ao_strcpy_s(_pOut, _bufferSize, this->write.fileName.data());
	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		�Ō�Ɏ擾�����ǂݍ��ݗp�p�X�擾 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:43	T.Aoyama	�쐬
//------------------------------------------------------------------------------
TCHAR* DialogWindow::GetLastReadFilePath(const size_t _bufferSize, TCHAR* _pOut)
{
	if (_pOut == nullptr)
		return this->read.filePath.data();

	ao_strcpy_s(_pOut, _bufferSize, this->read.filePath.data());
	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		�Ō�Ɏ擾�����ǂݍ��ݗp�̃t�@�C�������擾 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:45	T.Aoyama	�쐬
//------------------------------------------------------------------------------
TCHAR* DialogWindow::GetLastReadFileName(const size_t _bufferSize,TCHAR* _pOut)
{
	if (_pOut == nullptr)
		return this->read.fileName.data();

	ao_strcpy_s(_pOut, _bufferSize, this->read.fileName.data());
	return _pOut;
}