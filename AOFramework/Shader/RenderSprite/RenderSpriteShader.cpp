#include"stdafx.hpp"
#include"RenderSpriteShader.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Api/DirectX11Shader.hpp"


using namespace ao;


RenderSpriteShader::RenderSpriteShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	RenderBaseShader(_pd3dDevice,_pd3dDeviceContext)
{
	this->Naming(_T("RenderMesh"));
}


RenderSpriteShader::~RenderSpriteShader()
{

}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/02	23:17	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderSpriteShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/RenderSprite/RenderSpriteVertexShader.hlsl");
	this->pPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/RenderSprite/RenderSpritePixelShader.hlsl");
#ifdef __AO_DEBUG__
	if (this->pVertexShader == nullptr || this->pPixelShader == nullptr)
	{
		return E_FAIL;
	}
#endif

	this->vsConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psConst0.Register(this->pd3dDevice, this->pd3dDeviceContext);


	HRESULT hr(E_FAIL);

	//create constant buffer
	hr = this->vsConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->psConst0.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);



	hr = this->defaultLeftTopSprite.CreateCornerVertexBuffer(this->pd3dDevice.GetComPtr());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr,_T("Failed create sprite vertex buffer[Left top]\n"));

	hr = this->defaultCenterSprite.CreateCenterVertexBuffer(this->pd3dDevice.GetComPtr());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create sprite vertex buffer[Center]\n"));

	this->pRenderer->GetProjMatrix(&this->projMatrix);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		ビュー行列更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/02	23:17	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderSpriteShader::UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx)
{
	::CopyMemory(&this->viewMatrix, _pViewMtx, sizeof(XMFLOAT4X4));
	ao::MatrixMultiply(&this->viewProjMatrix, this->viewMatrix, this->projMatrix);
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/02	23:17	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderSpriteShader::Render(
	Sprite* const _pSprite,
	Transform& _transform,
	const float _r,
	const float _g,
	const float _b,
	const float _a,
	const BlendState _blendState,
	const BOOL _isCulling,
	const FillState _fillMode,
	const UINT _priority)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderSpriteShader::Render]pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}

	if (_pSprite == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderSpriteShader::Render]_pSprite argument is null.\n"));
		return E_INVALIDARG;
	}
#endif

	COMMAND pushCommand;

	//姿勢構築
	_transform.GetMatrix(&pushCommand.matrix);

	//頂点バッファ設定
	pushCommand.pSprite = _pSprite;

	//色設定
	pushCommand.color[0] = _r;
	pushCommand.color[1] = _g;
	pushCommand.color[2] = _b;
	pushCommand.color[3] = _a;

	//ブレンドステート設定
	pushCommand.blendState = _blendState;

	//描画モード設定
	pushCommand.fillMode = static_cast<D3D11_FILL_MODE>(_fillMode);

	//カリング設定
	pushCommand.cullMode = (_isCulling) ? D3D11_CULL_BACK : D3D11_CULL_NONE;

	//プライオリティ設定
	pushCommand.priority = _priority;

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
//	2014/10/02	23:17	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderSpriteShader::Render(
	BaseTexture* const _pTexture,
	Transform& _transform,
	const float _r,
	const float _g,
	const float _b,
	const float _a,
	const BlendState _blendState,
	const BOOL _isCulling,
	const FillState _fillMode,
	const UINT _priority)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderSpriteShader::Render]pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}

	if (_pTexture == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderSpriteShader::Render]_pSprite argument is null.\n"));
		return E_INVALIDARG;
	}
#endif

	COMMAND pushCommand;

	//姿勢構築
	_transform.GetMatrix(&pushCommand.matrix);

	//色設定
	pushCommand.color[0] = _r;
	pushCommand.color[1] = _g;
	pushCommand.color[2] = _b;
	pushCommand.color[3] = _a;

	//ブレンドステート設定
	pushCommand.blendState = _blendState;

	//描画モード設定
	pushCommand.fillMode = static_cast<D3D11_FILL_MODE>(_fillMode);

	//カリング設定
	pushCommand.cullMode = (_isCulling) ? D3D11_CULL_BACK : D3D11_CULL_NONE;

	//プライオリティ設定
	pushCommand.priority = _priority;

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
/**	@brief		描画コマンド実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/02	23:17	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderSpriteShader::OnDraw()
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

	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->VSSetShader(this->pVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pPixelShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pVertexShader->layout.GetComPtr());
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->pGraphicsState->SetDepthStecilState(TRUE,D3D11_DEPTH_WRITE_MASK_ZERO);

	UINT offset(0);
	for (auto& it : this->commandList)
	{
		UINT stride = static_cast<UINT>(it.pSprite->GetStride());
		
		this->MapMatrixConstBuffer(it.matrix);
		this->pd3dDeviceContext->IASetVertexBuffers(0, 1,it.pSprite->GetVertexBufferPtr(), &stride, &offset);
		this->pGraphicsState->SetBlendState(it.blendState);
		this->pGraphicsState->SetRasterizerState(it.fillMode, it.cullMode);
		::CopyMemory(this->psConst0->color, it.color, sizeof(it.color));
		this->psConst0.MapPixel();
		this->pRenderer->SetTexture(0, 0, it.pSprite->pTexture);
		this->pd3dDeviceContext->Draw((UINT)it.pSprite->GetVertexCount(), 0);
	}

	this->pGraphicsState->SetRasterizerState();

	this->isNeedSort = FALSE;
	this->commandList.clear();
}


//------------------------------------------------------------------------------
/**	@brief		行列定数バッファマッピング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/02	23:17	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderSpriteShader::MapMatrixConstBuffer(const XMFLOAT4X4& _mtx)
{
	HRESULT hr = E_FAIL;

	ao::MatrixMultiply(&this->vsConst->matWVP, _mtx, this->viewProjMatrix);
	ao::MatrixTranspose(&this->vsConst->matWVP, this->vsConst->matWVP);

	hr = this->vsConst.MapVertex();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed map vertex const buffer.\n"));

	return hr;
}