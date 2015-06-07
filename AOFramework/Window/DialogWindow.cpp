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
/**	@brief		ダイアログ初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:27	T.Aoyama	作成
//------------------------------------------------------------------------------
void DialogWindow::InitializeDialog(string _fileName,string _fileExt)
{
	// 構造体に情報をセット
	::SecureZeroMemory(&this->ofn, sizeof(this->ofn));
	this->ofn.lStructSize = sizeof(this->ofn);		// 構造体のサイズ
	this->ofn.hwndOwner = this->hWnd;				// コモンダイアログの親ウィンドウハンドル
	// ファイルの種類
	this->readInfo.filter = _fileName + _T(" file(*.") + _fileExt + _T(")@*.") + _fileExt + _T("@All files(*.*)@*.*@@");
	for (auto& it : this->readInfo.filter){
		if (it == _T('@'))
			it = 0;
	}
	this->readInfo.title = _T("Open [") + _fileName + _T("] file");
	this->readInfo.defExt = _fileExt;
	this->ofn.lpstrFilter = this->readInfo.filter.c_str();
	this->ofn.lpstrFile = this->read.filePath.data();								// 選択されたファイル名(フルパス)を受け取る変数のアドレス
	this->ofn.lpstrFileTitle = this->read.fileName.data();							// 選択されたファイル名を受け取る変数のアドレス
	this->ofn.nMaxFile = (DWORD)(this->read.filePath.size()*sizeof(TCHAR));			// lpstrFileに指定した変数のサイズ
	this->ofn.nMaxFileTitle = (DWORD)(this->read.fileName.size()*sizeof(TCHAR));	// lpstrFileTitleに指定した変数のサイズ
	this->ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;							// フラグ指定
	this->ofn.lpstrTitle = this->readInfo.title.c_str();							// コモンダイアログのキャプション
	this->ofn.lpstrDefExt = this->readInfo.defExt.c_str();							// デフォルトのファイルの種類


	// 構造体に情報をセット
	::SecureZeroMemory(&this->ifn, sizeof(this->ifn));
	this->ifn.lStructSize = sizeof(this->ifn);	    // 構造体のサイズ
	this->ifn.hwndOwner = this->hWnd;			    // コモンダイアログの親ウィンドウハンドル
	// ファイルの種類
	this->writeInfo.filter = _fileName + _T(" file(*.") + _fileExt + _T(")@*.") + _fileExt + _T("@All files(*.*)@*.*@@");
	for (auto& it : this->writeInfo.filter){
		if (it == _T('@'))
			it = 0;
	}
	this->writeInfo.title = _T("Save [") + _fileName + _T("] file");
	this->writeInfo.defExt = _fileExt;
	this->ifn.lpstrFilter = this->writeInfo.filter.c_str();
	this->ifn.lpstrFile = this->write.filePath.data();								// 選択されたファイル名(フルパス)を受け取る変数のアドレス
	this->ifn.lpstrFileTitle = this->write.fileName.data();							// 選択されたファイル名を受け取る変数のアドレス
	this->ifn.nMaxFile = (DWORD)(this->write.filePath.size()*sizeof(TCHAR));		// lpstrFileに指定した変数のサイズ
	this->ifn.nMaxFileTitle = (DWORD)(this->write.fileName.size()*sizeof(TCHAR));	// lpstrFileTitleに指定した変数のサイズ
	this->ifn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;						// フラグ指定
	this->ifn.lpstrTitle = this->writeInfo.title.c_str();							// コモンダイアログのキャプション
	this->ifn.lpstrDefExt = this->writeInfo.defExt.c_str();							// デフォルトのファイルの種類

	//ドラッグ処理を許可
	::DragAcceptFiles(this->hWnd, FALSE);
}


//------------------------------------------------------------------------------
/**	@brief		ドロップファイルのプロセス処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:27	T.Aoyama	作成
//------------------------------------------------------------------------------
BOOL DialogWindow::ProcessDropFile(HWND _hWnd, DWORD _wParam)
{
	//ファイル数を調べる
	DWORD fileNum = DragQueryFile((HDROP)_wParam, 0xFFFFFFFF, nullptr, NULL);

	//複数だとエラー
	if (fileNum > 1){
		MessageBox(hWnd, _T("複数ファイルは不可。"), _T("ERROR"), MB_ICONWARNING);
		return FALSE;
	}

	//ドロップされたファイルを調べる
	::DragQueryFile((HDROP)_wParam, 0, this->read.filePath.data(),(UINT)(this->read.filePath.size()*sizeof(TCHAR)));
	//ファイルパスからファイル名抽出
	macro::GetFileNameFromPath(this->read.fileName.data(), this->read.fileName.size()*sizeof(TCHAR), this->read.filePath.data());

	//終了処理
	::DragFinish((HDROP)_wParam);

	return TRUE;
}


//------------------------------------------------------------------------------
/**	@brief		ファイルオープンメソッド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:28	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DialogWindow::OpenFile()
{
	if (!::GetOpenFileName(&this->ofn))
		return E_FAIL;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		ファイルセーブメソッド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:29	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DialogWindow::SaveFile()
{
	if (!::GetSaveFileName(&this->ifn))
		return E_FAIL;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		最後に取得した書き込みパス取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:43	T.Aoyama	作成
//------------------------------------------------------------------------------
TCHAR* DialogWindow::GetLastWriteFilePath(const size_t _bufferSize, TCHAR* _pOut)
{
	if (_pOut == nullptr)
		return this->write.filePath.data();

	ao_strcpy_s(_pOut, _bufferSize, this->write.filePath.data());
	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		最後に取得した書き込み用のファイル名を取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:45	T.Aoyama	作成
//------------------------------------------------------------------------------
TCHAR* DialogWindow::GetLastWriteFileName(const size_t _bufferSize, TCHAR* _pOut)
{
	if (_pOut == nullptr)
		return this->write.fileName.data();

	ao_strcpy_s(_pOut, _bufferSize, this->write.fileName.data());
	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		最後に取得した読み込み用パス取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:43	T.Aoyama	作成
//------------------------------------------------------------------------------
TCHAR* DialogWindow::GetLastReadFilePath(const size_t _bufferSize, TCHAR* _pOut)
{
	if (_pOut == nullptr)
		return this->read.filePath.data();

	ao_strcpy_s(_pOut, _bufferSize, this->read.filePath.data());
	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		最後に取得した読み込み用のファイル名を取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/09	09:45	T.Aoyama	作成
//------------------------------------------------------------------------------
TCHAR* DialogWindow::GetLastReadFileName(const size_t _bufferSize,TCHAR* _pOut)
{
	if (_pOut == nullptr)
		return this->read.fileName.data();

	ao_strcpy_s(_pOut, _bufferSize, this->read.fileName.data());
	return _pOut;
}