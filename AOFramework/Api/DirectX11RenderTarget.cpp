#include"stdafx.hpp"
#include"DirectX11RenderTarget.hpp"
#include"DirectXTex\DirectXTex.hpp"



using namespace ao;


DirectX11RenderTarget::DirectX11RenderTarget(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	pLogDeviceContext(nullptr),
	DirectX11BaseApi(_pd3dDevice, _pd3dDeviceContext)
{

}


DirectX11RenderTarget::~DirectX11RenderTarget()
{

}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/06/27	22:41	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11RenderTarget::Initialize()
{
	//DirectX11RenderTarget専用の初期化処理（なにかあれば）
	this->CreateDefaultSamplerState();

	//レンダーターゲットのテクスチャ設定
	this->rtTexture.fileName.assign(_T("RenderTarget"));
	this->rtTexture.filePath.assign(_T("not path"));
	this->rtTexture.pSampState = this->renderSamplerState[(int)SamplerState::LINEAR];

	//深度ステンシルのテクスチャ設定
	this->dsTexture.fileName.assign(_T("DepthStencil"));
	this->dsTexture.filePath.assign(_T("not path"));
	this->dsTexture.pSampState = this->renderSamplerState[(int)SamplerState::LINEAR];

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/06/27	22:27	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11RenderTarget::Create(
	CONST UINT _nWidth,
	CONST UINT _nHeight,
	CONST DXGI_FORMAT _format,
	CONST UINT _miscFlag,
	CONST UINT _mipLevel,
	CONST BOOL _isCreateDepthStencil,
	CONST DXGI_FORMAT _depthFormat)
{
	HRESULT hr(E_FAIL);

#ifdef __AO_DEBUG__
	if (this->pd3dDevice == nullptr || this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11RenderTarget::Create]\npd3dDevice or pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}
#endif

	//レンダーターゲット作成
	hr = this->CreateRenderTargetView(_nWidth, _nHeight, _format, _miscFlag, _mipLevel);

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create render target view.\n\n"));

	this->rtTexture.pSrView = this->pRtSrView;

	//深度ステンシルの作成
	if (_isCreateDepthStencil == FALSE)
	{
		return hr;
	}

	hr = this->CreateDepthStencilView(_nWidth, _nHeight, _depthFormat);

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create depth stencil view.\n\n"));

	this->dsTexture.pSrView = this->pDsSrView;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		レンダーターゲットの設定 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/23	09:48	T.Aoyama	作成
//------------------------------------------------------------------------------
void	DirectX11RenderTarget::SetRenderTarget(
	BOOL _isLogRestore,
	BOOL _isSetDepthStencil,
	ID3D11DeviceContext* _pd3dDeviceContext)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11RenderTarget::SetRenderTarget]\npd3dDevice or pd3dDeviceContext is null.\n"));
		return;
	}
#endif

	UINT num = 1;
	ID3D11DepthStencilView* pSetDs = (_isSetDepthStencil) ? this->pDsView.GetComPtr() : nullptr;
	ID3D11RenderTargetView* setRtView[] = { this->pRtView.GetComPtr() };

	if (_isLogRestore)
	{
		if (_pd3dDeviceContext)
		{
			this->pLogDeviceContext = _pd3dDeviceContext;
			this->pLogDeviceContext->RSGetViewports(&num, &this->logViewport);
			this->pLogDeviceContext->RSSetViewports(1, &this->viewport);
			this->pLogDeviceContext->OMGetRenderTargets(1, this->pLogRtView.ToCreator(), this->pLogDsView.ToCreator());
			this->pLogDeviceContext->OMSetRenderTargets(1, setRtView, this->pDsView.GetComPtr());
		}
		else
		{
			this->pd3dDeviceContext->RSGetViewports(&num, &this->logViewport);
			this->pd3dDeviceContext->RSSetViewports(1, &this->viewport);
			this->pd3dDeviceContext->OMGetRenderTargets(1, this->pLogRtView.ToCreator(), this->pLogDsView.ToCreator());
			this->pd3dDeviceContext->OMSetRenderTargets(1, setRtView, this->pDsView.GetComPtr());
		}
	}
}


//------------------------------------------------------------------------------
/**	@brief		レンダーターゲットのリストア */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/23	09:53	T.Aoyama	作成
//------------------------------------------------------------------------------
void DirectX11RenderTarget::RestoreRenderTarget()
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11RenderTarget::RestoreRenderTarget]\npd3dDevice or pd3dDeviceContext is null.\n"));
		return;
	}
#endif

	ID3D11RenderTargetView* setRtView[] = { this->pLogRtView.GetComPtr() };

	if (this->pLogDeviceContext)
	{
		this->pLogDeviceContext->RSSetViewports(1, &this->logViewport);
		this->pLogDeviceContext->OMSetRenderTargets(1, setRtView, this->pLogDsView.GetComPtr());
		this->pLogDeviceContext = nullptr;
	}
	else
	{
		this->pd3dDeviceContext->RSSetViewports(1, &this->logViewport);
		this->pd3dDeviceContext->OMSetRenderTargets(1, setRtView, this->pLogDsView.GetComPtr());
	}
}


//------------------------------------------------------------------------------
/**	@brief		クリア */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/11	01:38	T.Aoyama	作成
//------------------------------------------------------------------------------
void DirectX11RenderTarget::Clear(float _r, float _g, float _b, float _a)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11RenderTarget::Clear]\npd3dDevice or pd3dDeviceContext is null.\n"));
		return;
	}
#endif

	const float clearColor[] = { _r, _g, _b, _a };
	if (this->pRtView != nullptr)
	{
		this->pd3dDeviceContext->ClearRenderTargetView(this->pRtView.GetComPtr(), clearColor);
	}

	if (this->pDsView != nullptr)
	{
		this->pd3dDeviceContext->ClearDepthStencilView(this->pDsView.GetComPtr(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}



//------------------------------------------------------------------------------
/**	@brief		レンダーターゲット内容を外部ファイルとして出力 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/24	22:43	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11RenderTarget::OutputRenderTarget(AOLPCSTR _filePath,const ImageFormat _format)
{
	GUID guid;

	switch (_format)
	{
	case ImageFormat::BMP:
		guid = GUID_ContainerFormatBmp;
		break;
	case ImageFormat::JPG:
		guid = GUID_ContainerFormatJpeg;
		break;
	case ImageFormat::PNG:
		guid = GUID_ContainerFormatPng;
		break;
	}

	DirectX::ScratchImage sImage;
	DirectX::CaptureTexture(this->pd3dDevice.GetComPtr(), this->pd3dDeviceContext.GetComPtr(), this->pRtTex.GetComPtr(), sImage);

	const DirectX::Image* pImage = sImage.GetImage(0, 0, 0);
	HRESULT hr = DirectX::SaveToWICFile(*pImage, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, guid, _filePath);

	//エラーチェック
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Faield out put render target image.\n"));

	return hr;
}



//------------------------------------------------------------------------------
/**	@brief		ビューポートの取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/03	16:07	T.Aoyama	作成
//------------------------------------------------------------------------------
const D3D11_VIEWPORT* DirectX11RenderTarget::GetViewport(D3D11_VIEWPORT* const _pOut)
{
	if (_pOut == nullptr)
	{
		return &this->viewport;
	}

	::CopyMemory(_pOut, &this->viewport, sizeof(D3D11_VIEWPORT));

	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		レンダーターゲットビュー作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/11	01:36	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT DirectX11RenderTarget::CreateRenderTargetView(
	CONST UINT _nWidth,
	CONST UINT _nHeight,
	CONST DXGI_FORMAT _format,
	CONST UINT _miscFlag,
	CONST UINT _mipLevel)
{
	HRESULT hr(S_FALSE);

	D3D11_TEXTURE2D_DESC descRT;
	D3D11_RENDER_TARGET_VIEW_DESC	descRTV;

	descRT.Width = _nWidth;
	descRT.Height = _nHeight;
	descRT.MipLevels = _mipLevel;
	descRT.ArraySize = 1;
	descRT.Format = _format;
	descRT.SampleDesc.Count = 1;
	descRT.SampleDesc.Quality = 0;
	descRT.Usage = D3D11_USAGE_DEFAULT;
	descRT.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	descRT.CPUAccessFlags = FALSE;
	descRT.MiscFlags = _miscFlag;

	::SecureZeroMemory(&descRTV, sizeof(descRTV));

	descRTV.Format = descRT.Format;
	descRTV.Texture2D.MipSlice = 0;

	// マルチサンプルを使用していないと仮定
	descRTV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	//テクスチャ作成
	hr = this->pd3dDevice->CreateTexture2D(&descRT, nullptr, this->pRtTex.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create texture 2D.\n"));

	//シェーダーリソースビュー作成
	hr = this->pd3dDevice->CreateShaderResourceView(this->pRtTex.GetComPtr(), nullptr, this->pRtSrView.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create shader resource view.\n"));


	// レンダリングターゲットビューの作成
	hr = this->pd3dDevice->CreateRenderTargetView(this->pRtTex.GetComPtr(), &descRTV, this->pRtView.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create render target view.\n"));


	// ビューポートの設定
	::SecureZeroMemory(&this->viewport, sizeof(D3D11_VIEWPORT));
	this->viewport.Width = (float)_nWidth;
	this->viewport.Height = (float)_nHeight;
	this->viewport.MinDepth = 0.0f;
	this->viewport.MaxDepth = 1.0f;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		深度ステンシル作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/11	01:36	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT DirectX11RenderTarget::CreateDepthStencilView(
	CONST UINT _nWidth,
	CONST UINT _nHeight,
	CONST DXGI_FORMAT _format)
{
	HRESULT hr = S_FALSE;

	D3D11_TEXTURE2D_DESC descDS;
	D3D11_DEPTH_STENCIL_VIEW_DESC	descDSV;
	D3D11_SHADER_RESOURCE_VIEW_DESC	descSRV;


	descDS.Width = _nWidth;
	descDS.Height = _nHeight;
	descDS.MipLevels = 1;
	descDS.ArraySize = 1;
	descDS.Format = _format;
	descDS.SampleDesc.Count = 1;
	descDS.SampleDesc.Quality = 0;
	descDS.Usage = D3D11_USAGE_DEFAULT;
	descDS.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDS.CPUAccessFlags = FALSE;
	descDS.MiscFlags = FALSE;

	::SecureZeroMemory(&descDSV, sizeof(descDSV));
	::SecureZeroMemory(&descSRV, sizeof(descSRV));

	switch (descDS.Format)
	{
	case DXGI_FORMAT_R16_TYPELESS:
		descDSV.Format = DXGI_FORMAT_D16_UNORM;
		descSRV.Format = DXGI_FORMAT_R16_UNORM;
		break;
	case DXGI_FORMAT_R32_TYPELESS:
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descSRV.Format = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_R24G8_TYPELESS:
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descSRV.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	default:
		descDSV.Format = descDS.Format;
	}


	descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MipLevels = 1;

	// マルチサンプルを使用していないと仮定
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	//テクスチャ作成
	hr = this->pd3dDevice->CreateTexture2D(&descDS, nullptr, this->pDsTex.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create texture 2D.\n"));


	//シェーダーリソースビュー作成
	hr = this->pd3dDevice->CreateShaderResourceView(this->pDsTex.GetComPtr(), &descSRV, this->pDsSrView.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create shader resource view.\n"));


	// 深度ステンシルビューの作成
	hr = this->pd3dDevice->CreateDepthStencilView(this->pDsTex.GetComPtr(), &descDSV, this->pDsView.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create depth stencil view.\n"));


	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		デフォルトのサンプラーステート作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/03	19:33	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT DirectX11RenderTarget::CreateDefaultSamplerState()
{
	HRESULT hr;
	D3D11_SAMPLER_DESC desc;

	//テクスチャのサンプラステートを設定
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 ～ 1 の範囲外にある u テクスチャー座標の描画方法
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 ～ 1 の範囲外にある v テクスチャー座標
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 ～ 1 の範囲外にある w テクスチャー座標
	desc.MipLODBias = 0;								// 計算されたミップマップ レベルからのバイアス
	desc.MaxAnisotropy = 16;							// サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 。
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;		// 比較オプション。
	// 境界色をバーストコピー
	::SecureZeroMemory(desc.BorderColor, sizeof(float) * 4);
	//ミップマップの設定
	desc.MinLOD = 0;					// アクセス可能なミップマップの下限値
	desc.MaxLOD = D3D11_FLOAT32_MAX;	// アクセス可能なミップマップの上限値

	//サンプラーステート作成
	auto& samplerState = this->renderSamplerState;

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = this->pd3dDevice->CreateSamplerState(&desc, samplerState[(int)SamplerState::LINEAR].ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create sampler state.[LINEAR]\n"));


	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = this->pd3dDevice->CreateSamplerState(&desc, samplerState[(int)SamplerState::POINT].ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create sampler state.[POINT]\n"));

	return hr;
}