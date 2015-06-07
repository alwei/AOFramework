#include"stdafx.hpp"
#include"../Api/DirectX11Application.hpp"
#include"../Api/DirectX11Resource.hpp"
#include"../Api/DirectX11Shader.hpp"
#include"../Api/DirectX11GraphicsRenderer.hpp"
#include"../Api/DirectX11GraphicsState.hpp"
#include"../Api/DirectX11DebugFont.hpp"
#include"../Api/DirectX11Profiler.hpp"
#include"ApiScene.hpp"


using namespace ao;


ApiScene::ApiScene(SystemScene* _pSystemScene) :
pResource(nullptr),
pShader(nullptr),
pRenderer(nullptr),
pGraphicsState(nullptr),
pDebugFont(nullptr),
SystemComposite(_pSystemScene)
{

}


ApiScene::~ApiScene()
{
	delete this->pProfiler;
	delete this->pDebugFont;
	delete this->pGraphicsState;
	delete this->pRenderer;
	delete this->pShader;
	delete this->pResource;
}


//------------------------------------------------------------------------------
/**	@brief		‰Šú‰» */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/02/28	15:45	T.Aoyama	ì¬
//------------------------------------------------------------------------------
HRESULT ApiScene::Initialize(
	CONST HWND _hWnd,
	CONST UINT _wResolution,
	CONST UINT _hResolution,
	CONST UINT _nWidth,
	CONST UINT _nHeight,
	CONST DXGI_FORMAT _renderFormat,
	CONST BOOL _isFullScreen)
{
	HRESULT hr(E_FAIL);

	hr = this->application.InitDisplay(_renderFormat);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->application.InitDirect3D(_hWnd, _wResolution, _hResolution, _renderFormat, !_isFullScreen);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	SmartComPtr<ID3D11Device> pd3dDevice = this->application.GetDevice();
	SmartComPtr<ID3D11DeviceContext> pd3dDeviceContext = this->application.GetDeviceContext();
	SmartComPtr<IDXGISwapChain> pSwapChain = this->application.GetSwapChain();

	this->pResource = new DirectX11Resource(pd3dDevice, pd3dDeviceContext);
	this->pShader = new DirectX11Shader(pd3dDevice, pd3dDeviceContext);
	this->pRenderer = new DirectX11GraphicsRenderer(pd3dDevice, pd3dDeviceContext);
	this->pGraphicsState = new DirectX11GraphicsState(pd3dDevice, pd3dDeviceContext);
	this->pDebugFont = new DirectX11DebugFont(pd3dDevice, pd3dDeviceContext);
	this->pProfiler = new DirectX11Profiler(_wResolution,_hResolution,pd3dDevice, pd3dDeviceContext);

	hr = this->pResource->Initialize();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pRenderer->Initialize(
		pSwapChain,
		this->application.GetBackBuffer(),
		this->pGraphicsState);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pDebugFont->Initialize(
		_nWidth, _nHeight,
		this->pGraphicsState, this->pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pProfiler->Initialize(this->pRenderer,this->pGraphicsState, this->pShader);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	return hr;
}