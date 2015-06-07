#include"stdafx.hpp"
#include"RenderMeshAnimationShader.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Api/DirectX11Shader.hpp"


using namespace ao;


static CONST D3D11_INPUT_ELEMENT_DESC INPUT_LAYOUT[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	// アニメーション用の追加インデックス頂点バッファ
	{ "MESH_INDEX", 0, DXGI_FORMAT_R32_SINT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


RenderMeshAnimationShader::RenderMeshAnimationShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	pPixelShader(nullptr),
	pVSMPixelShader(nullptr),
	pGraphicsState(nullptr),
	pRenderer(nullptr),
	RenderBaseShader(_pd3dDevice,_pd3dDeviceContext)

{
	this->Naming(_T("RenderMesh"));
}


RenderMeshAnimationShader::~RenderMeshAnimationShader()
{
	for (auto& it : this->instanceList)
	{
		delete it;
	}
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/31	11:46	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderMeshAnimationShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/RenderMeshAnimation/RenderMeshAnimationPixelShader.hlsl");
#ifdef __AO_DEBUG__
	if (this->pPixelShader == nullptr)
	{
		return E_FAIL;
	}
#endif

	this->pVSMPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/RenderMeshAnimation/RenderVSMMeshAnimationPixelShader.hlsl");
#ifdef __AO_DEBUG__
	if (this->pVSMPixelShader == nullptr)
	{
		return E_FAIL;
	}
#endif

	this->vsConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psConst0.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psConst1.Register(this->pd3dDevice, this->pd3dDeviceContext);

	//create constant buffer
	HRESULT hr(E_FAIL);

	hr = this->vsConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->psConst0.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->psConst1.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	this->pRenderer->GetProjMatrix(&this->projMatrix);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		アニメーション用のメッシュインスタンス生成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/31	11:58	T.Aoyama	作成
//------------------------------------------------------------------------------
AnimationInstance* RenderMeshAnimationShader::CreateAnimationInstance(BaseMesh* _pMesh, UINT _numMesh)
{
#ifdef __AO_DEBUG__
	if (_pMesh == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderMeshAnimationShader::CreateAnimationInstance]_pMesh argument is null.\n"));
		return nullptr;
	}

	if (_pMesh->GetType() != ao::ResourceType::STA_MESH)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderMeshAnimationShader::CreateAnimationInstance]Mesh is not STA_MESH.\n"));
		return nullptr;
	}
#endif

	AnimationInstance* pInstance = new AnimationInstance();

	std::array<char, 16> numMesh;
	sprintf_s(numMesh.data(), 16, "%d", _numMesh);
	D3D_SHADER_MACRO macro[] = { "NUM_MESH", numMesh.data(), nullptr, nullptr };

	pInstance->pVertexShader = sShader()->LoadVertexShaderFromFile(
		L"hlsl/RenderMeshAnimation/RenderMeshAnimationVertexShader.hlsl",
		"main", "vs_5_0", nullptr, macro, INPUT_LAYOUT, sizeof(INPUT_LAYOUT));
#ifdef __AO_DEBUG__
	if (pInstance->pVertexShader == nullptr)
	{
		delete pInstance;
		return nullptr;
	}
#endif

	pInstance->pVSMVertexShader = sShader()->LoadVertexShaderFromFile(
		L"hlsl/RenderMeshAnimation/RenderVSMMeshAnimationVertexShader.hlsl",
		"main", "vs_5_0", nullptr, macro, INPUT_LAYOUT, sizeof(INPUT_LAYOUT));
#ifdef __AO_DEBUG__
	if (pInstance->pVSMVertexShader == nullptr)
	{
		delete pInstance;
		return nullptr;
	}
#endif

	std::vector<XMFLOAT4X4> identityMtx;

	identityMtx.resize(_numMesh);
	for (UINT i = 0; i < _numMesh; ++i)
	{
		ao::MatrixIdentity(&identityMtx[i]);
	}

	pInstance->mesh = reinterpret_cast<StaMesh*>(_pMesh);
	pInstance->mtxBuffer.Create(_numMesh);
	pInstance->bufferSize = pInstance->mtxBuffer.GetBufferSize();
	pInstance->mtxBuffer.Register(this->pd3dDevice, this->pd3dDeviceContext);

	::CopyMemory(pInstance->mtxBuffer.data(), identityMtx.data(), sizeof(XMFLOAT4X4)*_numMesh);

	this->instanceList.push_back(pInstance);

	return pInstance;
}


//------------------------------------------------------------------------------
/**	@brief		ビュー行列更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/31	11:46	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderMeshAnimationShader::UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx)
{
	::CopyMemory(&this->viewMatrix, _pViewMtx, sizeof(XMFLOAT4X4));
	ao::MatrixMultiply(&this->viewProjMatrix, this->viewMatrix, this->projMatrix);
}


//------------------------------------------------------------------------------
/**	@brief		ライトビュー行列更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/31	11:46	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderMeshAnimationShader::UpdateLightViewMatrix(const XMFLOAT4X4* _pLightViewMtx)
{
	::CopyMemory(&this->lightViewMatrix, _pLightViewMtx, sizeof(XMFLOAT4X4));
	ao::MatrixMultiply(&this->lightViewProjMatrix, this->lightViewMatrix, this->projMatrix);
}


//------------------------------------------------------------------------------
/**	@brief		ライト方向更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/31	11:46	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderMeshAnimationShader::UpdateLightDirection(float _x, float _y, float _z, float _ambientPower)
{
	this->lightDir.x = _x;
	this->lightDir.y = _y;
	this->lightDir.z = _z;
	this->psConst1->lightDir[3] = _ambientPower;
}


//------------------------------------------------------------------------------
/**	@brief		視点座標の更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/31	11:46	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderMeshAnimationShader::UpdateEyePosition(float _x, float _y, float _z, float _specularPower)
{
	this->eyePos.x = _x;
	this->eyePos.y = _y;
	this->eyePos.z = _z;
	this->psConst1->eyePos[3] = _specularPower;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/31	11:46	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderMeshAnimationShader::Render(
	AnimationInstance* _pInstance,
	Transform& _transform,
	const float _r,
	const float _g,
	const float _b,
	const float _a,
	const BOOL _disableLighting,
	const BOOL _isReceiveShadow,
	const BlendState _blendState,
	const FillState _fillState,
	const UINT _priority)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderMeshAnimationShader::Render]pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}

	if (_pInstance == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderMeshAnimationShader::Render]_pInstance argument is null.\n"));
		return E_INVALIDARG;
	}

	if (_pInstance->mesh == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[RenderMeshAnimationShader::Render]_pInstance->mesh argument is null.\n"));
		return E_INVALIDARG;
	}
#endif

	COMMAND pushCommand;

	//姿勢構築
	_transform.GetMatrix(&pushCommand.matrix);

	//頂点バッファ設定
	BaseMesh* pMesh = _pInstance->mesh;
	pushCommand.stride = pMesh->GetStride();
	pushCommand.pUseVertexShader = (_isReceiveShadow) ? _pInstance->pVSMVertexShader : _pInstance->pVertexShader;
	pushCommand.pInstance = _pInstance;

	//色設定
	pushCommand.color[0] = _r;
	pushCommand.color[1] = _g;
	pushCommand.color[2] = _b;
	pushCommand.color[3] = _a;

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
	this->mutex.lock();
	this->commandList.push_back(pushCommand);
	this->mutex.unlock();

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
//	2014/10/31	11:46	T.Aoyama	作成
//------------------------------------------------------------------------------
void RenderMeshAnimationShader::OnDraw()
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
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->pGraphicsState->SetDepthStecilState(TRUE);


	PixelShader* usePixelShader[] = { this->pPixelShader, this->pVSMPixelShader };

	for (auto& it : this->commandList)
	{
		this->pd3dDeviceContext->IASetInputLayout(it.pUseVertexShader->layout.GetComPtr());

		UINT indexOffset = 0;
		this->MapMatrixConstBuffer(it.matrix, it.pInstance, it.shadowFlag);

		UINT stride[] = { it.stride, sizeof(int) };
		UINT offset[] = { 0, 0 };
		StaMesh* pMesh = it.pInstance->mesh;
		ID3D11Buffer* setVB[] = { *pMesh->GetVertexBuffer(), *pMesh->GetMeshIndexBuffer() };
		this->pd3dDeviceContext->VSSetShader(it.pUseVertexShader->shader.GetComPtr(), nullptr, 0);
		this->pd3dDeviceContext->PSSetShader(usePixelShader[it.shadowFlag]->shader.GetComPtr(), nullptr, 0);
		this->pd3dDeviceContext->IASetVertexBuffers(0, 2, setVB, stride, offset);
		this->pd3dDeviceContext->IASetIndexBuffer(pMesh->GetIndexBuffer(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		this->pGraphicsState->SetBlendState(it.blendState);
		this->pGraphicsState->SetRasterizerState(static_cast<D3D11_FILL_MODE>(it.fillState));
		this->psConst1->disableLighting = it.disableLighting;
		::CopyMemory(this->psConst1->color, it.color, sizeof(it.color));
		for (auto mIt : pMesh->subset)
		{
			this->pRenderer->SetTexture(3, 0, 0, mIt.pTextures.data());
			this->MapMaterialConstBuffer(mIt.material, it.matrix);
			this->pd3dDeviceContext->DrawIndexed((UINT)mIt.index.size(), indexOffset, 0);
			indexOffset += static_cast<UINT>(mIt.index.size());
		}
	}

	this->isNeedSort = FALSE;
	this->commandList.clear();
}


//------------------------------------------------------------------------------
/**	@brief		行列定数バッファマッピング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/31	11:46	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderMeshAnimationShader::MapMatrixConstBuffer(
	const XMFLOAT4X4& _mtx,
	AnimationInstance* _pInstance,
	BOOL _isReceiveShadow)
{
	HRESULT hr = E_FAIL;

	ao::MatrixMultiply(&this->vsConst->matWVP, _mtx, this->viewProjMatrix);
	ao::MatrixTranspose(&this->vsConst->matWVP, this->vsConst->matWVP);
	if (_isReceiveShadow)
	{
		ao::MatrixMultiply(&this->vsConst->matLWVP, _mtx, this->lightViewProjMatrix);
		ao::MatrixTranspose(&this->vsConst->matLWVP, this->vsConst->matLWVP);
	}

	hr = this->vsConst.MapVertex();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed map vertex 0 const buffer.\n"));

	hr = _pInstance->mtxBuffer.MapVertex(1, 1);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed map vertex 1 const buffer.\n"));


	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		マテリアル定数バッファマッピング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/31	11:46	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT RenderMeshAnimationShader::MapMaterialConstBuffer(const Material& _material, const XMFLOAT4X4& _wRotation)
{
	HRESULT hr = E_FAIL;

	::CopyMemory(this->psConst0->diffuse, _material.diffuse.data(), sizeof(float) * 4);
	::CopyMemory(this->psConst0->specular, _material.specular.data(), sizeof(float) * 4);
	::CopyMemory(this->psConst0->emissive, _material.emissive.data(), sizeof(float) * 3);

	hr = this->psConst0.MapPixel();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed map pixel const 0 buffer.\n"));

	ao::Vector3TransformNormal((XMFLOAT3*)&this->psConst1->lightDir, this->lightDir, _wRotation);
	ao::Vector3Normalize((XMFLOAT3*)&this->psConst1->lightDir, *(XMFLOAT3*)&this->psConst1->lightDir);
	ao::Vector3TransformNormal((XMFLOAT3*)&this->psConst1->eyePos, this->eyePos, _wRotation);
	ao::Vector3Normalize((XMFLOAT3*)&this->psConst1->eyePos, *(XMFLOAT3*)&this->psConst1->eyePos);

	hr = this->psConst1.MapPixel(1);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed map pixel const 1 buffer.\n"));

	return hr;
}