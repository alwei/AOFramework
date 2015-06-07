#include"stdafx.hpp"
#include"GaussianFilterShader.hpp"
#include"../Function/ShaderFunction.hpp"
#include"../../Define/SingletonTypeDefine.inl"


using namespace ao;

namespace
{
	struct SUFVERTEX
	{
		float pos[3];
		float uv[2];
	};

	static 	SUFVERTEX	surface[3] =
	{
		{ -1.f, 1.f, 0.f, 0.f, 0.f },
		{ 3.f, 1.f, 0.f, 2.f, 0.f },
		{ -1.f, -3.f, 0.f, 0.f, 2.f }
	};
}


GaussianFilterShader::GaussianFilterShader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	RenderBaseShader(_pd3dDevice,_pd3dDeviceContext)
{
	this->renderCommand.isActive = FALSE;
	this->Naming(_T("GaussianFilter"));

	this->pBlurRenderTarget[0] = new DirectX11RenderTarget(_pd3dDevice, _pd3dDeviceContext);
	this->pBlurRenderTarget[1] = new DirectX11RenderTarget(_pd3dDevice, _pd3dDeviceContext);
}


GaussianFilterShader::~GaussianFilterShader()
{
	delete this->pBlurRenderTarget[1];
	delete this->pBlurRenderTarget[0];
}


//------------------------------------------------------------------------------
/**	@brief		������ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	14:09	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT GaussianFilterShader::Initialize(
	DirectX11GraphicsState* _pGraphicsState,
	DirectX11GraphicsRenderer* _pRenderer)
{
	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	this->pXBlurVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/GaussianFilter/XBlurVertexShader.hlsl");
	this->pXBlurPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/GaussianFilter/XBlurPixelShader.hlsl");
	this->pYBlurVertexShader = sShader()->LoadVertexShaderFromFile(L"hlsl/GaussianFilter/YBlurVertexShader.hlsl");
	this->pYBlurPixelShader = sShader()->LoadPixelShaderFromFile(L"hlsl/GaussianFilter/YBlurPixelShader.hlsl");


	HRESULT hr(E_FAIL);

	hr = this->pBlurRenderTarget[0]->Initialize();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed Initialize blur render target 0 index.\n"));

	hr = this->pBlurRenderTarget[1]->Initialize();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed Initialize blur render target 1 index.\n"));


	this->vsConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psConst.Register(this->pd3dDevice, this->pd3dDeviceContext);

	hr = this->vsConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	hr = this->psConst.Create();
	AO_CHECK_ERROR_RETURN_HRESULT(hr);

	this->CreateSurface();

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�����_�[�^�[�Q�b�g�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	14:09	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT GaussianFilterShader::Create(
	CONST UINT _nBlurWidth,
	CONST UINT _nBlurHeight,
	CONST DXGI_FORMAT _blurFormat)
{
	HRESULT hr(S_OK);

	//�k���o�b�t�@�̃T�C�Y�L��
	this->vsConst->width = 1.0f / ((float)_nBlurWidth);
	this->vsConst->height = 1.0f / ((float)_nBlurHeight);

	//16�s�N�Z���P�ʂłڂ���
	this->psConst->offset[0] = 16.0f / _nBlurWidth;
	this->psConst->offset[1] = 16.0f / _nBlurHeight;

	hr = this->pBlurRenderTarget[0]->Create(_nBlurWidth, _nBlurHeight, _blurFormat, 0, 1, TRUE, DXGI_FORMAT_R16_TYPELESS);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create blur render target 0 index.\n"));

	hr = this->pBlurRenderTarget[1]->Create(_nBlurWidth, _nBlurHeight, _blurFormat, 0, 1, TRUE, DXGI_FORMAT_R16_TYPELESS);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create blur render target 1 index.\n"));

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�����_�����O���� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/09/04	16:14	T.Aoyama	�쐬
//	2913/10/09	23:48	T.Aoyama	������_ppHdrSRView��ǉ�
//------------------------------------------------------------------------------
HRESULT GaussianFilterShader::Render(
	BaseTexture* _pTexture,
	FLOAT _dispersion,
	FLOAT _mulPower)
{
#ifdef __AO_DEBUG__
	if (_pTexture == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[GaussianFilterShader::Render]_pTexture argument is null.\n"));
		return E_INVALIDARG;
	}
#endif

	if (this->renderCommand.isActive)
	{
		return S_FALSE;
	}

	this->renderCommand.isActive = TRUE;
	this->renderCommand.dispersion = _dispersion;
	this->renderCommand.mulPower = _mulPower;
	this->renderCommand.ppSrView = _pTexture->pSrView.GetComPtrPtr();
	this->renderCommand.ppSampState = _pTexture->pSampState.GetComPtrPtr();

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�`����s */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	16:58	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void GaussianFilterShader::OnDraw()
{
	if (!this->renderCommand.isActive)
	{
		return;
	}

	const auto& it = this->renderCommand;

	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);

	UINT offset = 0;
	UINT stride = sizeof(::SUFVERTEX);
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->pSurfaceBuffer.GetComPtrPtr(), &stride, &offset);

	this->pGraphicsState->SetBlendState(BlendState::Default);
	this->pGraphicsState->SetDepthStecilState(FALSE, D3D11_DEPTH_WRITE_MASK_ZERO);
	this->UpdateMap(it.dispersion, it.mulPower);

	// x blur
	this->pd3dDeviceContext->VSSetShader(this->pXBlurVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pXBlurPixelShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pXBlurVertexShader->layout.GetComPtr());

	this->pBlurRenderTarget[0]->SetRenderTarget(TRUE, TRUE);
	this->pRenderer->SetTexture(0, 0, it.ppSrView, it.ppSampState, TRUE);
	this->pd3dDeviceContext->Draw(3, 0);
	this->pRenderer->SetTexture(0, 0, nullptr, TRUE, TRUE);
	this->pBlurRenderTarget[0]->RestoreRenderTarget();

	// y blur
	this->pd3dDeviceContext->VSSetShader(this->pYBlurVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pYBlurPixelShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->IASetInputLayout(this->pYBlurVertexShader->layout.GetComPtr());

	this->pBlurRenderTarget[1]->SetRenderTarget(TRUE, TRUE);
	BaseTexture* pTexture = this->pBlurRenderTarget[0]->GetRenderTexture();
	this->pRenderer->SetTexture(0, 0, pTexture, TRUE, TRUE);
	this->pd3dDeviceContext->Draw(3, 0);
	this->pRenderer->SetTexture(0, 0, nullptr, TRUE, TRUE);
	this->pBlurRenderTarget[1]->RestoreRenderTarget();

	this->renderCommand.isActive = FALSE;
	this->pGraphicsState->SetDepthStecilState(TRUE);
}


//------------------------------------------------------------------------------
/**	@brief		�����_�[�^�[�Q�b�g�̃N���A */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	17:18	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void GaussianFilterShader::Clear(float _r, float _g, float _b, float _a)
{
	this->pBlurRenderTarget[0]->Clear(_r, _g, _b, _a);
	this->pBlurRenderTarget[1]->Clear(_r, _g, _b, _a);
}


//------------------------------------------------------------------------------
/**	@brief		�T�[�t�F�C�X�̍쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	17:07	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	HRESULT GaussianFilterShader::CreateSurface()
{
	return ::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		this->pSurfaceBuffer.ToCreator(),
		sizeof(surface),
		surface);
}


//------------------------------------------------------------------------------
/**	@brief		�萔�̍X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	17:04	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	void GaussianFilterShader::UpdateMap(float _dispersion, float _mulPower)
{
	CONST UINT division = 8;

	float total = 0.0f;
	float weight[division];

	//�K�E�X�֐��ɂ��d�݌v�Z
	for (int i = 0; i < division; ++i)
	{
		float pos = (float)i*2.0f;
		weight[i] = expf(-pos * pos * _dispersion);
		total += weight[i];
	}

	//�d�݂̋K�i��
	for (int i = 0; i < division; ++i)
	{
		weight[i] = weight[i] / total * 0.5f;
	}

	this->psConst->power[0] = _mulPower;
	this->psConst->power[1] = 0;		//dummy

	//�d�݂̃R�s�[
	::CopyMemory(this->psConst->weight, weight, sizeof(float)*division);

	this->vsConst.MapVertex(0);
	this->psConst.MapPixel(0);
}