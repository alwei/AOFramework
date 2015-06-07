#include"stdafx.hpp"
#include"iRenderSpriteShader.hpp"
#include"../Function/ShaderFunction.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Api/DirectX11Shader.hpp"

using namespace ao;

static CONST D3D11_INPUT_ELEMENT_DESC INPUT_LAYOUT[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXEL", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	// 入力アセンブラにジオメトリ処理用の行列を追加設定する
	{ "MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};


iRenderSpriteShader::iRenderSpriteShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	RenderBaseShader(_pd3dDevice,_pd3dDeviceContext)
{
	this->Naming(_T("iRenderSpriteShader"));
}


iRenderSpriteShader::~iRenderSpriteShader()
{
	for (auto& it : this->pInstanceDataList)
	{
		delete[] it->pInstance;
		delete it;
	}
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	12:10	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iRenderSpriteShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pVertexShader = sShader()->LoadVertexShaderFromFile(
		L"hlsl/iRenderSprite/iRenderSpriteVertexShader.hlsl",
		"main",
		"vs_5_0",
		nullptr,
		nullptr,
		INPUT_LAYOUT,
		sizeof(INPUT_LAYOUT));
	this->pPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/iRenderSprite/iRenderSpritePixelShader.hlsl");
#ifdef __AO_DEBUG__
	if (this->pVertexShader == nullptr || this->pPixelShader == nullptr)
	{
		return E_FAIL;
	}
#endif

	this->psConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psConst.Create();

	this->pRenderer->GetProjMatrix(&this->projMatrix);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		インスタンスデータ作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	12:10	T.Aoyama	作成
//------------------------------------------------------------------------------
SpriteInstance* iRenderSpriteShader::CreateInstance(
	UINT _numInstance,
	Sprite* _pSprite,
	SpritePivot _pivot)
{
	HRESULT hr(S_OK);
	SpriteInstance* pInstance = new SpriteInstance;

	pInstance->numPushRender = 0;
	pInstance->maxPushRender = _numInstance;
	pInstance->pSprite = _pSprite;

	switch (_pivot)
	{
	case SpritePivot::LeftTop:
		hr = pInstance->pSprite->CreateCornerVertexBuffer(this->pd3dDevice.GetComPtr());
		if (FAILED(hr))
		{
			return nullptr;
		}
		break;
	case SpritePivot::Center:
		hr = pInstance->pSprite->CreateCenterVertexBuffer(this->pd3dDevice.GetComPtr());
		if (FAILED(hr))
		{
			return nullptr;
		}
		break;
	default:
		AO_FAILED_FUNCTION_MESSAGE(_T("Specified pivot is not supported.\n"));
		return nullptr;
	}

	::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		pInstance->buffer.ToCreator(),
		sizeof(SpriteInstance::Instance)*_numInstance,
		nullptr,
		D3D11_USAGE_DYNAMIC,
		D3D11_CPU_ACCESS_WRITE);

	pInstance->pInstance = new SpriteInstance::Instance[_numInstance];

	this->pInstanceDataList.push_back(pInstance);

	return pInstance;
}


//------------------------------------------------------------------------------
/**	@brief		ビュー行列更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	12:10	T.Aoyama	作成
//------------------------------------------------------------------------------
void iRenderSpriteShader::UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx)
{
	::CopyMemory(&this->viewMatrix, _pViewMtx, sizeof(XMFLOAT4X4));
	ao::MatrixMultiply(&this->viewProjMatrix, this->viewMatrix, this->projMatrix);
}


//------------------------------------------------------------------------------
/**	@brief		レンダリングコマンド登録 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	12:10	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iRenderSpriteShader::PushRenderCommand(
	SpriteInstance* const _pInstance,
	Transform& _transform,
	const FLOAT _r,
	const FLOAT _g,
	const FLOAT _b,
	const FLOAT _a)
{
	int numPushRender = _pInstance->numPushRender;
#ifdef __AO_DEBUG__
	if (numPushRender >= (int)_pInstance->maxPushRender)
	{
		_pInstance->numPushRender = _pInstance->maxPushRender - 1;
		AO_FAILED_FUNCTION_MESSAGE(_T("Overflow rendering command buffer.\n"));
		return E_FAIL;
	}
#endif

	XMFLOAT4X4 invMtx, worldMtx;
	_transform.GetMatrix(&worldMtx);

	ao::MatrixMultiply(&_pInstance->pInstance[numPushRender].matrix, worldMtx, this->viewProjMatrix);
	ao::MatrixTranspose(&_pInstance->pInstance[numPushRender].matrix, _pInstance->pInstance[numPushRender].matrix);

	_pInstance->pInstance[numPushRender].color.x = _r;
	_pInstance->pInstance[numPushRender].color.y = _g;
	_pInstance->pInstance[numPushRender].color.z = _b;
	_pInstance->pInstance[numPushRender].color.w = _a;

	_pInstance->numPushRender = min(_pInstance->numPushRender+1,_pInstance->maxPushRender);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリングコマンドを実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	12:10	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iRenderSpriteShader::ExecuteCommandList(
	SpriteInstance* const _pInstance,
	BlendState _blendState,
	FillState _fillState,
	UINT _priority)
{
	if (_pInstance->numPushRender == 0)
		return S_FALSE;

#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		AO_FAILED_FUNCTION_MESSAGE(_T("pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}
#endif

	COMMAND pushCommand;
	pushCommand.numRender = _pInstance->numPushRender;
	pushCommand.pInstance = _pInstance;
	pushCommand.blendState = _blendState;
	pushCommand.fillState = _fillState;

	//プライオリティ設定
	pushCommand.priority = _priority;

	// stores rendering command exclusively
	this->mutex.lock();
	this->commandList.push_back(pushCommand);
	this->mutex.unlock();

	//レンダリングコマンドクリア
	_pInstance->numPushRender = 0;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		描画コマンド実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	12:10	T.Aoyama	作成
//------------------------------------------------------------------------------
void iRenderSpriteShader::OnDraw()
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

	std::array<UINT, 2> offset = {0,0};
	for (auto& it : this->commandList)
	{
		SpriteInstance* pInst = it.pInstance;
		ao::OverWriteVertexBuffer(
			this->pd3dDeviceContext.GetComPtr(),
			pInst->buffer.GetComPtr(),
			pInst->pInstance,
			it.numRender*sizeof(SpriteInstance::Instance));

		std::array<UINT, 2> stride = { (UINT)pInst->pSprite->GetStride(), sizeof(SpriteInstance::Instance) };
		std::array<ID3D11Buffer*, 2> vBuffer = {*pInst->pSprite->GetVertexBufferPtr(),pInst->buffer.GetComPtr()};
		this->pd3dDeviceContext->IASetVertexBuffers(0, 2, vBuffer.data(), stride.data(), offset.data());
		this->pGraphicsState->SetBlendState(it.blendState);
		this->pGraphicsState->SetRasterizerState(static_cast<D3D11_FILL_MODE>(it.fillState));
		this->pRenderer->SetTexture(0, 0, pInst->pSprite->pTexture);
		this->pd3dDeviceContext->DrawInstanced((UINT)it.pInstance->pSprite->GetVertexCount(), (UINT)it.numRender, 0, 0);
	}

	this->isNeedSort = FALSE;
	this->commandList.clear();
}


//------------------------------------------------------------------------------
/**	@brief		クリア処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	12:10	T.Aoyama	作成
//------------------------------------------------------------------------------
void iRenderSpriteShader::Clear()
{
	for (auto& it : this->pInstanceDataList)
	{
		it->numPushRender = 0;
	}

	this->commandList.clear();
}