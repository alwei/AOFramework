#include"stdafx.hpp"
#include"FadeEffectShader.hpp"
#include"../../../AOFramework/System/FpsController.hpp"
#include"../../../AOFramework/Define/SingletonTypeDefine.inl"
#include"../../../AOFramework/Api/DirectX11Shader.hpp"
#include"../../../AOFramework/Shader/Function/ShaderFunction.hpp"


using namespace ao;


namespace
{
	struct SUFVERTEX
	{
		float pos[3];
	};
}


const float _60F = 1.0f / 60.0f;
const float LimitDelta = _60F * 2;


FadeEffectShader::FadeEffectShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	isPushRender(FALSE),
	RenderBaseShader(_pd3dDevice,_pd3dDeviceContext)
{

}


FadeEffectShader::~FadeEffectShader()
{

}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/03	09:25	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT FadeEffectShader::Initialize(
	ao::DirectX11GraphicsState* _pGraphicsState,
	ao::DirectX11GraphicsRenderer* _pRenderer)
{
	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/FadeEffect/FadeEffectVertexShader.hlsl");
	this->pPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/FadeEffect/FadeEffectPixelShader.hlsl");
#ifdef __AO_DEBUG__
	if (this->pVertexShader == nullptr || this->pPixelShader == nullptr)
	{
		return E_FAIL;
	}
#endif

	static 	SUFVERTEX	surface[3] =
	{
		{ -1.f, +1.f, 0.f },
		{ +3.f, +1.f, 0.f },
		{ -1.f, -3.f, 0.f }
	};

	//頂点バッファ初期化
	ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		this->vertexBuffer.ToCreator(),
		sizeof(surface),
		surface);

	this->mapColor.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->mapColor.Create();

	this->isPushRender = FALSE;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		フェードパラメーター設定 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/03	09:54	T.Aoyama	作成
//------------------------------------------------------------------------------
void FadeEffectShader::SetFadeParameter(FadeType _fadeType,float _sec,float _init)
{
	this->maxFadeTime = _sec;
	this->nowFadeTime = _init;

	this->fadeType = _fadeType;

	switch (this->fadeType)
	{
	case FadeType::BlackOut:
	case FadeType::BlockIn:
		this->fadeColor.r = this->fadeColor.g = this->fadeColor.b = 0;
		break;
	case FadeType::WhiteIn:
	case FadeType::WhiteOut:
		this->fadeColor.r = this->fadeColor.g = this->fadeColor.b = 1;
		break;
	}

	switch (this->fadeType)
	{
	case FadeType::BlackOut:
	case FadeType::WhiteOut:
		this->fadeAlpha = this->nowFadeTime / this->maxFadeTime;
		break;
	case FadeType::WhiteIn:
	case FadeType::BlockIn:
		this->fadeAlpha = 1.0f - this->nowFadeTime / this->maxFadeTime;
		break;
	}
}


//------------------------------------------------------------------------------
/**	@brief		更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/03	09:36	T.Aoyama	作成
//------------------------------------------------------------------------------
void FadeEffectShader::Update()
{
	float d = min(LimitDelta,ao::sFpsCtrl()->GetDeltaTime());
	this->nowFadeTime = min(this->maxFadeTime, this->nowFadeTime + d);

	switch (this->fadeType)
	{
	case FadeType::BlackOut:
	case FadeType::WhiteOut:
		this->fadeAlpha = this->nowFadeTime / this->maxFadeTime;
		break;
	case FadeType::BlockIn:
	case FadeType::WhiteIn:
		this->fadeAlpha = 1.0f-this->nowFadeTime / this->maxFadeTime;
		break;
	}
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/03	09:36	T.Aoyama	作成
//------------------------------------------------------------------------------
void FadeEffectShader::Render()
{
	this->isPushRender = TRUE;
	this->mapColor->r = this->fadeColor.r;
	this->mapColor->g = this->fadeColor.g;
	this->mapColor->b = this->fadeColor.b;
	this->mapColor->a = this->fadeAlpha;
}


//------------------------------------------------------------------------------
/**	@brief		描画処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/03	09:50	T.Aoyama	作成
//------------------------------------------------------------------------------
void FadeEffectShader::OnDraw()
{
	UINT offset = 0;
	UINT stride = sizeof(::SUFVERTEX);
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetComPtrPtr(), &stride, &offset);
	this->pGraphicsState->SetBlendState(ao::BlendState::Alpha);
	this->pGraphicsState->SetDepthStecilState(FALSE);

	//サーフェイス描画
	this->mapColor.MapPixel();
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->VSSetShader(this->pVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pPixelShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pVertexShader->layout.GetComPtr());
	this->pd3dDeviceContext->Draw(3, 0);

	this->pGraphicsState->SetDepthStecilState(TRUE);

	this->isPushRender = FALSE;
}