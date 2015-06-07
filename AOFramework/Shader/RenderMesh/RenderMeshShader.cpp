#include"stdafx.hpp"
#include"RenderMeshShader.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Api/DirectX11Shader.hpp"


using namespace ao;


RenderMeshShader::RenderMeshShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	RenderBaseShader(_pd3dDevice,_pd3dDeviceContext)
{
	this->Naming(_T("RenderMesh"));
}


RenderMeshShader::~RenderMeshShader()
{

}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/20	09:24	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderMeshShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/RenderMesh/RenderMeshVertexShader.hlsl");
	this->pPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/RenderMesh/RenderMeshPixelShader.hlsl");
#ifdef __AO_DEBUG__
	if (this->pVertexShader == nullptr || this->pPixelShader == nullptr)
	{
		return E_FAIL;
	}
#endif

	this->pVSMVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/RenderMesh/RenderVSMMeshVertexShader.hlsl");
	this->pVSMPixelShader = sShader()->LoadPixelShaderFromFile(
		L"hlsl/RenderMesh/RenderVSMMeshPixelShader.hlsl", "main", "ps_5_0",
		D3D_COMPILE_STANDARD_FILE_INCLUDE);
#ifdef __AO_DEBUG__
	if (this->pVSMVertexShader == nullptr || this->pVSMPixelShader == nullptr)
	{
		return E_FAIL;
	}
#endif

	this->vsConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psConst0.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psConst1.Register(this->pd3dDevice, this->pd3dDeviceContext);

	//create constant buffer
	this->vsConst.Create();
	this->psConst0.Create();
	this->psConst1.Create();

	this->pRenderer->GetProjMatrix(&this->projMatrix);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		ライト用の射影行列 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/01/19	00:33	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderMeshShader::UpdateLightProjMatrix(const XMFLOAT4X4& lightProjMtx)
{
	::CopyMemory(&this->lightProjMatrix, &lightProjMtx, sizeof(XMFLOAT4X4));
}


//------------------------------------------------------------------------------
/**	@brief		ビュー行列更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/20	10:56	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderMeshShader::UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx)
{
	::CopyMemory(&this->viewMatrix, _pViewMtx, sizeof(XMFLOAT4X4));
	ao::MatrixMultiply(&this->viewProjMatrix, this->viewMatrix, this->projMatrix);
}


//------------------------------------------------------------------------------
/**	@brief		ライトビュー行列更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/19	01:15	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderMeshShader::UpdateLightViewMatrix(const XMFLOAT4X4* _pLightViewMtx)
{
	::CopyMemory(&this->lightViewMatrix, _pLightViewMtx, sizeof(XMFLOAT4X4));
	ao::MatrixMultiply(&this->lightViewProjMatrix, this->lightViewMatrix, this->lightProjMatrix);
}


//------------------------------------------------------------------------------
/**	@brief		ライト方向更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/21	23:09	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderMeshShader::UpdateLightDirection(float _x,float _y,float _z,float _ambientPower)
{
	this->lightDir.x = _x;
	this->lightDir.y = _y;
	this->lightDir.z = _z;
	this->psConst1->lightDir.w = _ambientPower;
}


//------------------------------------------------------------------------------
/**	@brief		視点座標の更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/03	10:07	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderMeshShader::UpdateEyePosition(
	float _x,float _y,float _z,
	float _specularPower,int _specularRange)
{
	this->eyePos.x = _x;
	this->eyePos.y = _y;
	this->eyePos.z = _z;
	this->vsConst->eyePos.w = _specularPower;
	this->psConst1->specularRange = _specularRange;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/20	09:27	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderMeshShader::Render(
	BaseMesh* const _pMesh,
	Transform& _transform,
	const BOOL _disableLighting,
	const BOOL _isReceiveShadow,
	const BlendState _blendState,
	const FillState _fillState,
	const ao::float4 _dColor,
	const ao::float4 _sColor,
	const ao::float4 _eColor,
	const UINT _priority)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderMeshShader::Render]pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}

	if (_pMesh == nullptr){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderMeshShader::Render]_pMesh argument is null.\n"));
		return E_INVALIDARG;
	}
#endif

	COMMAND pushCommand;

	//姿勢構築
	_transform.GetMatrix(&pushCommand.matrix);

	//頂点バッファ設定
	pushCommand.stride = _pMesh->GetStride();
	pushCommand.ppVertexBuffer = _pMesh->GetVertexBuffer();
	pushCommand.pIndexBuffer = _pMesh->GetIndexBuffer();
	pushCommand.pSubsets = &_pMesh->subset;

	//色設定
	::CopyMemory(pushCommand.dColor.f, _dColor.f, sizeof(pushCommand.dColor));
	::CopyMemory(pushCommand.sColor.f, _sColor.f, sizeof(pushCommand.sColor));
	::CopyMemory(pushCommand.eColor.f, _eColor.f, sizeof(pushCommand.eColor));

	//フィルステート
	pushCommand.fillState = _fillState;

	//ブレンドステート設定
	pushCommand.blendState = _blendState;

	//ライティングフラグ
	pushCommand.disableLighting = _disableLighting;

	//シャドウフラグ
	pushCommand.shadowFlag = _isReceiveShadow;

	//プライオリティ設定
	pushCommand.priority = _priority;

	// stores rendering command exclusively
	this->commandList.push_back(pushCommand);

	// sort is necessary
	if (!this->isNeedSort)
		this->isNeedSort = (_priority != 0xCCCCCCC);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		描画コマンド実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/20	09:26	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderMeshShader::OnDraw()
{
	if (this->commandList.empty())
		return;

	// to sort sort if necessary
	if (this->isNeedSort)
	{
		//ラムダ式によるプレディケート
		auto Predicate = [](COMMAND& inst1, COMMAND& inst2){
			return (inst1.priority < inst2.priority);
		};
		std::sort(this->commandList.begin(), this->commandList.end(), Predicate);
	}

	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pVertexShader->layout.GetComPtr());
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->pGraphicsState->SetDepthStecilState(TRUE);

	VertexShader* useVertexShader[] = { this->pVertexShader, this->pVSMVertexShader };
	PixelShader* usePixelShader[] = { this->pPixelShader, this->pVSMPixelShader };

	for (auto& it : this->commandList)
	{
		UINT offset = 0;
		UINT indexOffset = 0;
		this->pd3dDeviceContext->VSSetShader(useVertexShader[it.shadowFlag]->shader.GetComPtr(), nullptr, 0);
		this->pd3dDeviceContext->PSSetShader(usePixelShader[it.shadowFlag]->shader.GetComPtr(), nullptr, 0);
		this->pd3dDeviceContext->IASetVertexBuffers(0, 1, it.ppVertexBuffer, &it.stride, &offset);
		this->pd3dDeviceContext->IASetIndexBuffer(it.pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		this->pGraphicsState->SetBlendState(it.blendState);
		this->pGraphicsState->SetRasterizerState(static_cast<D3D11_FILL_MODE>(it.fillState));

		this->psConst1->disableLighting = it.disableLighting;
		::CopyMemory(this->psConst1->dColor.f, it.dColor.f, sizeof(this->psConst1->dColor));
		::CopyMemory(this->psConst1->sColor.f, it.sColor.f, sizeof(this->psConst1->sColor));
		::CopyMemory(this->psConst1->eColor.f, it.eColor.f, sizeof(this->psConst1->eColor));
		this->MapConstBuffer(it.matrix, it.shadowFlag);

		for(auto mIt = it.pSubsets->begin(); mIt != it.pSubsets->end(); ++mIt )
		{			
			this->pRenderer->SetTexture((UINT)(mIt->pTextures.size()), 0, 0, mIt->pTextures.data());
			this->MapMaterialBuffer(mIt->material);
			this->pd3dDeviceContext->DrawIndexed((UINT)mIt->index.size(), (UINT)indexOffset, 0);
			indexOffset += static_cast<UINT>(mIt->index.size());
		}
	}

	this->isNeedSort = FALSE;
	this->commandList.clear();
}


//------------------------------------------------------------------------------
/**	@brief		行列定数バッファマッピング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/20	10:41	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderMeshShader::MapConstBuffer(
	const XMFLOAT4X4& _mtx,
	BOOL _isReceiveShadow)
{
	HRESULT hr = E_FAIL;

	XMFLOAT4X4 invMtx;
	ao::MatrixInverse(&invMtx, _mtx);
	ao::MatrixMultiply(&this->vsConst->matWVP, _mtx, this->viewProjMatrix);
	ao::MatrixTranspose(&this->vsConst->matWVP, this->vsConst->matWVP);
	ao::Vector3TransformCoord((XMFLOAT3*)&this->vsConst->eyePos, this->eyePos, invMtx);
	if (_isReceiveShadow){
		ao::MatrixMultiply(&this->vsConst->matLWVP, _mtx, this->lightViewProjMatrix);
		ao::MatrixTranspose(&this->vsConst->matLWVP, this->vsConst->matLWVP);
	}

	hr = this->vsConst.MapVertex();
#ifdef __AO_DEBUG__
	if (FAILED(hr)){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderMeshShader::MapConstBuffer]Failed map vertex const buffer.\n"));
		return hr;
	}
#endif

	ao::Vector3TransformNormal((XMFLOAT3*)&this->psConst1->lightDir, this->lightDir, invMtx);
	ao::Vector3Normalize((XMFLOAT3*)&this->psConst1->lightDir, *(XMFLOAT3*)&this->psConst1->lightDir);
	hr = this->psConst1.MapPixel(1);
#ifdef __AO_DEBUG__
	if (FAILED(hr)){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderMeshShader::MapConstBuffer]Failed map pixel const 1 buffer.\n"));
		return hr;
	}
#endif

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		マテリアル定数バッファマッピング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/20	10:41	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderMeshShader::MapMaterialBuffer(const Material& _material)
{
	::CopyMemory(this->psConst0->diffuse.f, _material.diffuse.data(), sizeof(float) * 4);
	::CopyMemory(this->psConst0->specular.f, _material.specular.data(), sizeof(float) * 4);
	::CopyMemory(this->psConst0->emissive.f, _material.emissive.data(), sizeof(float) * 3);
	HRESULT hr = this->psConst0.MapPixel();
#ifdef __AO_DEBUG__
	if (FAILED(hr)){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderMeshShader::MapConstBuffer]Failed map pixel const 0 buffer.\n"));
		return hr;
	}
#endif

	return hr;
}