#include"stdafx.hpp"
#include"iRenderUiShader.hpp"
#include"../Function/ShaderFunction.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Api/DirectX11Shader.hpp"

using namespace ao;


iRenderUiShader::iRenderUiShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	RenderBaseShader(_pd3dDevice,_pd3dDeviceContext)
{
	this->Naming(_T("iRenderUiShader"));
}


iRenderUiShader::~iRenderUiShader()
{
	for (auto& it : this->pUiInstanceList)
	{
		delete it;
	}

	this->pUiInstanceList.clear();
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	12:10	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iRenderUiShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/iRenderUi/iRenderUiPixelShader.hlsl");
#ifdef __AO_DEBUG__
	if (this->pPixelShader == nullptr)
	{
		return E_FAIL;
	}
#endif

	//正射影行列のGPUリソース作成
	this->orthoMatrix.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->orthoMatrix.Create(D3D11_USAGE_DYNAMIC);

	//正射影行列作成
	UINT num = 1;
	D3D11_VIEWPORT viewport;
	this->pd3dDeviceContext->RSGetViewports(&num, &viewport);

	XMFLOAT4X4 mtx;
	ao::MatrixOrthographicLH(&mtx, viewport.Width, viewport.Height, 0, 1);
	ao::MatrixTranspose(&this->orthoMatrix, mtx);

	//各種パラメーター初期化
	this->isNeedSort = FALSE;
	this->commandList.reserve(100);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		バッファ作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/02/11	15:21	T.Aoyama	作成
//------------------------------------------------------------------------------
UiInstance* iRenderUiShader::CreateInstance(
	BaseTexture* _pTexture,
	UINT _numAllocate)
{
	UiInstance* pInstance = new UiInstance();

	pInstance->numPushRender = 0;
	pInstance->numAllocatedBuffer = _numAllocate;
	pInstance->pTexture = _pTexture;
	pInstance->blendState = ao::BlendState::Alpha;
	pInstance->fillState = ao::FillState::Solid;
	pInstance->priority = 0xCCCCCCC;

	pInstance->vsConst0.Register(this->pd3dDevice, this->pd3dDeviceContext);
	pInstance->vsConst1.Register(this->pd3dDevice, this->pd3dDeviceContext);
	pInstance->vsConst2.Register(this->pd3dDevice, this->pd3dDeviceContext);
	pInstance->vsConst0.Create(_numAllocate / 4 + 1);
	pInstance->vsConst1.Create(_numAllocate);
	pInstance->vsConst2.Create(_numAllocate);


	//シェーダーの定数定義
	std::array<char, 16> numInst, numDegree;
	sprintf_s(numInst.data(), 16, "%d", _numAllocate);
	sprintf_s(numDegree.data(), 16, "%d", _numAllocate / 4 + 1);
	D3D_SHADER_MACRO macros[] = { "NUM_INST", numInst.data(), "NUM_DEGREE", numDegree.data(), nullptr, nullptr };

	//定数バッファを動的にマクロ定義して頂点シェーダーを作成する
	pInstance->pVertexShader = sShader()->LoadVertexShaderFromFile(
		L"hlsl/iRenderUi/iRenderUiVertexShader.hlsl",
		"main", "vs_5_0", D3D_COMPILE_STANDARD_FILE_INCLUDE, macros);

	//頂点データにはIDを振り分ける
	std::vector<int> initData(_numAllocate*4);
	for (UINT i = 0; i < initData.size(); ++i)
	{
		initData[i] = i;
	}

	ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		pInstance->vertexBuffer.ToCreator(),
		(UINT)(sizeof(int)*initData.size()),
		initData.data());

	this->pUiInstanceList.push_back(pInstance);

	return pInstance;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリングコマンド登録 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	12:10	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iRenderUiShader::PushRenderCommand(
	UiInstance* _pInstance,
	const int _x,
	const int _y,
	const float _w,
	const float _h,
	const float2& _lt,
	const float2& _rt,
	const float2& _lb,
	const float2& _rb,
	const float _degree,
	const FLOAT _r,
	const FLOAT _g,
	const FLOAT _b,
	const FLOAT _a,
	UINT _priority)
{
#ifdef __AO_DEBUG__
	if (_pInstance->numPushRender >= _pInstance->numAllocatedBuffer)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[iRenderUiShader::PushRenderCommand]Buffer overflow or not allocated buffer.\n"));
		return E_FAIL;
	}
#endif

	iUiVsConst0& vsConst0 = _pInstance->vsConst0[_pInstance->numPushRender / 4];
	iUiVsConst1& vsConst1 = _pInstance->vsConst1[_pInstance->numPushRender];
	iUiVsConst2& vsConst2 = _pInstance->vsConst2[_pInstance->numPushRender];

	vsConst0.degree[_pInstance->numPushRender % 4] = _degree;

	vsConst1.uv[0] = _lt;
	vsConst1.uv[1] = _rt;
	vsConst1.uv[2] = _lb;
	vsConst1.uv[3] = _rb;

	vsConst2.pos.x = static_cast<float>(_x);
	vsConst2.pos.y = static_cast<float>(_y);
	vsConst2.size.x = _w;
	vsConst2.size.y = _h;
	vsConst2.color.r = _r;
	vsConst2.color.g = _g;
	vsConst2.color.b = _b;
	vsConst2.color.a = _a;

	if (_priority != 0xCCCCCCC)
	{
		this->isNeedSort = TRUE;
	}

	++_pInstance->numPushRender;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		レンダリングコマンドを実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	12:10	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iRenderUiShader::ExecuteCommandList(
	UiInstance* _pInstance,
	BlendState _blendState,
	FillState _fillState)
{
#ifdef __AO_DEBUG__
	if (this->pd3dDeviceContext == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[iRenderUiShader::ExecuteCommandList]pd3dDeviceContext is null.\n"));
		return E_FAIL;
	}

	if (_pInstance->numAllocatedBuffer == 0)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[iRenderUiShader::ExecuteCommandList]Not allocated buffer.\n"));
		return E_FAIL;
	}
#endif

	_pInstance->blendState = _blendState;
	_pInstance->fillState = _fillState;
	_pInstance->numCommitRender = _pInstance->numPushRender;
	_pInstance->numPushRender = 0;

	this->commandList.push_back(_pInstance);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		描画コマンド実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/29	12:10	T.Aoyama	作成
//------------------------------------------------------------------------------
void iRenderUiShader::OnDraw()
{
	// to sort sort if necessary
	if (this->isNeedSort)
	{
		//ラムダ式によるプレディケート
		auto Predicate = [](const UiInstance* inst1, const UiInstance* inst2)
		{
			return (inst1->priority < inst2->priority);
		};
		std::sort(this->commandList.begin(), this->commandList.end(), Predicate);
	}

	//正射影行列MAPとピクセルシェーダー設定
	this->orthoMatrix.MapVertex(3);
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pPixelShader->shader.GetComPtr(), nullptr, 0);

	//プリミティブタイプと深度ステート
	UINT stride = 4, offset = 0;
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->pGraphicsState->SetDepthStecilState(FALSE);

	//レンダリング部分
	for (auto& it : this->commandList)
	{
		this->pGraphicsState->SetBlendState(it->blendState);
		this->pGraphicsState->SetRasterizerState(static_cast<D3D11_FILL_MODE>(it->fillState));

		this->pd3dDeviceContext->VSSetShader(it->pVertexShader->shader.GetComPtr(), nullptr, 0);
		this->pd3dDeviceContext->IASetInputLayout(it->pVertexShader->layout.GetComPtr());
		this->pd3dDeviceContext->IASetVertexBuffers(0, 1, it->vertexBuffer.GetComPtrPtr(), &stride, &offset);

		it->vsConst0.MapVertex(0);
		it->vsConst1.MapVertex(1);
		it->vsConst2.MapVertex(2);

		this->pRenderer->SetTexture(0, 0, it->pTexture);
		this->pd3dDeviceContext->Draw(it->numCommitRender * 4, 0);
		it->numCommitRender = 0;
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
void iRenderUiShader::Clear()
{
	this->commandList.clear();
}


//------------------------------------------------------------------------------
/**	@brief		破棄処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/02/12	10:02	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iRenderUiShader::Destory(UiInstance* _pInstance)
{
	for (auto it = this->pUiInstanceList.begin(); it != this->pUiInstanceList.end(); ++it)
	{
		if (*it == _pInstance)
		{
			this->pUiInstanceList.erase(it);
			delete _pInstance;
			return S_OK;
		}
	}

#ifdef __AO_DEBUG__
	ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[iRenderUiShader::Destory]Not found ui instance.[adr : %x]\n"), _pInstance);
#endif

	return S_OK;
}