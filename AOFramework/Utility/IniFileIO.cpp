#include"stdafx.hpp"
#include"IniFileIO.hpp"


using namespace ao;


IniFileIO::IniFileIO()
{
	::SecureZeroMemory(this->buffer.data(), sizeof(this->buffer));
	::SecureZeroMemory(this->filePath.data(), sizeof(this->filePath));
	::GetCurrentDirectory(Buffer::Path, this->filePath.data());
}


IniFileIO::IniFileIO(const ao::string& _filePath)
{
	int test = sizeof(this->buffer);
	::SecureZeroMemory(this->buffer.data(), sizeof(this->buffer));
	::SecureZeroMemory(this->filePath.data(), sizeof(this->filePath));
	::GetCurrentDirectory(Buffer::Path, this->filePath.data());

	ao_sprintf_s(
		this->filePath.data(),
		this->filePath.size(),
		_T("%s//%s") ,
		this->filePath.data(),
		_filePath.c_str());
}


IniFileIO::~IniFileIO()
{

}


//------------------------------------------------------------------------------
/**	@brief		ファイルオープン */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/16	10:13	T.Aoyama	作成
//------------------------------------------------------------------------------
int IniFileIO::Open(const ao::string& _filePath)
{
	return ao_sprintf_s(
		this->filePath.data(),
		sizeof(this->filePath),
		_T("%s//%s"),
		this->filePath.data(),
		_filePath.c_str());
}


//------------------------------------------------------------------------------
/**	@brief		指定セクションの情報取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/16	09:40	T.Aoyama	作成
//------------------------------------------------------------------------------
const float IniFileIO::GetFloat(
	const ao::string& _sectionName,
	const ao::string& _keyName,
	const float _defaultParam)
{
	::GetPrivateProfileString(
		_sectionName.c_str(),
		_keyName.c_str(),
		ao::to_string(_defaultParam).c_str(),
		this->buffer.data(),
		sizeof(this->buffer),
		this->filePath.data());

	return static_cast<float>(ao_to_f(this->buffer.data()));
}


//------------------------------------------------------------------------------
/**	@brief		指定セクションの情報取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/16	10:01	T.Aoyama	作成
//------------------------------------------------------------------------------
const int IniFileIO::GetInt(
	const ao::string& _sectionName,
	const ao::string& _keyName,
	const int _defaultParam)
{
	GetPrivateProfileString(
		_sectionName.c_str(),
		_keyName.c_str(),
		ao::to_string(_defaultParam).c_str(),
		this->buffer.data(),
		sizeof(this->buffer),
		this->filePath.data());

	return ao_to_i(this->filePath.data());
}


//------------------------------------------------------------------------------
/**	@brief		指定セクションの情報取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/16	10:06	T.Aoyama	作成
//------------------------------------------------------------------------------
const AOCHAR* IniFileIO::GetString(
	AOCHAR* _pOut,
	const size_t _size,
	const ao::string& _sectionName,
	const ao::string& _keyName,
	const ao::string& _defaultParam)
{
	GetPrivateProfileString(
		_sectionName.c_str(),
		_keyName.c_str(),
		_defaultParam.c_str(),
		this->buffer.data(),
		sizeof(this->buffer),
		this->filePath.data());

	if (_size >= this->buffer.size())
		return nullptr;

	if (_pOut == nullptr)
		return this->buffer.data();

	::CopyMemory(_pOut, this->buffer.data(), _size);

	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		指定セクションの情報取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/16	10:25	T.Aoyama	作成
//------------------------------------------------------------------------------
const BOOL IniFileIO::GetBool(
	const ao::string& _sectionName,
	const ao::string& _keyName,
	const BOOL _defaultParam)
{
	ao::string defaultString(_T("FALSE"));
	if (_defaultParam == TRUE)
		defaultString.assign(_T("TRUE"));

	GetPrivateProfileString(
		_sectionName.c_str(),
		_keyName.c_str(),
		defaultString.c_str(),
		this->buffer.data(),
		sizeof(this->buffer),
		this->filePath.data());

	ao::string temp(this->buffer.data());
	if (temp == _T("TRUE"))
		return TRUE;

	return FALSE;
}