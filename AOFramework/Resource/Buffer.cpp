#include"stdafx.hpp"
#include"Buffer.hpp"


using namespace ao;


Buffer::Buffer()
{

}


Buffer::~Buffer()
{

}


//------------------------------------------------------------------------------
/**	@brief		バッファ生成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/21	20:52	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT Buffer::Create(
	const void* _initSrc,
	const UINT _dataSize,
	const D3D11_USAGE _usage,
	const UINT _cpuAccessFlag,
	SmartComPtr<ID3D11Device> _pd3dDevice)
{
	D3D11_BUFFER_DESC BufferDesc;
	D3D11_SUBRESOURCE_DATA Resource;

	// 初期値を設定する
	Resource.pSysMem = _initSrc;
	Resource.SysMemPitch = 0;
	Resource.SysMemSlicePitch = 0;

	// バッファの設定
	::SecureZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.ByteWidth = _dataSize;						// バッファサイズ
	BufferDesc.Usage = _usage;								// リソース使用法を特定する
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// バッファの種類
	BufferDesc.CPUAccessFlags = _cpuAccessFlag;				// CPU アクセス
	BufferDesc.MiscFlags = 0;								// その他のフラグも設定しない

	// バッファを作成する
	return _pd3dDevice->CreateBuffer(&BufferDesc, &Resource, this->buffer.ToCreator());
}


//------------------------------------------------------------------------------
/**	@brief		マップ処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/21	21:01	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT Buffer::Map(
	const void* _mapSrc,
	const UINT _dataSize,
	const D3D11_MAP _mapFlag,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext)
{
	HRESULT hr = E_FAIL;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	hr = _pd3dDeviceContext->Map(this->buffer.GetComPtr(), 0, _mapFlag, 0, &mappedResource);
	if (FAILED(hr))
		return hr;

	::CopyMemory(mappedResource.pData, _mapSrc, _dataSize);
	_pd3dDeviceContext->Unmap(this->buffer.GetComPtr(), 0);

	return hr;
}