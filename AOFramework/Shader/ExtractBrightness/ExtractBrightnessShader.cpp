#include"stdafx.hpp"
#include"ExtractBrightnessShader.hpp"
#include"../Function/ShaderFunction.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Api/DirectX11Shader.hpp"


using namespace ao;

namespace
{
	struct SUFVERTEX
	{
		float pos[3];
		float uv[2];
	};

	static 	SUFVERTEX	surface[3] =
	{
		{ -1.f, 1.f, 0.f, 0.f, 0.f },
		{ 3.f, 1.f, 0.f, 2.f, 0.f },
		{ -1.f, -3.f, 0.f, 0.f, 2.f }
	};
}


ExtractBrightnessShader::ExtractBrightnessShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) : 
	pExtRenderTarget(new DirectX11RenderTarget(_pd3dDevice,_pd3dDeviceContext)),
	RenderBaseShader(_pd3dDevice,_pd3dDeviceContext)
{

}


ExtractBrightnessShader::~ExtractBrightnessShader()
{
	delete this->pExtRenderTarget;
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	01:22	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT ExtractBrightnessShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	this->pVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/ExtractBrightness/ExtractBrightnessVertexShader.hlsl");
	this->pPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/ExtractBrightness/ExtractBrightnessPixelShader.hlsl");

	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;


	HRESULT hr(E_FAIL);

	hr = this->pExtRenderTarget->Initialize();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed initialize extract brightness shader.\n"));


	hr = this->CreateSurface();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create surface vertex buffer.\n"));


	this->psConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	hr = this->psConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create constant buffer.\n"));


	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		抽出用のサーフェイス作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	01:22	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT ExtractBrightnessShader::Create(
	CONST UINT _nWidth,
	CONST UINT _nHeight,
	CONST DXGI_FORMAT _format)
{
	HRESULT hr = this->pExtRenderTarget->Create(_nWidth, _nHeight, _format,0,1,TRUE,DXGI_FORMAT_R16_TYPELESS);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("[ExtractBrightnessShader::Create]Failed create extract render target.\n"));


	ID3D11RenderTargetView* pGetRtView[1];
	ID3D11DepthStencilView* pGetDsView[1];
	this->pd3dDeviceContext->OMGetRenderTargets(1, pGetRtView, pGetDsView);

	//バックバッファ抽出
	SmartComPtr<ID3D11Resource> pResource;
	pGetRtView[0]->GetResource(pResource.ToCreator());
	this->pd3dDevice->CreateShaderResourceView(
		pResource.GetComPtr(),
		nullptr,
		this->pBackBufferSrView.ToCreator());

	pGetRtView[0]->Release();
	pGetDsView[0]->Release();

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		輝度抽出 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	01:22	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT ExtractBrightnessShader::Render(
	BaseTexture* _pTexture,
	const float _r,
	const float _g,
	const float _b,
	const float _a)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[ExtractBrightnessShader::Render]pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}
#endif

	if (!this->commandList.empty())
	{
		return S_FALSE;
	}

	COMMAND pushCommand;

	pushCommand.sub[0] = _r;
	pushCommand.sub[1] = _g;
	pushCommand.sub[2] = _b;
	pushCommand.sub[3] = _a;
	pushCommand.ppSrView = _pTexture->pSrView.GetComPtrPtr();
	pushCommand.ppSampState = _pTexture->pSampState.GetComPtrPtr();

	// stores rendering command exclusively
	this->commandList.push_back(pushCommand);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		輝度抽出 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	01:22	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT ExtractBrightnessShader::Render(
	const float _r,
	const float _g,
	const float _b,
	const float _a)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[ExtractBrightnessShader::Render]pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}
#endif

	if (!this->commandList.empty())
		return S_FALSE;

	COMMAND pushCommand;

	pushCommand.sub[0] = _r;
	pushCommand.sub[1] = _g;
	pushCommand.sub[2] = _b;
	pushCommand.sub[3] = _a;
	pushCommand.ppSrView = this->pBackBufferSrView.GetComPtrPtr();
	pushCommand.ppSampState = nullptr;

	// stores rendering command exclusively
	this->commandList.push_back(pushCommand);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		描画実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	02:01	T.Aoyama	作成
//------------------------------------------------------------------------------
void ExtractBrightnessShader::OnDraw()
{
	if (this->commandList.empty())
	{
		return;
	}

	this->pd3dDeviceContext->VSSetShader(this->pVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pPixelShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pVertexShader->layout.GetComPtr());
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);

	UINT offset = 0;
	UINT stride = sizeof(::SUFVERTEX);
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->pSurfaceBuffer.GetComPtrPtr(), &stride, &offset);

	this->pGraphicsState->SetBlendState(BlendState::Default);
	this->pGraphicsState->SetDepthStecilState(FALSE,D3D11_DEPTH_WRITE_MASK_ZERO);


	auto& it = this->commandList.cbegin();
	this->pExtRenderTarget->SetRenderTarget(TRUE);
	this->pRenderer->SetTexture(0, 0, it->ppSrView,it->ppSampState, TRUE);
	::CopyMemory(this->psConst->sub, it->sub, sizeof(float) * 4);
	this->psConst.MapPixel(0);
	this->pd3dDeviceContext->Draw(3, 0);
	this->pRenderer->SetTexture(0, 0, nullptr, TRUE, TRUE);
	this->pExtRenderTarget->RestoreRenderTarget();

	this->commandList.clear();

	this->pGraphicsState->SetDepthStecilState(TRUE);
}


//------------------------------------------------------------------------------
/**	@brief		抽出したレンダーターゲットのクリア処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	11:44	T.Aoyama	作成
//------------------------------------------------------------------------------
void ExtractBrightnessShader::Clear(
	const float _r,
	const float _g,
	const float _b,
	const float _a)
{
	this->pExtRenderTarget->Clear(_r, _g, _b, _a);
}


//------------------------------------------------------------------------------
/**	@brief		サーフェイスの作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	02:06	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT ExtractBrightnessShader::CreateSurface()
{
	return ::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		this->pSurfaceBuffer.ToCreator(),
		sizeof(surface),
		surface);
}