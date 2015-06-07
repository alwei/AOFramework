#include"stdafx.hpp"
#include"CrossFilterShader.hpp"
#include"../Function/ShaderFunction.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Define/SystemDefine.hpp"
#include"../../Math/Math.hpp"
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


CrossFilterShader::CrossFilterShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) : 
	pUpSampleRenderTarget(new DirectX11RenderTarget(_pd3dDevice,_pd3dDeviceContext)),
	RenderBaseShader(_pd3dDevice,_pd3dDeviceContext)
{
	for (auto& it : this->pStretchBuffer)
	{
		it = new DirectX11RenderTarget(_pd3dDevice, _pd3dDeviceContext);
	}

	for (auto& it : this->pReductionBuffer)
	{
		it = new DirectX11RenderTarget(_pd3dDevice, _pd3dDeviceContext);
	}
}



CrossFilterShader::~CrossFilterShader()
{
	for (auto& it : this->pStretchBuffer)
	{
		delete it;
	}

	for (auto& it : this->pReductionBuffer)
	{
		delete it;
	}

	delete pUpSampleRenderTarget;
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/15	02:31	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT CrossFilterShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pGlareVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/CrossFilter/GlareVertexShader.hlsl");
	this->pGlarePixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/CrossFilter/GlarePixelShader.hlsl");

	this->pUpSampleVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/CrossFilter/UpSampleVertexShader.hlsl");

	this->pSurfaceVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/CrossFilter/SurfaceVertexShader.hlsl");
	this->pSurfacePixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/CrossFilter/SurfacePixelShader.hlsl");


	HRESULT hr(E_FAIL);
	for (auto& it : this->pStretchBuffer)
	{
		hr = it->Initialize();
		AO_CHECK_ERROR_RETURN_HRESULT(hr);
	}

	for (auto& it : this->pReductionBuffer)
	{
		hr = it->Initialize();
		AO_CHECK_ERROR_RETURN_HRESULT(hr);
	}

	//アップサンプリング用の初期化
	this->pUpSampleRenderTarget->Initialize();

	this->vsConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psUpSampleConst.Register(this->pd3dDevice, this->pd3dDeviceContext);

	hr = this->vsConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->psConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->psUpSampleConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);


	this->CreateSurface();

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/15	03:10	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT CrossFilterShader::Create(
	CONST UINT _nWidth,
	CONST UINT _nHeight,
	CONST CrossType _crossType,
	CONST CrossQuality _numStretch,
	CONST DXGI_FORMAT _format)
{
	HRESULT hr = E_FAIL;

	UINT w = _nWidth / 4;
	UINT h = _nHeight / 4;

	//グレアタイプ
	this->crossType = _crossType;

	//アップサンプル用
	UINT up_w = w * 2;
	UINT up_h = h * 2;
	this->pUpSampleRenderTarget->Create(up_w, up_h, _format);
	this->psUpSampleConst->uvOffset[0] = 1.0f / up_w;
	this->psUpSampleConst->uvOffset[1] = 1.0f / up_h;
	this->psUpSampleConst->numStar = (int)this->crossType;

	for (int i = 0; i < (int)this->crossType; ++i)
	{
		hr = this->pReductionBuffer[i]->Create(w, h, _format);
		AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("[CrossFilterShader::Create]Failed create reduction render target.\n"));

		this->renderCommand.pGlareTextureArray[i] = this->pReductionBuffer[i]->GetRenderTexture();
	}

	this->numStretch = _numStretch;
	for (int i = 0; i < (int)this->numStretch; ++i)
	{
		hr = this->pStretchBuffer[i]->Create(w, h, _format);
		AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr,_T("[CrossFilterShader::Create]Failed create stretch render target.\n"));

		this->renderCommand.pTextureArray[i + 1] = this->pStretchBuffer[i]->GetRenderTexture();
	}

	this->sourceSize.x = (float)_nWidth;
	this->sourceSize.y = (float)_nHeight;

	this->downSize.x = (float)w;
	this->downSize.y = (float)h;

	//色調作成
	this->CreateGradationColor();
	//色の重み定数を作成
	this->CreateColorWeight();


	//マクロ定義でピクセルシェーダー作成
	std::array<char, 16> num;
	sprintf_s(num.data(), 16, "%d", (int)this->crossType);
	D3D_SHADER_MACRO macro[2] = { "MAX_BUFFER", num.data(), nullptr, nullptr };
	this->pUpSamplePixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/CrossFilter/UpSamplePixelShader.hlsl", "main", "ps_5_0", nullptr, macro);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/15	04:57	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT CrossFilterShader::Render(
	BaseTexture* _pTexture,
	int _degree,
	float _mulPower,
	float _length)
{
	HRESULT hr = E_FAIL;
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[CrossFilterShader::Render]pd3dDeviceContext is null.\n"));
		return hr;
	}
#endif

	float2 tempStarAngle;

	auto CalcProcessUV = [&](int idxI, int idxJ, int idxK)
	{
		this->renderCommand.uvTexel[idxI][idxJ][idxK].u = tempStarAngle.x * (float)idxK*_length;
		this->renderCommand.uvTexel[idxI][idxJ][idxK].v = tempStarAngle.y * (float)idxK*_length;
	};

	auto CalcSample = [&](int idxI, int idxJ)
	{
		for (int k = 0; k < this->NUM_SAMPLING; ++k)
			CalcProcessUV(idxI, idxJ, k);
		tempStarAngle.x *= NUM_SAMPLING;
		tempStarAngle.y *= NUM_SAMPLING;
	};

	auto CalcStretch = [&](int idxI)
	{
		::CopyMemory(&tempStarAngle, &this->starAngle[idxI], sizeof(float2));
		for (int j = 0; j < (int)this->numStretch; ++j)
			CalcSample(idxI, j);
	};

	const int angle = 360 / (int)this->crossType;
	for (int i = 0; i < (int)this->crossType; ++i)
	{
		float deg = (float)((angle * i + _degree) % 360);

		this->starAngle[i].x = ao::SinCosTable::GetInstance()->GetCosTable(deg);
		this->starAngle[i].y = ao::SinCosTable::GetInstance()->GetSinTable(deg);
		ao::Vector2Normalize(&this->starAngle[i].xm, this->starAngle[i].xm);
		this->starAngle[i].x = (this->starAngle[i].x * 0.1f) / this->downSize.x;
		this->starAngle[i].y = (this->starAngle[i].y * 0.1f) / this->downSize.y;

		CalcStretch(i);
	}

	this->renderCommand.pTextureArray[0] = _pTexture;
	this->renderCommand.mulPower = _mulPower;

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		描画実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/15	05:04	T.Aoyama	作成
//------------------------------------------------------------------------------
void CrossFilterShader::OnDraw()
{
	auto& it = this->renderCommand;

	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);

	UINT offset = 0;
	UINT stride = sizeof(::SUFVERTEX);
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->pSurfaceBuffer.GetComPtrPtr(), &stride, &offset);

	this->pd3dDeviceContext->VSSetShader(this->pGlareVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pGlarePixelShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pGlareVertexShader->layout.GetComPtr());

	this->pGraphicsState->SetBlendState(BlendState::Default);
	this->pGraphicsState->SetDepthStecilState(FALSE, D3D11_DEPTH_WRITE_MASK_ZERO);

	auto RenderGlare = [&](int idxStar, int index)
	{
		::CopyMemory(this->vsConst->uvTexel, &it.uvTexel[idxStar][index], sizeof(this->vsConst->uvTexel));
		::CopyMemory(this->psConst->colorWeight, it.colorWeight[index], sizeof(this->psConst->colorWeight));

		this->vsConst.MapVertex();
		this->psConst.MapPixel();

		DirectX11RenderTarget* pRt = (index == (int)this->numStretch - 1) ? this->pReductionBuffer[idxStar] : this->pStretchBuffer[index];
		pRt->SetRenderTarget(TRUE, FALSE);
		this->pRenderer->SetTexture(0, 1, it.pTextureArray[index]);
		this->pd3dDeviceContext->Draw(3, 0);
		this->pRenderer->SetTexture(0, 1, nullptr, TRUE, TRUE);

		pRt->RestoreRenderTarget();
	};

	for (int i = 0; i < (int)this->crossType; ++i)
	{
		for (int j = 0; j < (int)this->numStretch; ++j)
		{
			RenderGlare(i, j);
		}
	}

	//アップサンプリング
	this->psUpSampleConst->mulPower = it.mulPower;
	this->pd3dDeviceContext->VSSetShader(this->pUpSampleVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pUpSamplePixelShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pUpSampleVertexShader->layout.GetComPtr());

	this->pUpSampleRenderTarget->SetRenderTarget(TRUE);
	this->pGraphicsState->SetBlendState(BlendState::Add);

	this->psUpSampleConst.MapPixel();
	this->pRenderer->SetTexture((int)this->crossType, 0, 0, it.pGlareTextureArray.data());
	this->pd3dDeviceContext->Draw(3, 0);
	this->pRenderer->SetTexture((int)this->crossType, 0, 0, this->nullTextures.data());

	this->pUpSampleRenderTarget->RestoreRenderTarget();


	//サーフェイス描画
	this->pd3dDeviceContext->VSSetShader(this->pSurfaceVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pSurfacePixelShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pSurfaceVertexShader->layout.GetComPtr());

	this->pRenderer->SetTexture(0, 0, this->pUpSampleRenderTarget->GetRenderTexture());
	this->pd3dDeviceContext->Draw(3, 0);
	this->pRenderer->SetTexture(0, 0, nullptr, TRUE, TRUE);

	this->pGraphicsState->SetDepthStecilState(TRUE);

	this->renderCommand.isActive = FALSE;
}


//------------------------------------------------------------------------------
/**	@brief		クリア処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/15	17:00	T.Aoyama	作成
//------------------------------------------------------------------------------
void CrossFilterShader::Clear(float _r, float _g, float _b, float _a)
{
	for (int i = 0; i < (int)this->numStretch; ++i)
	{
		this->pStretchBuffer[i]->Clear(_r, _g, _b, _a);
	}

	for (int i = 0; i < (int)this->crossType; ++i)
	{
		this->pReductionBuffer[i]->Clear(_r, _g, _b, _a);
	}

	this->pUpSampleRenderTarget->Clear(_r, _g, _b, _a);
}


//------------------------------------------------------------------------------
/**	@brief		光芒の色階調を初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/15	04:07	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	void CrossFilterShader::CreateGradationColor()
{
	// 光芒の色
	const float4 ColorWhite(0.63f, 0.63f, 0.63f, 0.0f);
	const float4 ChromaticAberrationColor[NUM_SAMPLING] =
	{
		float4(0.5f, 0.5f, 0.5f, 0.0f),	// 白
		float4(0.5f, 0.4f, 0.4f, 0.0f),	// 赤
		float4(0.8f, 0.4f, 0.4f, 0.0f),	// 赤
		float4(0.4f, 0.2f, 0.5f, 0.0f),	// 紫
		float4(0.2f, 0.2f, 1.0f, 0.0f),	// 青
		float4(0.2f, 0.3f, 0.7f, 0.0f),	// 青
		float4(0.2f, 0.6f, 0.2f, 0.0f),	// 緑
		float4(0.3f, 0.5f, 0.3f, 0.0f),	// 緑
	};

	for (int p = 0; p < (int)this->numStretch; ++p)
	{
		// 中心からの距離に応じて光芒の色をつける
		float Ratio = (float)(p + 1) / (float)this->numStretch;

		// それぞれのサンプリングで適当に色をつける
		for (int s = 0; s < NUM_SAMPLING; ++s)
		{
			float4 chromaticAberrColor;

			ao::Vector4Lerp(&chromaticAberrColor.xm, ChromaticAberrationColor[s].xm, ColorWhite.xm, Ratio);
			// 全体的な色の変化を調整する
			ao::Vector4Lerp(&(this->gradationColor[p].c[s].xm), ColorWhite.xm, chromaticAberrColor.xm, 0.7f);
		}
	}
}


//------------------------------------------------------------------------------
/**	@brief		色の重み定数を作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/15	22:26	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	void CrossFilterShader::CreateColorWeight()
{
	float degree = 360.0f / (float)this->crossType;
	float down = this->downSize.x + this->downSize.y;
	float source = this->sourceSize.x + this->sourceSize.y;
	float powScale = (atanf((float)ao::ToRadian(degree)) + 0.1f) * (down / source);

	//色の重みを初期化
	float tempPowScale = 0;
	auto CalcColorWeight = [&](int idxI, int idxJ)
	{
		for (int k = 0; k < NUM_SAMPLING; ++k)
		{
			float lum = powf(0.95f, tempPowScale*(float)k) * ((1.0f + idxJ)*0.5f);
			auto& cWeight = this->renderCommand.colorWeight[idxJ][k];

			cWeight.r = this->gradationColor[(int)this->numStretch - 1 - idxJ].c[k].r * lum;
			cWeight.g = this->gradationColor[(int)this->numStretch - 1 - idxJ].c[k].g * lum;
			cWeight.b = this->gradationColor[(int)this->numStretch - 1 - idxJ].c[k].b * lum;
			cWeight.w = cWeight.r + cWeight.g + cWeight.b;
		}

		tempPowScale *= NUM_SAMPLING;
	};

	//引き伸ばし分の処理
	auto CalcStretch = [&](int idxI)
	{
		tempPowScale = powScale;
		for (int j = 0; j < (int)this->numStretch; ++j)
		{
			CalcColorWeight(idxI, j);
		}
	};

	//光芒数分だけ計算キャッシュ
	for (int i = 0; i < (int)this->crossType; i++)
	{
		CalcStretch(i);
	}
}


//------------------------------------------------------------------------------
/**	@brief		サーフェイスの作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/15	03:08	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT CrossFilterShader::CreateSurface()
{
	return ::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		this->pSurfaceBuffer.ToCreator(),
		sizeof(surface),
		surface);
}