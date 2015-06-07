#include"stdafx.hpp"
#include"GpuMeshParticleShader.hpp"
#include"../Function/ShaderFunction.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Api/DirectX11Shader.hpp"
#include"../../Api/DirectX11Resource.hpp"


using namespace ao;


GpuMeshParticleShader::GpuMeshParticleShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	RenderBaseShader(_pd3dDevice, _pd3dDeviceContext)
{
	this->Naming(_T("GpuMeshParticleShader"));
}


GpuMeshParticleShader::~GpuMeshParticleShader()
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
//	2014/08/20	09:24	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT GpuMeshParticleShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/GpuMeshParticle/GpuMeshParticleVertexShader.hlsl");
	this->pGeometryShader = sShader()->LoadGeometryShaderFromFile(L"hlsl/GpuMeshParticle/GpuMeshParticleGeometryShader.hlsl");
	this->pPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/GpuMeshParticle/GpuMeshParticlePixelShader.hlsl");
#ifdef __AO_DEBUG__
	if (this->pVertexShader == nullptr || this->pPixelShader == nullptr)
	{
		return E_FAIL;
	}
#endif

	this->csConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->gsConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psConst.Register(this->pd3dDevice, this->pd3dDeviceContext);


	HRESULT hr(E_FAIL);

	//create constant buffer
	hr = this->csConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->gsConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->psConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);


	this->pRenderer->GetProjMatrix(&this->projMatrix);
	ao::MatrixTranspose(&this->gsConst->projMatrix, this->projMatrix);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		GPUメッシュパーティクルインスタンス作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/01/19	04:02	T.Aoyama	作成
//------------------------------------------------------------------------------
GpuMeshParticle::Instance* GpuMeshParticleShader::Create(
	const UINT _numBuffer,
	const ao::StMesh* _pMesh,
	const ao::string _texPath,
	const ao::float3& _minPos,
	const ao::float3& _maxPos,
	const float& _minSize, const float& _maxSize,
	const ao::float4& _minColor,
	const ao::float4& _maxColor,
	const ao::float3& _minSpeed,
	const ao::float3& _maxSpeed,
	const float& _minLifeTime, const float _maxLifeTime)
{
	std::vector<GpuMeshParticle::InitLayout> initBuffer(_numBuffer);
	UINT index = 0;
	for (auto& it : initBuffer)
	{
		auto& v = _pMesh->geometry.vertex[index%_pMesh->geometry.vertex.size()];
		it.pos.x = ao::sMtRandom()->GetFloatRandom(_minPos.x, _maxPos.x) + v.x;
		it.pos.y = ao::sMtRandom()->GetFloatRandom(_minPos.y, _maxPos.y) + v.y;
		it.pos.z = ao::sMtRandom()->GetFloatRandom(_minPos.z, _maxPos.z) + v.z;

		it.color.r = ao::sMtRandom()->GetFloatRandom(_minColor.r, _maxColor.r);
		it.color.g = ao::sMtRandom()->GetFloatRandom(_minColor.g, _maxColor.g);
		it.color.b = ao::sMtRandom()->GetFloatRandom(_minColor.b, _maxColor.b);
		it.color.a = ao::sMtRandom()->GetFloatRandom(_minColor.a, _maxColor.a);

		it.speed.x = ao::sMtRandom()->GetFloatRandom(_minSpeed.x, _maxSpeed.x);
		it.speed.y = ao::sMtRandom()->GetFloatRandom(_minSpeed.y, _maxSpeed.y);
		it.speed.z = ao::sMtRandom()->GetFloatRandom(_minSpeed.z, _maxSpeed.z);

		it.maxLifeTime = ao::sMtRandom()->GetFloatRandom(_minLifeTime, _maxLifeTime);

		++index;
	}

	std::vector<GpuMeshParticle::UpdateLayout> updateBuffer(_numBuffer);
	for (UINT i = 0; i < updateBuffer.size(); ++i)
	{
		updateBuffer[i].nowLifeTime = 0;
		updateBuffer[i].size = ao::sMtRandom()->GetFloatRandom(_minSize, _maxSize);
	}

	//create vector field
	UINT vx(20), vy(20), vz(20);
	std::vector<ao::float3> vectorBuffer(vx*vy*vz);
	ao::float3 v;
	for (UINT i = 0; i < vx; ++i)
	{
		for (UINT j = 0; j < vy; ++j)
		{
			for (UINT k = 0; k < vz; ++k)
			{
				v.x = ao::sMtRandom()->GetFloatRandom(-1,+1);
				v.y = ao::sMtRandom()->GetFloatRandom(-1,+1);
				v.z = ao::sMtRandom()->GetFloatRandom(-1,+1);
				v.Normalize();
				int idx = k * vy * vx + j * vx + i;
				vectorBuffer[idx].x = v.x / 100.0f;
				vectorBuffer[idx].y = v.y / 100.0f;
				vectorBuffer[idx].z = v.z / 100.0f;
			}
		}
	}

	GpuMeshParticle::Instance* pInstance = new GpuMeshParticle::Instance;

	pInstance->numBuffer = _numBuffer;

	std::array<char, 16> maxBuffer, initStride, updateStride,xVec,yVec,zVec;
	sprintf_s(maxBuffer.data(), 16, "%d", _numBuffer);
	sprintf_s(initStride.data(), 16, "%d", sizeof(GpuMeshParticle::InitLayout));
	sprintf_s(updateStride.data(), 16, "%d", sizeof(GpuMeshParticle::UpdateLayout));
	sprintf_s(xVec.data(), 16, "%d", vx);
	sprintf_s(yVec.data(), 16, "%d", vy);
	sprintf_s(zVec.data(), 16, "%d", vz);
	D3D_SHADER_MACRO macros[] =
	{
		"MAX_BUFFER", maxBuffer.data(),
		"INIT_STRIDE", initStride.data(),
		"UPDATE_STRIDE", updateStride.data(),
		"X_VEC",xVec.data(),
		"Y_VEC",yVec.data(),
		"Z_VEC",zVec.data(),
		nullptr, nullptr
	};
	pInstance->pComputeShader = sShader()->LoadComputeShaderFromFile(
		L"hlsl/GpuMeshParticle/GpuMeshParticleComputeShader.hlsl",
		"main", "cs_5_0", nullptr, macros);

	ao::CreateUnorderedAccessBuffer(
		this->pd3dDevice.GetComPtr(),
		pInstance->initBuffer.buffer.ToCreator(),
		pInstance->initBuffer.view.ToCreator(),
		D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS,
		sizeof(GpuMeshParticle::InitLayout), _numBuffer,
		initBuffer.data(), D3D11_BIND_VERTEX_BUFFER);

	ao::CreateUnorderedAccessBuffer(
		this->pd3dDevice.GetComPtr(),
		pInstance->updateBuffer.buffer.ToCreator(),
		pInstance->updateBuffer.view.ToCreator(),
		D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS,
		sizeof(GpuMeshParticle::UpdateLayout), _numBuffer,
		updateBuffer.data(), D3D11_BIND_VERTEX_BUFFER);

	ao::CreateUnorderedAccessBuffer(
		this->pd3dDevice.GetComPtr(),
		pInstance->vectorField.buffer.ToCreator(),
		pInstance->vectorField.view.ToCreator(),
		D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS,
		sizeof(ao::float3), (UINT)vectorBuffer.size(),
		vectorBuffer.data());

	pInstance->pTexture = sResource()->LoadTextureFromFile(_texPath);

	this->instanceList.push_back(pInstance);
	return pInstance;
}


//------------------------------------------------------------------------------
/**	@brief		ビュー行列更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/20	10:56	T.Aoyama	作成
//------------------------------------------------------------------------------
void GpuMeshParticleShader::UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx)
{
	::CopyMemory(&this->viewMatrix, _pViewMtx, sizeof(XMFLOAT4X4));
	ao::MatrixMultiply(&this->viewProjMatrix, this->viewMatrix, this->projMatrix);
	ao::MatrixTranspose(&this->gsConst->viewProjMatrix, this->viewProjMatrix);
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/20	09:27	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT GpuMeshParticleShader::Render(
	GpuMeshParticle::Instance* _pInstance,
	const ao::float3& _offset,
	const ao::float4& _color,
	const BlendState _blendState,
	const FillState _fillState,
	const UINT _priority)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[GpuMeshParticleShader::Render]pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}
#endif

	//演算処理
	ID3D11UnorderedAccessView* nullBuffer[] = { nullptr, nullptr,nullptr };
	ID3D11UnorderedAccessView* setBuffer[] =
	{
		_pInstance->initBuffer.view.GetComPtr(),
		_pInstance->updateBuffer.view.GetComPtr(),
		_pInstance->vectorField.view.GetComPtr()
	};

	this->csConst->offsetPos.x = _offset.x;
	this->csConst->offsetPos.y = _offset.y;
	this->csConst->offsetPos.z = _offset.z;
	this->csConst->deltaTime = ao::sFpsCtrl()->GetDeltaTime();
	this->csConst.MapCompute();
	this->pd3dDeviceContext->CSSetUnorderedAccessViews(0, 3, setBuffer, nullptr);
	this->pd3dDeviceContext->CSSetShader(_pInstance->pComputeShader->shader.GetComPtr(), nullptr, 0);

	//TODO:
	this->pd3dDeviceContext->Dispatch((UINT)(_pInstance->numBuffer / 1000 + 1), 1, 1);

	this->pd3dDeviceContext->CSSetUnorderedAccessViews(0, 3, nullBuffer, nullptr);
	this->pd3dDeviceContext->CSSetShader(nullptr, nullptr, 0);

	COMMAND pushCommand;

	pushCommand.pInstance = _pInstance;

	//色設定
	::CopyMemory(pushCommand.color.f, _color.f, sizeof(pushCommand.color));

	//フィルステート
	pushCommand.fillState = _fillState;

	//ブレンドステート設定
	pushCommand.blendState = _blendState;

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
//	2014/08/20	09:26	T.Aoyama	作成
//------------------------------------------------------------------------------
void GpuMeshParticleShader::OnDraw()
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
	this->pd3dDeviceContext->IASetInputLayout(this->pVertexShader->layout.GetComPtr());
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
	this->pGraphicsState->SetDepthStecilState(TRUE, D3D11_DEPTH_WRITE_MASK_ZERO);

	this->pd3dDeviceContext->VSSetShader(this->pVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(this->pGeometryShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pPixelShader->shader.GetComPtr(), nullptr, 0);
	this->gsConst.MapGeometry();

	UINT offset = 0;
	UINT stride = sizeof(GpuMeshParticle::UpdateLayout);
	for (auto& it : this->commandList)
	{
		this->pd3dDeviceContext->IASetVertexBuffers(0, 1, it.pInstance->updateBuffer.buffer.GetComPtrPtr(), &stride, &offset);

		this->pGraphicsState->SetBlendState(it.blendState);
		this->pGraphicsState->SetRasterizerState(static_cast<D3D11_FILL_MODE>(it.fillState));
		::CopyMemory(this->psConst->color.f, it.color.f, sizeof(this->psConst->color));
		this->psConst.MapPixel();
		this->pRenderer->SetTexture(0, 0, it.pInstance->pTexture);
		this->pd3dDeviceContext->Draw((UINT)it.pInstance->numBuffer, 0);
	}


	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pGraphicsState->SetDepthStecilState(TRUE);

	this->isNeedSort = FALSE;
	this->commandList.clear();
}