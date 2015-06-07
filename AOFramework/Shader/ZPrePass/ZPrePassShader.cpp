#include"stdafx.hpp"
#include"ZPrePassShader.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Resource/RenderBurstBuffer.hpp"


using namespace ao;

static CONST D3D11_INPUT_ELEMENT_DESC INPUT_LAYOUT[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


ZPrePassShader::ZPrePassShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	pDepthRt(new DirectX11RenderTarget(_pd3dDevice,_pd3dDeviceContext)),
	pZBlurRt(new GaussianFilterShader(_pd3dDevice,_pd3dDeviceContext)),
	RenderBaseShader(_pd3dDevice,_pd3dDeviceContext)
{
	this->Naming(_T("ZPrePass"));
}


ZPrePassShader::~ZPrePassShader()
{
	delete this->pZBlurRt;
	delete this->pDepthRt;
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/03	15:23	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT ZPrePassShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	this->pDepthRt->Initialize();
	this->pZBlurRt->Initialize(_pGraphicsState,_pRenderer);

	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/ZPrePass/ZPrePassVertexShader.hlsl");
	this->pPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/ZPrePass/ZPrePassPixelShader.hlsl");

	this->piVertexShader = sShader()->LoadVertexShaderFromFile(
		L"hlsl/ZPrePass/iZPrePassVertexShader.hlsl",
		"main",
		"vs_5_0",
		nullptr,
		nullptr,
		INPUT_LAYOUT,
		sizeof(INPUT_LAYOUT));
#ifdef __AO_DEBUG__
	if (this->pVertexShader == nullptr || this->pPixelShader == nullptr)
	{
		return E_FAIL;
	}
#endif

	this->vsConst.Register(this->pd3dDevice, this->pd3dDeviceContext);

	//create constant buffer
	HRESULT hr = this->vsConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	this->blurPower = 0.02f;

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		深度テクスチャ作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/18	12:07	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT ZPrePassShader::Create(
	const UINT _nWidth,
	const UINT _nHeight,
	const DXGI_FORMAT _format,
	const FLOAT _degree,
	const FLOAT _nZ,
	const FLOAT _fZ)
{
	HRESULT hr(E_FAIL);

	hr = this->pDepthRt->Create((int)_nWidth, (int)_nHeight, _format,0,1,TRUE,DXGI_FORMAT_R16_TYPELESS);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pZBlurRt->Create((int)_nWidth, (int)_nHeight, _format);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	ao::MatrixPerspectiveFovLH(&this->projMatrix, ao::ToRadian(_degree), _nWidth / (float)_nHeight, _nZ, _fZ);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		ビュー行列更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/03	15:23	T.Aoyama	作成
//------------------------------------------------------------------------------
void ZPrePassShader::UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx)
{
	::CopyMemory(&this->viewMatrix, _pViewMtx, sizeof(XMFLOAT4X4));
	ao::MatrixMultiply(&this->viewProjMatrix, this->viewMatrix, this->projMatrix);
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/03	15:23	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT ZPrePassShader::Render(
	BaseMesh* const _pMesh,
	const Transform& _transform,
	UINT _priority)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[ZPrePassShader::Render]pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}

	if (_pMesh == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[ZPrePassShader::Render]_pSprite argument is null.\n"));
		return E_INVALIDARG;
	}
#endif

	COMMAND pushCommand;

	//姿勢構築
	XMMATRIX translate, localRotation, scale, matrix;
	float sx = _transform.localScale.x;
	float sy = _transform.localScale.y;
	float sz = _transform.localScale.z;
	scale = ::XMMatrixScaling(sx, sy, sz);
	localRotation = ::XMMatrixRotationQuaternion(XMLoadFloat4(&_transform.localRotation));
	translate = ::XMMatrixTranslation(_transform.localPosition.x, _transform.localPosition.y, _transform.localPosition.z);
	matrix = ::XMMatrixMultiply(::XMMatrixMultiply(scale, localRotation), translate);
	XMStoreFloat4x4(&pushCommand.matrix, matrix);
	pushCommand.pMesh = _pMesh;

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
/**	@brief		レンダリングコマンドを実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	12:36	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT ZPrePassShader::ExecuteCommandList(
	RenderBurstBuffer* const _pBurstBuffer,
	UINT _priority)
{
	if (_pBurstBuffer->numPushRender == 0)
	{
		return S_FALSE;
	}

	BaseMesh* pMesh = _pBurstBuffer->pMesh;

	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(this->pd3dDeviceContext == nullptr, E_FAIL, _T("pd3dDeviceContext is null.\n"));
	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(pMesh == nullptr, E_INVALIDARG, _T("pMesh argument is null.\n"));

	ICOMMAND pushCommand;
	pushCommand.numRender = _pBurstBuffer->numPushRender;
	pushCommand.pGpuBuffer = _pBurstBuffer->gpuBuffer2.GetSrView();
	pushCommand.pMesh = pMesh;

	//プライオリティ設定
	pushCommand.priority = _priority;

	// stores rendering command exclusively
	this->iCommandList.push_back(pushCommand);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		プッシュバック時の処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/19	09:36	T.Aoyama	作成
//------------------------------------------------------------------------------
void ZPrePassShader::OnPushbackQuit()
{
	this->pZBlurRt->Render(this->pDepthRt->GetRenderTexture(), this->blurPower);
	this->pRenderer->PreRenderPushback(this->pZBlurRt);
}


//------------------------------------------------------------------------------
/**	@brief		描画コマンド実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/03	15:23	T.Aoyama	作成
//------------------------------------------------------------------------------
void ZPrePassShader::OnDraw()
{
	ao::sRenderer()->SetTexture(5, 5, nullptr,TRUE,TRUE);

	this->OnDrawBurst();
	this->OnDrawSingle();
}


//------------------------------------------------------------------------------
/**	@brief		リザルト処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/19	10:04	T.Aoyama	作成
//------------------------------------------------------------------------------
void ZPrePassShader::OnResult()
{
	ao::sRenderer()->SetTexture(5, 5, this->GetRenderTexture());
}


//------------------------------------------------------------------------------
/**	@brief		射影行列取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/01/19	00:36	T.Aoyama	作成
//------------------------------------------------------------------------------
const XMFLOAT4X4& ZPrePassShader::GetProjMatrix() const
{
	return this->projMatrix;
}


//------------------------------------------------------------------------------
/**	@brief		行列定数バッファマッピング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/03	15:23	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT ZPrePassShader::MapMatrixConstBuffer(const XMFLOAT4X4& _mtx)
{
	HRESULT hr = E_FAIL;

	ao::MatrixMultiply(&this->vsConst->matWVP, _mtx, this->viewProjMatrix);
	ao::MatrixTranspose(&this->vsConst->matWVP, this->vsConst->matWVP);

	hr = this->vsConst.MapVertex();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("[ZPrePassShader::MapConstBuffer]Failed map vertex const buffer.\n"));

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		単一レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/17	18:56	T.Aoyama	作成
//------------------------------------------------------------------------------
void ZPrePassShader::OnDrawSingle()
{
	if (this->commandList.empty())
		return;

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

	this->pDepthRt->SetRenderTarget(TRUE, TRUE);

	this->pd3dDeviceContext->VSSetShader(this->pVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pPixelShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pVertexShader->layout.GetComPtr());
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->pGraphicsState->SetBlendState(BlendState::Default);
	this->pGraphicsState->SetDepthStecilState(TRUE);

	UINT offset(0);
	for (auto& it : this->commandList)
	{
		UINT indexOffset = 0;
		UINT stride = static_cast<UINT>(it.pMesh->GetStride());
		this->MapMatrixConstBuffer(it.matrix);
		this->pd3dDeviceContext->IASetVertexBuffers(0, 1, it.pMesh->GetVertexBuffer(),&stride, &offset);
		this->pd3dDeviceContext->IASetIndexBuffer(it.pMesh->GetIndexBuffer(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		for (auto mIt : it.pMesh->subset)
		{
			this->pd3dDeviceContext->DrawIndexed((UINT)mIt.index.size(), indexOffset, 0);
			indexOffset += static_cast<UINT>(mIt.index.size());
		}
	}

	this->isNeedSort = FALSE;
	this->commandList.clear();

	this->pDepthRt->RestoreRenderTarget();
}


//------------------------------------------------------------------------------
/**	@brief		インスタンスレンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/17	18:58	T.Aoyama	作成
//------------------------------------------------------------------------------
void ZPrePassShader::OnDrawBurst()
{
	if (this->iCommandList.empty())
	{
		return;
	}

	// to sort sort if necessary
	if (this->isNeedSort)
	{
		//ラムダ式によるプレディケート
		auto Predicate = [](ICOMMAND& inst1, ICOMMAND& inst2)
		{
			return (inst1.priority < inst2.priority);
		};
		std::sort(this->iCommandList.begin(), this->iCommandList.end(), Predicate);
	}

	this->pd3dDeviceContext->VSSetShader(this->piVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pPixelShader->shader.GetComPtr(), nullptr, 0);

	this->pd3dDeviceContext->IASetInputLayout(this->piVertexShader->layout.GetComPtr());
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->pDepthRt->SetRenderTarget(TRUE, TRUE);
	this->pGraphicsState->SetBlendState(BlendState::Default);
	this->pGraphicsState->SetDepthStecilState(TRUE);

	std::array<UINT, 1> offset = { 0 };
	for (auto& it : this->iCommandList)
	{
		//定数マップ
		const std::array<ID3D11ShaderResourceView*, 1> pSrView =
		{
			it.pGpuBuffer
		};

		//構造化バッファマッピング
		this->pd3dDeviceContext->VSSetShaderResources(2, 1, pSrView.data());

		UINT indexOffset = 0;
		std::array<UINT, 1> stride = { static_cast<UINT>(it.pMesh->GetStride())};
		std::array<ID3D11Buffer*, 1> vBuffer = {*it.pMesh->GetVertexBuffer()};

		this->pd3dDeviceContext->IASetVertexBuffers(0, 1, vBuffer.data(), stride.data(), offset.data());
		this->pd3dDeviceContext->IASetIndexBuffer(it.pMesh->GetIndexBuffer(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

		for (auto& mIt : it.pMesh->subset)
		{
			this->pd3dDeviceContext->DrawIndexedInstanced((UINT)mIt.index.size(),(UINT)it.numRender, (UINT)indexOffset, 0, 0);
			indexOffset += static_cast<UINT>(mIt.index.size());
		}
	}

	this->pDepthRt->RestoreRenderTarget();

	this->isNeedSort = FALSE;
	this->iCommandList.clear();
}