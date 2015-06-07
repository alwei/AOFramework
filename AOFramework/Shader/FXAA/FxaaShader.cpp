#include"stdafx.hpp"
#include"FXAAShader.hpp"
#include"../Function/ShaderFunction.hpp"
#include"../../Define/SystemDefine.hpp"
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


FXAAShader::FXAAShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	RenderBaseShader(_pd3dDevice,_pd3dDeviceContext)
{
	this->Naming(_T("FXAA"));
}


FXAAShader::~FXAAShader()
{

}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	09:17	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT FXAAShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	HRESULT hr = this->CreateSurface();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		レンダーターゲット作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	09:17	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT FXAAShader::Create(DirectX11RenderTarget* const _pRenderTarget)
{
	HRESULT hr = S_OK;
	D3D11_VIEWPORT viewport;
	_pRenderTarget->GetViewport(&viewport);
	std::array<char, 16> AdjustX, AdjustY;
	sprintf_s(AdjustX.data(), 16, "%f", 0.5f / viewport.Width);
	sprintf_s(AdjustY.data(), 16, "%f", 0.5f / viewport.Height);
	D3D_SHADER_MACRO vs_macros[] = { "ADJUST_X", AdjustX.data(), "ADJUST_Y", AdjustY.data(), nullptr, nullptr };

	this->pVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/FXAA/FXAAVertexShader.hlsl", "main", "vs_5_0", nullptr, vs_macros);

	std::array<char, 16> _1TexelU, _1TexelV;
	sprintf_s(_1TexelU.data(), 16, "%f", 1.0f / viewport.Width);
	sprintf_s(_1TexelV.data(), 16, "%f", 1.0f / viewport.Height);
	D3D_SHADER_MACRO ps_macros[] = { "_1TEXEL_U", _1TexelU.data(), "_1TEXEL_V", _1TexelV.data(), nullptr, nullptr };

	this->pPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/FXAA/FXAAPixelShader.hlsl", "main", "ps_5_0", nullptr, ps_macros);

	this->renderCommand.pRenderTarget = _pRenderTarget;

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	09:31	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT FXAAShader::Render(BaseTexture* _pTexture)
{
#ifdef __AO_DEBUG__
	if (_pTexture == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[FXAAShader::Render]_pTexture argument is null.\n"));
		return E_INVALIDARG;
	}
#endif

	this->renderCommand.renderTexture = _pTexture;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		プッシュバック時の処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	11:09	T.Aoyama	作成
//------------------------------------------------------------------------------
void FXAAShader::OnPushbackEnter()
{

}


//------------------------------------------------------------------------------
/**	@brief		描画実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	09:42	T.Aoyama	作成
//------------------------------------------------------------------------------
void FXAAShader::OnDraw()
{
	auto& it = this->renderCommand;

	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);

	UINT offset = 0;
	UINT stride = sizeof(::SUFVERTEX);
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->pSurfaceBuffer.GetComPtrPtr(), &stride, &offset);
	this->pd3dDeviceContext->IASetInputLayout(this->pVertexShader->layout.GetComPtr());
	this->pGraphicsState->SetBlendState(ao::BlendState::Default);
	this->pGraphicsState->SetDepthStecilState(FALSE);

	//サーフェイス描画
	this->pd3dDeviceContext->VSSetShader(this->pVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pPixelShader->shader.GetComPtr(), nullptr, 0);

	it.pRenderTarget->SetRenderTarget(TRUE);

	this->pRenderer->SetTexture(0, 0, it.renderTexture);
	this->pd3dDeviceContext->Draw(3, 0);
	this->pRenderer->SetTexture(0, 0, nullptr, TRUE, TRUE);

	this->pGraphicsState->SetDepthStecilState(TRUE);
}


//------------------------------------------------------------------------------
/**	@brief		サーフェイスの作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	09:24	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT FXAAShader::CreateSurface()
{
	return ::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		this->pSurfaceBuffer.ToCreator(),
		sizeof(surface),
		surface);
}