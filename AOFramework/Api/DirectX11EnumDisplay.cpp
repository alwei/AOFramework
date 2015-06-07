#include"stdafx.hpp"
#include"DirectX11EnumDisplay.hpp"
#include"../Debug/DebugSystem.hpp"
#include"../Utility/SmartComPtr.hpp"

using namespace ao;


DirectX11EnumDisplay::DirectX11EnumDisplay()
{

}


DirectX11EnumDisplay::~DirectX11EnumDisplay()
{

}


//------------------------------------------------------------------------------
/**	@brief		ディスプレイ列挙 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/06/27	21:20	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11EnumDisplay::EnumDisplay(
	IDXGIAdapter* _pAdapter,
	DXGI_FORMAT _getFormat
	)
{
	HRESULT	hr = E_FAIL;
	DXGI_MODE_DESC* pDesc = nullptr;
	SmartComPtr<IDXGIOutput> pOutPut;

	// アダプターに接続されているプライマリディスプレイを取得。
	hr = _pAdapter->EnumOutputs(0, pOutPut.ToCreator());
#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11EnumDisplay::EnumDisplay]\nFailed get primary display.\n"));
		return E_FAIL;
	}

	ao::debug::PrintfColor(ConsoleColor::H_GREEN, _T("[DirectX11EnumDisplay::EnumDisplay]\nSuccedded get primary display.\n"));
#endif


	UINT num;
	// ディスプレイモード一覧の配列数を取得
	hr = pOutPut->GetDisplayModeList(_getFormat, 0, &num, nullptr);
#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11EnumDisplay::EnumDisplay]\nFailed get display mode list of array number.\n"));
		return E_FAIL;
	}

	ao::debug::PrintfColor(ConsoleColor::H_GREEN, _T("[DirectX11EnumDisplay::EnumDisplay]\nSuccedded get display mode list of array number.\n"));
#endif


	pDesc = new DXGI_MODE_DESC[num];
	// ディスプレイモードの一覧を取得
	hr = pOutPut->GetDisplayModeList(_getFormat, 0, &num, pDesc);
#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		delete[] pDesc;
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11EnumDisplay::EnumDisplay]\nFailed get display mode list.\n\n"));
		return E_FAIL;
	}

	ao::debug::PrintfColor(ConsoleColor::H_GREEN, _T("[DirectX11EnumDisplay::EnumDisplay]\nSuccedded get display mode list.\n\n"));
#endif

	for (UINT i = 0; i<num; i++)
	{
		this->dxGiDescVector.push_back(pDesc[i]);
		CONST INT _nWidth = this->dxGiDescVector[i].Width;
		CONST INT _nHeight = this->dxGiDescVector[i].Height;
		CONST INT denominator = this->dxGiDescVector[i].RefreshRate.Denominator;
		CONST INT numerator = this->dxGiDescVector[i].RefreshRate.Numerator;
		ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Resolution[ %4d : %4d ] Refresh Rate[ %d / %d ]\n"), _nWidth, _nHeight, denominator, numerator);
	}

	delete[] pDesc;
	this->bEnumDisplayMode = TRUE;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		列挙ディスプレイ取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/06/27	00:00	T.Aoyama	作成
//------------------------------------------------------------------------------
DXGI_MODE_DESC* DirectX11EnumDisplay::GetEnumDisplayDesc(
	DXGI_MODE_DESC* _pOut,
	const UINT _nWidth,
	const UINT _nHeight
	)
{
	if (!this->bEnumDisplayMode){
		ao::debug::PrintfColor(ConsoleColor::H_YELLOW, _T("[DirectX11EnumDisplay::GetEnumDisplayDesc]It is not enumerate yet.\n"));
		return nullptr;
	}

	auto& vector = this->dxGiDescVector;
	for (auto& it = vector.rbegin(); it != vector.rend(); ++it)
	{
		if (it->Width == _nWidth && it->Height == _nHeight)
		{
			::CopyMemory(_pOut, &(*it), sizeof(DXGI_MODE_DESC));
			return _pOut;
		}
	}

	ao::debug::PrintfColor(ConsoleColor::H_YELLOW, _T("[DirectX11EnumDisplay::GetEnumDisplayDesc]Can not found display desc.\n"));

	return nullptr;
}


//------------------------------------------------------------------------------
/**	@brief		列挙したディスプレイ数を取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/06/27	22:02	T.Aoyama	作成
//------------------------------------------------------------------------------
const size_t DirectX11EnumDisplay::GetNumEnumDisplay()
{
	if (!this->bEnumDisplayMode){
		ao::debug::PrintfColor(ConsoleColor::H_YELLOW, _T("[DirectX11EnumDisplay::GetNumEnumDisplay]It is not enumerate yet.\n"));
		return 0;
	}

	return this->dxGiDescVector.size();
}