#include"stdafx.hpp"
#include"iRenderMeshShader.hpp"
#include"../Function/ShaderFunction.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Api/DirectX11Shader.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"


using namespace ao;


static CONST D3D11_INPUT_ELEMENT_DESC INPUT_LAYOUT[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXEL", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


iRenderMeshShader::iRenderMeshShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	RenderBaseShader(_pd3dDevice, _pd3dDeviceContext)
{
	this->Naming(_T("iRenderMesh"));
}


iRenderMeshShader::~iRenderMeshShader()
{
	for (auto& it : this->pRenderBurstBufferMap)
	{
		delete it.second;
	}
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	11:26	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iRenderMeshShader::Initialize(
	DirectX11GraphicsState* const _pGraphicsState,
	DirectX11GraphicsRenderer* const _pRenderer)
{
	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pVertexShader = sShader()->LoadVertexShaderFromFile(
		L"hlsl/iRenderMesh/iRenderMeshVertexShader.hlsl",
		"main",
		"vs_5_0",
		nullptr,
		nullptr,
		INPUT_LAYOUT,
		sizeof(INPUT_LAYOUT));

	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(this->pVertexShader == nullptr, E_FAIL, _T("Failed load vertex shader.\n"));


	this->pPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/iRenderMesh/iRenderMeshPixelShader.hlsl");

	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(this->pPixelShader == nullptr, E_FAIL, _T("Failed load pixel shader.\n"));


	//Shadow ver
	this->pShadowVertexShader = sShader()->LoadVertexShaderFromFile(
		L"hlsl/iRenderMesh/iRenderVSMMeshVertexShader.hlsl",
		"main",
		"vs_5_0",
		nullptr,
		nullptr,
		INPUT_LAYOUT,
		sizeof(INPUT_LAYOUT));

	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(this->pShadowVertexShader == nullptr, E_FAIL, _T("Failed load shadow vertex shader.\n"));


	this->pShadowPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/iRenderMesh/iRenderVSMMeshPixelShader.hlsl");

	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(this->pShadowPixelShader == nullptr, E_FAIL, _T("Failed load shadow pixel shader.\n"));


	this->psConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psConst.Create();

	this->pRenderer->GetProjMatrix(&this->projMatrix);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリングバーストバッファの作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	11:34	T.Aoyama	作成
//------------------------------------------------------------------------------
RenderBurstBuffer* iRenderMeshShader::CreateRenderBurstBuffer(const u32 _numInstance, BaseMesh* const _pMesh)
{
	//同じリソースが見つかれば、既存のテクスチャを返す
	ao_intptr hash = reinterpret_cast<ao_intptr>(_pMesh);
	if (this->pRenderBurstBufferMap.find(hash) != this->pRenderBurstBufferMap.end())
	{
		return this->pRenderBurstBufferMap[hash];
	}

	RenderBurstBuffer* pInstBuf = new RenderBurstBuffer(_numInstance,_pMesh);

	pInstBuf->numPushRender = 0;

	//GPUバッファ作成
	HRESULT hr(E_FAIL);

	hr = this->CreateConstantBuffer<RenderBurstBuffer::Mapping0>(_numInstance, &pInstBuf->gpuBuffer0);
	AO_CHECK_ERROR_RETURN_TEMPLATE(hr, nullptr);

	hr = this->CreateConstantBuffer<RenderBurstBuffer::Mapping1>(_numInstance, &pInstBuf->gpuBuffer1);
	AO_CHECK_ERROR_RETURN_TEMPLATE(hr, nullptr);

	hr = this->CreateConstantBuffer<RenderBurstBuffer::Mapping2>(_numInstance, &pInstBuf->gpuBuffer2);
	AO_CHECK_ERROR_RETURN_TEMPLATE(hr, nullptr);

	//MAPに登録（同じリソースを再作成させないため）
	this->pRenderBurstBufferMap.insert(std::map<ao_intptr, RenderBurstBuffer*>::value_type(hash, pInstBuf));

	return pInstBuf;
}


//------------------------------------------------------------------------------
/**	@brief		バッファのリサイズ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/17	01:42	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iRenderMeshShader::ResizeRenderBurstBuffer(
	const u32 _numInstance,
	RenderBurstBuffer* const _pRenderBurstBuffer)
{
	_pRenderBurstBuffer->numPushRender = 0;

	//GPUバッファ作成
	HRESULT hr(E_FAIL);

	hr = this->CreateConstantBuffer<RenderBurstBuffer::Mapping0>(_numInstance, &_pRenderBurstBuffer->gpuBuffer0);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->CreateConstantBuffer<RenderBurstBuffer::Mapping1>(_numInstance, &_pRenderBurstBuffer->gpuBuffer1);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->CreateConstantBuffer<RenderBurstBuffer::Mapping2>(_numInstance, &_pRenderBurstBuffer->gpuBuffer2);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		ライト用の射影行列 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/14	01:20	T.Aoyama	作成
//------------------------------------------------------------------------------
void iRenderMeshShader::UpdateLightProjMatrix(const XMFLOAT4X4& lightProjMtx)
{
	::CopyMemory(&this->lightProjMatrix, &lightProjMtx, sizeof(XMFLOAT4X4));
}


//------------------------------------------------------------------------------
/**	@brief		ビュー行列更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	11:26	T.Aoyama	作成
//------------------------------------------------------------------------------
void iRenderMeshShader::UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx)
{
	::CopyMemory(&this->viewMatrix, _pViewMtx, sizeof(XMFLOAT4X4));
	ao::MatrixMultiply(&this->viewProjMatrix, this->viewMatrix, this->projMatrix);
}


//------------------------------------------------------------------------------
/**	@brief		ライトビュー行列更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/18	23:39	T.Aoyama	作成
//------------------------------------------------------------------------------
void iRenderMeshShader::UpdateLightViewMatrix(const XMFLOAT4X4* _pLightViewMtx)
{
	::CopyMemory(&this->lightViewMatrix, _pLightViewMtx, sizeof(XMFLOAT4X4));
	ao::MatrixMultiply(&this->lightViewProjMatrix, this->lightViewMatrix, this->lightProjMatrix);
}


//------------------------------------------------------------------------------
/**	@brief		ライト方向更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	11:26	T.Aoyama	作成
//------------------------------------------------------------------------------
void iRenderMeshShader::UpdateLightDirection(float _x, float _y, float _z, float _ambientPower)
{
	this->lightDirection.x = _x;
	this->lightDirection.y = _y;
	this->lightDirection.z = _z;
	this->lightDirection.w = _ambientPower;
}


//------------------------------------------------------------------------------
/**	@brief		視点座標更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	12:27	T.Aoyama	作成
//------------------------------------------------------------------------------
void iRenderMeshShader::UpdateEyePosition(float _x, float _y, float _z, float _specularPower)
{
	this->eyePosition.x = _x;
	this->eyePosition.y = _y;
	this->eyePosition.z = _z;
	this->eyePosition.w = _specularPower;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリングコマンド登録 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	12:00	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iRenderMeshShader::PushRenderCommand(
	RenderBurstBuffer* const _pRenderBurstBuffer,
	Transform& _transform,
	const FLOAT _r,
	const FLOAT _g,
	const FLOAT _b,
	const FLOAT _a)
{
	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(_pRenderBurstBuffer == nullptr, E_INVALIDARG, _T("_pRenderBurstBuffer argument is null.\n"));

	u32 renderIndex = _pRenderBurstBuffer->numPushRender;
	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(renderIndex >= _pRenderBurstBuffer->numInstance, E_FAIL, _T("Overflow rendering command buffer.\n"));


	XMFLOAT4X4 invMtx, worldMtx;
	_transform.GetMatrix(&worldMtx);
	ao::MatrixTranspose(&invMtx, worldMtx);


	_pRenderBurstBuffer->gpuBuffer2[renderIndex].matLWVP = worldMtx;
	ao::MatrixMultiply(&_pRenderBurstBuffer->gpuBuffer1[renderIndex].matWVP.xm, worldMtx, this->viewProjMatrix);
	ao::MatrixTranspose(&_pRenderBurstBuffer->gpuBuffer1[renderIndex].matWVP.xm, _pRenderBurstBuffer->gpuBuffer1[renderIndex].matWVP.xm);

	_pRenderBurstBuffer->gpuBuffer0[renderIndex].sufColor.r = _r;
	_pRenderBurstBuffer->gpuBuffer0[renderIndex].sufColor.g = _g;
	_pRenderBurstBuffer->gpuBuffer0[renderIndex].sufColor.b = _b;
	_pRenderBurstBuffer->gpuBuffer0[renderIndex].sufColor.a = _a;


	//ライト方向をローカル空間に変換
	ao::Vector3TransformNormal(
		(XMFLOAT3*)&_pRenderBurstBuffer->gpuBuffer0[renderIndex].localLightDir,
		*(XMFLOAT3*)&this->lightDirection.xm,
		invMtx);

	//視線方向をローカル空間に変換
	ao::Vector3TransformNormal(
		(XMFLOAT3*)&_pRenderBurstBuffer->gpuBuffer0[renderIndex].localEyePos,
		*(XMFLOAT3*)&this->eyePosition,
		invMtx);

	_pRenderBurstBuffer->gpuBuffer0[renderIndex].localLightDir.w = this->lightDirection.w;
	_pRenderBurstBuffer->gpuBuffer0[renderIndex].localEyePos.w = 1;
	++_pRenderBurstBuffer->numPushRender;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリングコマンドを実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	12:36	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iRenderMeshShader::ExecuteCommandList(
	RenderBurstBuffer* const _pRenderBurstBuffer,
	const u32 _renderFlags,
	const BlendState _blendState,
	const FillState _fillState,
	const u32 _priority)
{
	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(_pRenderBurstBuffer == nullptr, E_INVALIDARG, _T("_pRenderBurstBuffer argument is null.\n"));

	if (_pRenderBurstBuffer->numPushRender == 0)
	{
		return S_FALSE;
	}

	BaseMesh* const pMesh = _pRenderBurstBuffer->pMesh;

	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(this->pd3dDeviceContext == nullptr, E_FAIL, _T("pd3dDeviceContext is null.\n"));
	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(pMesh == nullptr, E_FAIL, _T("pMesh argument is null.\n"));

	COMMAND pushCommand;
	pushCommand.numRendering = _pRenderBurstBuffer->numPushRender;
	pushCommand.pRenderBurstBuffer = _pRenderBurstBuffer;
	pushCommand.blendState = _blendState;
	pushCommand.fillState = _fillState;
	pushCommand.renderFlags = _renderFlags;


	//定数データMAP
	this->MapConstantBuffers(&_pRenderBurstBuffer->gpuBuffer0);
	this->MapConstantBuffers(&_pRenderBurstBuffer->gpuBuffer1);

	//影フラグが存在しているならライト行列を更新
	if (_renderFlags & (RenderDef::CAST_SHADOW | RenderDef::RECV_SHADOW))
	{
		for (u32 i = 0; i < _pRenderBurstBuffer->numPushRender; ++i)
		{
			ao::MatrixMultiply(&_pRenderBurstBuffer->gpuBuffer2[i].matLWVP.xm, _pRenderBurstBuffer->gpuBuffer2[i].matLWVP.xm, this->lightViewProjMatrix);
			ao::MatrixTranspose(&_pRenderBurstBuffer->gpuBuffer2[i].matLWVP.xm, _pRenderBurstBuffer->gpuBuffer2[i].matLWVP.xm);
		}

		this->MapConstantBuffers(&_pRenderBurstBuffer->gpuBuffer2);
	}

	//プライオリティ設定
	pushCommand.priority = _priority;

	// stores rendering command exclusively
	this->commandList.push_back(pushCommand);

	//レンダリングコマンドクリア
	_pRenderBurstBuffer->numPushRender = 0;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		描画コマンド実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	11:26	T.Aoyama	作成
//------------------------------------------------------------------------------
void iRenderMeshShader::OnDraw()
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

	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->pGraphicsState->SetDepthStecilState(TRUE);

	VertexShader* useVertexShader[] = { this->pVertexShader, this->pShadowVertexShader };
	PixelShader* usePixelShader[] = { this->pPixelShader, this->pShadowPixelShader };

	for (auto& it : this->commandList)
	{
		u32 index = (it.renderFlags & RenderDef::RECV_SHADOW)? 1 : 0;

		this->SetConstantBuffers(2+index,it.pRenderBurstBuffer);

		this->pd3dDeviceContext->VSSetShader(useVertexShader[index]->shader.GetComPtr(), nullptr, 0);
		this->pd3dDeviceContext->PSSetShader(usePixelShader[index]->shader.GetComPtr(), nullptr, 0);
		this->pd3dDeviceContext->IASetInputLayout(useVertexShader[index]->layout.GetComPtr());

		std::array<UINT, 1> offset = { 0 };
		std::array<UINT, 1> stride = { it.pRenderBurstBuffer->pMesh->GetStride() };
		std::array<ID3D11Buffer*, 1> vb = { *it.pRenderBurstBuffer->pMesh->GetVertexBuffer() };
		ID3D11Buffer* const ib = { it.pRenderBurstBuffer->pMesh->GetIndexBuffer() };

		this->pd3dDeviceContext->IASetVertexBuffers(0, 1, vb.data(), stride.data(), offset.data());
		this->pd3dDeviceContext->IASetIndexBuffer(ib, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		this->pGraphicsState->SetBlendState(it.blendState);
		this->pGraphicsState->SetRasterizerState(static_cast<D3D11_FILL_MODE>(it.fillState));

		UINT indexOffset = 0;
		for (auto& mIt : it.pRenderBurstBuffer->pMesh->subset)
		{
			this->pRenderer->SetTexture(3, 0, 0, mIt.pTextures.data());
			this->MapMaterialConstBuffer(mIt.material);
			this->pd3dDeviceContext->DrawIndexedInstanced((UINT)mIt.index.size(), (UINT)it.numRendering, indexOffset, 0, 0);
			indexOffset += static_cast<UINT>(mIt.index.size());
		}
	}

	this->isNeedSort = FALSE;
	this->commandList.clear();
}


//------------------------------------------------------------------------------
/**	@brief		クリア処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/15	02:11	T.Aoyama	作成
//------------------------------------------------------------------------------
void iRenderMeshShader::Clear()
{
	for (auto& it : this->pRenderBurstBufferMap)
	{
		it.second->numPushRender = 0;
	}

	this->commandList.clear();
}


//------------------------------------------------------------------------------
/**	@brief		GPUバッファ領域確保 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/10	23:26	T.Aoyama	作成
//------------------------------------------------------------------------------
template <class T>
inline HRESULT iRenderMeshShader::CreateConstantBuffer(
	const u32 _numInstance,
	RenderBurstBuffer::GpuBuffer<T>* const _pGpuBuffer)
{
	u32 stride = sizeof(T);

	//パラメーター用のバッファを確保
	D3D11_BUFFER_DESC bufferDesc;
	::SecureZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;						/// 構造化バッファのみ（PS,CSのみアンオーダード可能）
	bufferDesc.ByteWidth = stride * _numInstance;							/// データの総量
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;									/// 動的バッファ
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;			/// 構造化バッファとして作成
	bufferDesc.StructureByteStride = stride;								/// 見ての通りStride
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;						/// CPUからの書き込みを許可

	//初期値なしで作成
	HRESULT hr = this->pd3dDevice->CreateBuffer(&bufferDesc, nullptr, _pGpuBuffer->ToBufferCreator());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create structured buffer.\n"));

	//構造化バッファからシェーダーリソースビューを作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	SecureZeroMemory(&srvDesc, sizeof(D3D11_SHADER_BUFFER_DESC));
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.NumElements = _numInstance;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;

	//SRV作成
	hr = this->pd3dDevice->CreateShaderResourceView(_pGpuBuffer->GetGpuBuffer(), &srvDesc, _pGpuBuffer->ToSrViewCreator());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create shader resource view.\n"));

	//CPUバッファ確保
	_pGpuBuffer->Create(_numInstance);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		定数バッファをマッピング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/12	00:38	T.Aoyama	作成
//------------------------------------------------------------------------------
template<class T>
inline HRESULT iRenderMeshShader::MapConstantBuffers(RenderBurstBuffer::GpuBuffer<T>* const _pGpuBuffer)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HRESULT hr = this->pd3dDeviceContext->Map(_pGpuBuffer->GetGpuBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	if (FAILED(hr))
	{
		return hr;
	}

	u8* dataPtr = reinterpret_cast<u8*>(mappedData.pData);
	::CopyMemory(dataPtr, _pGpuBuffer->GetCpuBuffer(), _pGpuBuffer->GetDataSize());

	this->pd3dDeviceContext->Unmap(_pGpuBuffer->GetGpuBuffer(), 0);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		定数バッファをセット */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/11	23:45	T.Aoyama	作成
//------------------------------------------------------------------------------
inline HRESULT iRenderMeshShader::SetConstantBuffers(const u32 _numSet,RenderBurstBuffer* const _pRenderBurstBuffer)
{
	const std::array<ID3D11ShaderResourceView*, 3> pSrView =
	{
		_pRenderBurstBuffer->gpuBuffer0.GetSrView(),
		_pRenderBurstBuffer->gpuBuffer1.GetSrView(),
		_pRenderBurstBuffer->gpuBuffer2.GetSrView()
	};

	//構造化バッファマッピング
	this->pd3dDeviceContext->VSSetShaderResources(0, _numSet, pSrView.data());

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		マテリアル定数バッファマッピング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	10:41	T.Aoyama	作成
//------------------------------------------------------------------------------
inline HRESULT iRenderMeshShader::MapMaterialConstBuffer(const Material& _material)
{
	HRESULT hr(E_FAIL);

	::CopyMemory(this->psConst->diffuse, _material.diffuse.data(), sizeof(float) * 4);
	::CopyMemory(this->psConst->specular, _material.specular.data(), sizeof(float) * 4);
	::CopyMemory(this->psConst->emissive, _material.emissive.data(), sizeof(float) * 3);

	hr = this->psConst.MapPixel();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed map pixel const 0 buffer.\n"));

	return hr;
}