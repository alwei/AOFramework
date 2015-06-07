#include"stdafx.hpp"
#include"ShaderScene.hpp"
#include"../Shader/RenderUi/RenderUiShader.hpp"
#include"../Shader/iRenderUi/iRenderUiShader.hpp"
#include"../Shader/RenderMesh/RenderMeshShader.hpp"
#include"../Shader/iRenderMesh/iRenderMeshShader.hpp"
#include"../Shader/RenderMeshAnimation/RenderMeshAnimationShader.hpp"
#include"../Shader/RenderSprite/RenderSpriteShader.hpp"
#include"../Shader/iRenderSprite/iRenderSpriteShader.hpp"
#include"../Shader/ZPrePass/ZPrePassShader.hpp"
#include"../Shader/ExtractBrightness/ExtractBrightnessShader.hpp"
#include"../Shader/GaussianFilter/GaussianFilterShader.hpp"
#include"../Shader/LightBloom/LightBloomShader.hpp"
#include"../Shader/CrossFilter/CrossFilterShader.hpp"
#include"../Shader/FXAA/FXAAShader.hpp"
#include"../Shader/GpuMeshParticle/GpuMeshParticleShader.hpp"


using namespace ao;


ShaderScene::ShaderScene(SystemScene* _pSystemScene) : 
pRenderUiShader(nullptr),
pIRenderUiShader(nullptr),
pRenderMeshShader(nullptr),
pIRenderMeshShader(nullptr),
pRenderMeshAnimationShader(nullptr),
pRenderSpriteShader(nullptr),
pIRenderSpriteShader(nullptr),
pRenderDepthShader(nullptr),
pExtractBrightnessShader(nullptr),
pGaussianFilterShader(nullptr),
pLightBloomShader(nullptr),
pCrossFilterShader(nullptr),
pFxaaShader(nullptr),
pGpuMeshParticleShader(nullptr),
SystemComposite(_pSystemScene)
{

}


ShaderScene::~ShaderScene()
{
	delete this->pGpuMeshParticleShader;
	delete this->pFxaaShader;
	delete this->pCrossFilterShader;
	delete this->pLightBloomShader;
	delete this->pGaussianFilterShader;
	delete this->pExtractBrightnessShader;
	delete this->pRenderDepthShader;
	delete this->pIRenderSpriteShader;
	delete this->pRenderSpriteShader;
	delete this->pRenderMeshAnimationShader;
	delete this->pIRenderMeshShader;
	delete this->pRenderMeshShader;
	delete this->pIRenderUiShader;
	delete this->pRenderUiShader;
}


//------------------------------------------------------------------------------
/**	@brief		èâä˙âª */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/02/28	16:05	T.Aoyama	çÏê¨
//------------------------------------------------------------------------------
HRESULT ShaderScene::Initialize(
	ao::SmartComPtr<ID3D11Device> _pd3dDevice,
	ao::SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext,
	ao::DirectX11GraphicsState* _pGraphicsState,
	ao::DirectX11GraphicsRenderer* _pRenderer)
{
	HRESULT hr(E_FAIL);

	//allocate shader class

	this->pRenderUiShader = new RenderUiShader(_pd3dDevice, _pd3dDeviceContext);
	this->pIRenderUiShader = new iRenderUiShader(_pd3dDevice, _pd3dDeviceContext);
	this->pIRenderMeshShader = new iRenderMeshShader(_pd3dDevice, _pd3dDeviceContext);
	this->pRenderMeshShader = new RenderMeshShader(_pd3dDevice, _pd3dDeviceContext);
	this->pRenderMeshAnimationShader = new RenderMeshAnimationShader(_pd3dDevice, _pd3dDeviceContext);
	this->pRenderSpriteShader = new RenderSpriteShader(_pd3dDevice, _pd3dDeviceContext);
	this->pIRenderSpriteShader = new iRenderSpriteShader(_pd3dDevice, _pd3dDeviceContext);
	this->pRenderDepthShader = new ZPrePassShader(_pd3dDevice, _pd3dDeviceContext);
	this->pExtractBrightnessShader = new ExtractBrightnessShader(_pd3dDevice, _pd3dDeviceContext);
	this->pGaussianFilterShader = new GaussianFilterShader(_pd3dDevice, _pd3dDeviceContext);
	this->pLightBloomShader = new LightBloomShader(_pd3dDevice, _pd3dDeviceContext);
	this->pCrossFilterShader = new CrossFilterShader(_pd3dDevice, _pd3dDeviceContext);
	this->pFxaaShader = new FXAAShader(_pd3dDevice, _pd3dDeviceContext);
	this->pGpuMeshParticleShader = new GpuMeshParticleShader(_pd3dDevice, _pd3dDeviceContext);

	hr = this->pRenderUiShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pIRenderUiShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pRenderMeshShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pIRenderMeshShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pRenderMeshAnimationShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pRenderSpriteShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pIRenderSpriteShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pRenderDepthShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pExtractBrightnessShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pGaussianFilterShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pLightBloomShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pCrossFilterShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pFxaaShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->pGpuMeshParticleShader->Initialize(_pGraphicsState, _pRenderer);
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	return hr;
}