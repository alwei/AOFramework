#include"stdafx.hpp"
#include"LightBloomShader.hpp"
#include"../Function/ShaderFunction.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Define/SystemDefine.hpp"


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


LightBloomShader::LightBloomShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	pUpSampleRenderTarget(new DirectX11RenderTarget(_pd3dDevice,_pd3dDeviceContext)),
	RenderBaseShader(_pd3dDevice,_pd3dDeviceContext)
{
	this->renderCommand.isActive = FALSE;
	this->nullTextures.fill(nullptr);
	this->Naming(_T("LightBloom"));

	for (auto& it : this->pReductionBuffer)
	{
		it = new GaussianFilterShader(_pd3dDevice, _pd3dDeviceContext);
	}
}


LightBloomShader::~LightBloomShader()
{
	for (auto& it : this->pReductionBuffer)
	{
		delete it;
	}

	delete this->pUpSampleRenderTarget;
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	09:17	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT LightBloomShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	HRESULT hr(E_FAIL);
	hr = this->pUpSampleRenderTarget->Initialize();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pSurfaceVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/LightBloom/SurfaceVertexShader.hlsl");
	this->pSurfacePixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/LightBloom/SurfacePixelShader.hlsl");
	
	this->pUpSampleVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/LightBloom/UpSampleVertexShader.hlsl");

	int index = 0;
	for (auto& it : this->pReductionBuffer)
	{
		AOCHAR buffer[32];
		ao_sprintf_s(buffer, 32, _T("GaussianFilter_%d"), index++);
		it->Naming(buffer);

		hr = it->Initialize(this->pGraphicsState, this->pRenderer);
		AO_CHECK_ERROR_RETURN_HRESULT(hr);
	}

	this->psConst.Register(this->pd3dDevice, this->pd3dDeviceContext);

	hr = this->psConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	this->CreateSurface();

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		レンダーターゲット作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	09:17	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT LightBloomShader::Create(
	CONST UINT _nWidth,
	CONST UINT _nHeight,
	CONST BloomBuffer _numBuffer,
	CONST DXGI_FORMAT _format)
{
	HRESULT hr = S_OK;

	//アップサンプル用
	UINT up_w = _nWidth / 4;
	UINT up_h = _nHeight / 4;
	hr = this->pUpSampleRenderTarget->Create(up_w, up_h, _format);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create up sample render target.\n"));

	this->renderCommand.upSampleTexture = this->pUpSampleRenderTarget->GetRenderTexture();

	//縮小バッファ作成
	this->numBuffer = _numBuffer;
	this->psConst->numBuffer = (int)_numBuffer;
	for (int i = 0; i < (int)this->numBuffer; ++i)
	{
		UINT w = _nWidth / (2 << i);
		UINT h = _nHeight / (2 << i);

		hr = this->pReductionBuffer[i]->Create(w, h, _format);
		AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create blur render target.\n"));

		this->renderCommand.pTextureArray[i] = this->pReductionBuffer[i]->GetBlurTexture();
	}

	std::array<char, 16> num;
	sprintf_s(num.data(), 16, "%d", (int)this->numBuffer);
	D3D_SHADER_MACRO macro[2] = { "MAX_BUFFER", num.data(), nullptr, nullptr };
	this->pUpSamplePixelShader = sShader()->LoadPixelShaderFromFile(
		L"hlsl/LightBloom/UpSamplePixelShader.hlsl",
		"main",
		"ps_5_0",
		nullptr,
		macro);

	//アップサンプリング用のオフセット設定
	this->psConst->uvOffset[0] = 1.0f / up_w;
	this->psConst->uvOffset[1] = 1.0f / up_h;

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	09:31	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT LightBloomShader::Render(
	BaseTexture* _pTexture,
	FLOAT _dispersion,
	FLOAT _mulPower)
{
#ifdef __AO_DEBUG__
	if (_pTexture == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[GaussianFilterShader::Render]_pTexture argument is null.\n"));
		return E_INVALIDARG;
	}
#endif

	if (this->renderCommand.isActive)
	{
		return S_FALSE;
	}

	this->renderCommand.isActive = TRUE;
	this->renderCommand.dispersion = _dispersion;
	this->renderCommand.mulPower = _mulPower;

	//down sampling
	for (int i = 0; i < (int)this->numBuffer; ++i)
	{
		if (i == 0)
		{
			this->pReductionBuffer[i]->Render(_pTexture, _dispersion);
		}
		else
		{
			this->pReductionBuffer[i]->Render(this->pReductionBuffer[i - 1]->GetBlurTexture(), _dispersion);
		}
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		プッシュバック時の処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	11:09	T.Aoyama	作成
//------------------------------------------------------------------------------
void LightBloomShader::OnPushbackEnter()
{
	for (int i = 0; i < (int)this->numBuffer; ++i)
	{
		this->pRenderer->PostRenderPushback(this->pReductionBuffer[i]);
	}
}


//------------------------------------------------------------------------------
/**	@brief		描画実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	09:42	T.Aoyama	作成
//------------------------------------------------------------------------------
void LightBloomShader::OnDraw()
{
	if (!this->renderCommand.isActive)
	{
		return;
	}

	auto& it = this->renderCommand;

	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);

	UINT offset = 0;
	UINT stride = sizeof(::SUFVERTEX);
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->pSurfaceBuffer.GetComPtrPtr(), &stride, &offset);

	this->psConst->mulPower = it.mulPower;
	this->pd3dDeviceContext->VSSetShader(this->pUpSampleVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pUpSamplePixelShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pUpSampleVertexShader->layout.GetComPtr());

	this->pUpSampleRenderTarget->SetRenderTarget(TRUE);
	this->pGraphicsState->SetBlendState(BlendState::Add);
	this->pGraphicsState->SetDepthStecilState(FALSE, D3D11_DEPTH_WRITE_MASK_ZERO);

	this->psConst.MapPixel();
	this->pRenderer->SetTexture((int)this->numBuffer, 0, 0, it.pTextureArray.data());
	this->pd3dDeviceContext->Draw(3, 0);
	this->pRenderer->SetTexture((int)this->numBuffer, 0, 0, this->nullTextures.data());

	this->pUpSampleRenderTarget->RestoreRenderTarget();

	//サーフェイス描画
	this->pd3dDeviceContext->VSSetShader(this->pSurfaceVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pSurfacePixelShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pSurfaceVertexShader->layout.GetComPtr());

	this->pRenderer->SetTexture(0, 0, it.upSampleTexture);
	this->pd3dDeviceContext->Draw(3, 0);
	this->pRenderer->SetTexture(0, 0, nullptr, TRUE, TRUE);

	this->pGraphicsState->SetDepthStecilState(TRUE);

	this->renderCommand.isActive = FALSE;
}


//------------------------------------------------------------------------------
/**	@brief		クリア処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	09:30	T.Aoyama	作成
//------------------------------------------------------------------------------
void LightBloomShader::Clear(float _r, float _g, float _b, float _a)
{
	for (int i = 0; i < (int)this->numBuffer; ++i)
	{
		this->pReductionBuffer[i]->Clear(_r, _g, _b, _a);
	}

	this->pUpSampleRenderTarget->Clear();
}


//------------------------------------------------------------------------------
/**	@brief		サーフェイスの作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	09:24	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT LightBloomShader::CreateSurface()
{
	return ::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		this->pSurfaceBuffer.ToCreator(),
		sizeof(surface),
		surface);
}

