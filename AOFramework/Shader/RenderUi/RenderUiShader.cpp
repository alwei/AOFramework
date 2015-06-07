#include"stdafx.hpp"
#include"RenderUiShader.hpp"
#include"../../Math/Math.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Shader/Function/ShaderFunction.hpp"
#include"../../Api/DirectX11Shader.hpp"


namespace
{
	struct TRIPOLYGON
	{
		float pos[3];
		float uv[2];
	};

	static 	TRIPOLYGON	triPolygon[3] =
	{
		{ -1.f, +1.f, 0.f, 0.f, 0.f },
		{ +3.f, +1.f, 0.f, 2.f, 0.f },
		{ -1.f, -3.f, 0.f, 0.f, 2.f }
	};
}


using namespace ao;


RenderUiShader::RenderUiShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	isNeedSort(FALSE),
	pVertexShader(nullptr),
	pPixelShader(nullptr),
	RenderBaseShader(_pd3dDevice, _pd3dDeviceContext)
{
	this->Naming(_T("RenderUiShader"));
}


RenderUiShader::~RenderUiShader()
{

}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	00:12	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderUiShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	HRESULT hr(E_FAIL);
	this->vsConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psConst.Register(this->pd3dDevice, this->pd3dDeviceContext);

	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/RenderUi/RenderUiVertexShader.hlsl");
	this->pPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/RenderUi/RenderUiPixelShader.hlsl");
#ifdef __AO_DEBUG__
	if (this->pVertexShader == nullptr || this->pPixelShader == nullptr)
	{
		return E_FAIL;
	}
#endif

	//create constant buffer
	hr = this->vsConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->psConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	this->pRenderer->GetLeftTopOrthoMatrix(&this->leftTopOrthoMatrix);
	this->pRenderer->GetCenterOrthoMatrix(&this->centerOrthoMatrix);

	hr = this->CreateDefaultSprite();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->CreatePointSampler();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	00:10	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderUiShader::Render(
	Sprite* const _pSprite,
	const int _x,
	const int _y,
	const float _xScale,
	const float _yScale,
	const float _radian,
	const float _r,
	const float _g,
	const float _b,
	const float _a,
	const BlendState _blendState,
	const FillState _fillState,
	const SpritePivot _spritePivot,
	const SurfacePivot _surfacePivot,
	const BOOL _isCache,
	const UINT _priority)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderUiShader::Render]pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}

	if (_pSprite == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderUiShader::Render]_pSprite argument is null.\n"));
		return E_INVALIDARG;
	}
#endif

	COMMAND pushCommand;

	pushCommand.x = _x;
	pushCommand.y = -_y;
	pushCommand.sx = _xScale;
	pushCommand.sy = _yScale;
	pushCommand.radian = _radian;
	pushCommand.color[0] = _r;
	pushCommand.color[1] = _g;
	pushCommand.color[2] = _b;
	pushCommand.color[3] = _a;
	pushCommand.blendState = _blendState;
	pushCommand.fillState = _fillState;
	pushCommand.isCache = _isCache;
	pushCommand.surfacePivot = _surfacePivot;
	pushCommand.priority = _priority;
	pushCommand.pSprite = _pSprite;
	pushCommand.pTexture = _pSprite->pTexture;

	switch (_spritePivot)
	{
	case SpritePivot::LeftTop:
		pushCommand.pSprite = &this->defaultLeftTopSprite;
		break;
	case SpritePivot::Center:
		pushCommand.pSprite = &this->defaultCenterSprite;
		break;
	}

	// stores rendering command exclusively
	this->commandList.push_back(pushCommand);

	// sort is necessary
	if (!this->isNeedSort)
	{
		this->isNeedSort = (_priority != 0xCCCCCCC);
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	00:10	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderUiShader::Render(
	BaseTexture* const _pTexture,
	const int _x,
	const int _y,
	const float _xScale,
	const float _yScale,
	const float _radian,
	const float _r,
	const float _g,
	const float _b,
	const float _a,
	const BlendState _blendState,
	const FillState _fillState,
	const SpritePivot _spritePivot,
	const SurfacePivot _surfacePivot,
	const BOOL _isCache,
	const UINT _priority)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderUiShader::Render]pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}

	if (_pTexture == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderUiShader::Render]_pSprite argument is null.\n"));
		return E_INVALIDARG;
	}
#endif

	COMMAND pushCommand;

	pushCommand.x = _x;
	pushCommand.y = -_y;
	pushCommand.sx = _xScale;
	pushCommand.sy = _yScale;
	pushCommand.radian = _radian;
	pushCommand.color[0] = _r;
	pushCommand.color[1] = _g;
	pushCommand.color[2] = _b;
	pushCommand.color[3] = _a;
	pushCommand.isCache = _isCache;
	pushCommand.surfacePivot = _surfacePivot;
	pushCommand.blendState = _blendState;
	pushCommand.fillState = _fillState;
	pushCommand.priority = _priority;
	pushCommand.pTexture = _pTexture;

	switch (_spritePivot)
	{
	case SpritePivot::LeftTop:
		pushCommand.pSprite = &this->defaultLeftTopSprite;
		break;
	case SpritePivot::Center:
		pushCommand.pSprite = &this->defaultCenterSprite;
		break;
	}

	// stores rendering command exclusively
	this->commandList.push_back(pushCommand);

	// sort is necessary
	if (!this->isNeedSort)
	{
		this->isNeedSort = (_priority != 0xCCCCCCC);
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		テクスチャスロット開放 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/04	00:26	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderUiShader::ReleaseTextureSlot(UINT _priority)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderUiShader::Render]pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}
#endif

	COMMAND pushCommand;

	pushCommand.x = 0;
	pushCommand.y = 0;
	pushCommand.sx = 0;
	pushCommand.sy = 0;
	pushCommand.radian = 0;
	pushCommand.color[0] = 0;
	pushCommand.color[1] = 0;
	pushCommand.color[2] = 0;
	pushCommand.color[3] = 0;
	pushCommand.blendState = BlendState::Invalid;
	pushCommand.isCache = FALSE;
	pushCommand.isCenter = FALSE;
	pushCommand.surfacePivot = SurfacePivot::Center;
	pushCommand.priority = _priority;
	pushCommand.pTexture = nullptr;
	pushCommand.pSprite = nullptr;

	// stores rendering command exclusively
	this->commandList.push_back(pushCommand);

	// sort is necessary
	if (!this->isNeedSort)
	{
		this->isNeedSort = (_priority != 0xCCCCCCC);
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		描画コマンド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	22:06	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderUiShader::OnDraw()
{
	if (this->commandList.empty())
	{
		return;
	}

	// to sort sort if necessary
	if (this->isNeedSort)
	{
		//ラムダ式によるプレディケート
		auto Predicate = [](COMMAND& inst1, COMMAND& inst2)
		{
			return (inst1.priority < inst2.priority);
		};
		std::sort(this->commandList.begin(), this->commandList.end(), Predicate);
	}

	this->pd3dDeviceContext->VSSetShader(this->pVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pPixelShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pVertexShader->layout.GetComPtr());
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
	this->pGraphicsState->SetDepthStecilState(FALSE);

	UINT offset = 0;
	for (auto& it : this->commandList)
	{
		UINT stride = static_cast<UINT>(it.pSprite->GetStride());
		ID3D11ShaderResourceView** ppUseSrView = (it.pTexture) ? it.pTexture->pSrView.GetComPtrPtr() : nullptr;
		this->pRenderer->SetTexture(0, 0, ppUseSrView, it.pTexture->pSampState.GetComPtrPtr(), it.isCache);
		this->pGraphicsState->SetBlendState(it.blendState);
		this->pGraphicsState->SetRasterizerState(static_cast<D3D11_FILL_MODE>(it.fillState));
		this->MapConstBuffer(it);
		this->pd3dDeviceContext->IASetVertexBuffers(0, 1, it.pSprite->GetVertexBufferPtr(), &stride, &offset);
		this->pd3dDeviceContext->Draw((UINT)it.pSprite->GetVertexCount(), 0);
	}

	this->isNeedSort = FALSE;
	this->commandList.clear();

	this->pGraphicsState->SetDepthStecilState(TRUE);
}


//------------------------------------------------------------------------------
/**	@brief		定数バッファマッピング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/16	21:50	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderUiShader::MapConstBuffer(const COMMAND& _command)
{
	HRESULT hr(E_FAIL);
	XMFLOAT4X4 worldMtx, rotMtx, scaleMtx;
	ao::MatrixIdentity(&worldMtx);
	ao::MatrixIdentity(&rotMtx);
	ao::MatrixIdentity(&scaleMtx);

	ao::MatrixRotationZ(&rotMtx, _command.radian);
	ao::MatrixScale(&scaleMtx, _command.sx, _command.sy, 0);
	ao::MatrixMultiply(&worldMtx, scaleMtx, rotMtx);
	worldMtx._41 = static_cast<float>(_command.x);
	worldMtx._42 = static_cast<float>(_command.y);


	XMFLOAT4X4* pOrthoMtx(nullptr);

	switch (_command.surfacePivot)
	{
	case SurfacePivot::LeftTop:
		pOrthoMtx = &this->leftTopOrthoMatrix;
		break;
	case SurfacePivot::Center:
		pOrthoMtx = &this->centerOrthoMatrix;
		break;
	}

	ao::MatrixMultiply(&this->vsConst->matWO, worldMtx, *pOrthoMtx);
	ao::MatrixTranspose(&this->vsConst->matWO, this->vsConst->matWO);
	hr = this->vsConst.MapVertex();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed map vertex const buffer.\n"));

	this->psConst->color[0] = _command.color[0];
	this->psConst->color[1] = _command.color[1];
	this->psConst->color[2] = _command.color[2];
	this->psConst->color[3] = _command.color[3];
	hr = this->psConst.MapPixel();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed map pixel const buffer.\n"));

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		デフォルトスプライトの作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/03	19:24	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderUiShader::CreateDefaultSprite()
{
	HRESULT hr(E_FAIL);

	hr = this->defaultLeftTopSprite.CreateCornerVertexBuffer(this->pd3dDevice.GetComPtr());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create sprite vertex buffer[Left top]\n"));

	hr = this->defaultCenterSprite.CreateCenterVertexBuffer(this->pd3dDevice.GetComPtr());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create sprite vertex buffer[Center]\n"));

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		ポイントサンプラー作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/09	01:02	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderUiShader::CreatePointSampler()
{
	// テクスチャイメージの情報を設定
	D3D11_SAMPLER_DESC	samplerDesc;

	//テクスチャのサンプラステートを設定
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;	// サンプリング時に使用するフィルタ。
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 ～ 1 の範囲外にある u テクスチャー座標の描画方法
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 ～ 1 の範囲外にある v テクスチャー座標
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 ～ 1 の範囲外にある w テクスチャー座標
	samplerDesc.MipLODBias = 0;								// 計算されたミップマップ レベルからのバイアス
	samplerDesc.MaxAnisotropy = 16;							// サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 。
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	// 比較オプション。
	// 境界色をバーストコピー
	::CopyMemory(samplerDesc.BorderColor, &XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(XMFLOAT4));
	//ミップマップの設定
	samplerDesc.MinLOD = 0;							// アクセス可能なミップマップの下限値
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;			// アクセス可能なミップマップの上限値

	//サンプラーステート作成
	HRESULT hr = E_FAIL;

	hr = this->pd3dDevice->CreateSamplerState(&samplerDesc, this->pointSampler.ToCreator());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create sampler state.\n"));

	return S_OK;
}