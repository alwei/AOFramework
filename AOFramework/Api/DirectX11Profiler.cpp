#include"stdafx.hpp"
#include"DirectX11Profiler.hpp"
#include"DirectX11GraphicsRenderer.hpp"
#include"DirectX11GraphicsState.hpp"
#include"DirectX11RenderTarget.hpp"
#include"DirectX11Shader.hpp"
#include"../System/CountTime.hpp"
#include"../Math/MathDefine.hpp"
#include"../Shader/Function/ShaderFunction.hpp"


using namespace ao;


static const u8 MaxProcLoadType = static_cast<u8>(ProcessLoadType::MAX);


DirectX11Profiler::DirectX11Profiler(
	const u32 _width,
	const u32 _height,
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	renderWidth(_width), renderHeight(_height),
	recordCount(0),
	dispPosition(0,static_cast<f32>(_height)),
	dispScale(static_cast<f32>(_width),static_cast<f32>(_height/2)),
	graphColor(color::white),
	backColor(color::transparent),
	selectedProfileIndex(ProcessLoadType::RENDER_BURST),
	pShader(nullptr),pGraphicsRenderer(nullptr),pGraphicsState(nullptr),
	pVertexShader(nullptr), pGeometryShader(nullptr), pPixelShader(nullptr),
	DirectX11BaseApi(_pd3dDevice, _pd3dDeviceContext)
{
	//�^�C�}�[�������m��
	for (u8 i = 0; i < MaxProcLoadType; ++i)
	{
		this->pProcLoadTime[i] = new CountTime;
	}

	//�������ʗp�̃����_�����O�^�[�Q�b�g�쐬
	for (u8 i = 0; i < MaxProcLoadType; ++i)
	{
		this->pProcLoadRT[i] = new DirectX11RenderTarget(_pd3dDevice, _pd3dDeviceContext);
		this->pProcLoadRT[i]->Initialize();
		this->pProcLoadRT[i]->Create(_width, _height/2, DXGI_FORMAT_B8G8R8A8_UNORM);
	}
}


DirectX11Profiler::~DirectX11Profiler()
{
	for (u8 i = 0; i < MaxProcLoadType; ++i)
	{
		delete this->pProcLoadTime[i];
	}

	for (u8 i = 0; i < MaxProcLoadType; ++i)
	{
		delete this->pProcLoadRT[i];
	}
}


//------------------------------------------------------------------------------
/**	@brief		������ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/19	00:57	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11Profiler::Initialize(
	DirectX11GraphicsRenderer* const _pGraphicsRenderer,
	DirectX11GraphicsState* const _pGraphicsState,
	DirectX11Shader* const _pShader)
{
	this->pShader = _pShader;
	this->pGraphicsRenderer = _pGraphicsRenderer;
	this->pGraphicsState = _pGraphicsState;

	//�V�F�[�_�[�ǂݍ���
	this->pVertexShader = this->pShader->LoadVertexShaderFromFile(_T("hlsl/ProfilerGUI/ProfilerGUIVertexShader.hlsl"));
	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(this->pVertexShader == nullptr, E_FAIL, _T("Failed load vertex shader.\n"));


	std::array<char, 16> num;
	sprintf_s(num.data(), 16, "%d", this->renderWidth/4);
	std::array<D3D_SHADER_MACRO, 2> defines = { "RENDER_WIDTH", num.data(), nullptr,nullptr };
	this->pGeometryShader = this->pShader->LoadGeometryShaderFromFile(_T("hlsl/ProfilerGUI/ProfilerGUIGeometryShader.hlsl"), "main", "gs_5_0", nullptr, defines.data());
	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(this->pGeometryShader == nullptr, E_FAIL, _T("Failed load geometry shader.\n"));

	this->pPixelShader = this->pShader->LoadPixelShaderFromFile(_T("hlsl/ProfilerGUI/ProfilerGUIPixelShader.hlsl"));
	AO_CHECK_ERROR_USER_RETURN_TEMPLATE_MESSAGE(this->pPixelShader == nullptr, E_FAIL, _T("Failed load pixel shader.\n"));

	
	//�萔�o�b�t�@�쐬
	this->geomConstBuffer0.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->geomConstBuffer0.Create(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	this->geomConstBuffer0->x = static_cast<f32>(this->renderWidth);
	this->geomConstBuffer0->y = static_cast<f32>(this->renderHeight);
	this->geomConstBuffer0->z = 1.0f/60.0f;	//normalize data
	this->geomConstBuffer0->w = 1;		//rendering alpha


	//�v���t�@�C�����镪�����̒萔�f�[�^�o�b�t�@�쐬
	for (u8 i = 0; i < PROC_LOAD_MAX_TYPE; ++i)
	{
		this->geomConstBuffer1[i].Register(this->pd3dDevice, this->pd3dDeviceContext);
		this->geomConstBuffer1[i].Create(this->renderWidth);

		for (u32 j = 0; j < this->renderWidth; ++j)
		{
			this->geomConstBuffer1[i][j / 4].procLoad.f[j % 4] = 0;
		}
	}


	//�������p��CPU�o�b�t�@�쐬
	UINT* cpuBuffer = new UINT[this->renderWidth];
	for (UINT i = 0; i < this->renderWidth; ++i)
	{
		cpuBuffer[i] = i;
	}

	//�`��p�̒��_�o�b�t�@�쐬
	CreateVertexBuffer(this->pd3dDevice.GetComPtr(), this->pVertexBuffer.ToCreator(), sizeof(UINT)*this->renderWidth, cpuBuffer);
	delete[] cpuBuffer;

	//�L�^�J�E���g������
	this->recordCount = 0;

	//�\������v���Z�X�v���t�@�C�����o�[�X�g�����_�����O�ɐݒ�
	this->selectedProfileIndex = ProcessLoadType::RENDER_BURST;

	//���肷��t���[�����[�g��ݒ�
	this->useRecordFrameRate = 60;

	//�\��������W�̏�����
	this->dispPosition.x = 0;
	this->dispPosition.y = static_cast<f32>(renderHeight/2);

	//�\������T�C�Y�̏�����
	this->dispScale.x = static_cast<f32>(this->renderWidth);
	this->dispScale.y = static_cast<f32>(this->renderHeight/2);

	//�O���t�F�ݒ�
	this->graphColor = color::white;

	//�w�i�F�ݒ�
	this->backColor = color::transparent;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�v���t�@�C�����ʂ������_�����O */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/18	00:09	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11Profiler::RenderResult()
{
	//�S���׏��������_�����O
	const u8 index = static_cast<u8>(this->selectedProfileIndex);
	this->RenderProcessLoad(this->pProcLoadRT[index], this->pProcLoadTime[index], &this->geomConstBuffer1[index]);

	//render surface
	this->pGraphicsRenderer->RenderSprite(
		+this->dispPosition.x,
		-this->dispPosition.y,
		this->dispScale.x,
		this->dispScale.y,
		this->graphColor,
		this->pProcLoadRT[index]->GetRenderTexture(SamplerState::POINT));

	this->recordCount = (this->recordCount + 1) % this->renderWidth;
}


//------------------------------------------------------------------------------
/**	@brief		���׌��ʂ��O���t�Ń����_�����O */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/18	02:09	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11Profiler::RenderProcessLoad(DirectX11RenderTarget* const _pRenderTarget, CountTime* const _pProcTime, LoadProcBuffer* const pMapGeomBuffer1)
{
	_pRenderTarget->Clear(this->backColor.r, this->backColor.g, this->backColor.b, this->backColor.a);
	_pRenderTarget->SetRenderTarget(TRUE);

	std::array<UINT, 1> offset = { 0 };
	std::array<UINT, 1> stride = { sizeof(UINT) };
	std::array<ID3D11Buffer*, 1> pVB = { this->pVertexBuffer.GetComPtr() };

	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, pVB.data(), stride.data(), offset.data());
	this->pd3dDeviceContext->IASetInputLayout(this->pVertexShader->layout.GetComPtr());

	this->pd3dDeviceContext->VSSetShader(this->pVertexShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(this->pGeometryShader->shader.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->pPixelShader->shader.GetComPtr(), nullptr, 0);

	this->pGraphicsState->SetBlendState(BlendState::Alpha);
	this->pGraphicsState->SetDepthStecilState(FALSE, D3D11_DEPTH_WRITE_MASK_ZERO);

	//�萔MAP
	u32 index = this->recordCount;
	this->geomConstBuffer0->z = 1.0f / static_cast<f32>(this->useRecordFrameRate)*1000.0f;
	(*pMapGeomBuffer1)[index / 4].procLoad.f[index % 4] = _pProcTime->GetMilliSeconds();
	this->geomConstBuffer0.MapGeometry(1);
	pMapGeomBuffer1->MapGeometry(2);

	//���׃����_�����O
	this->pd3dDeviceContext->Draw(this->renderWidth, 0);

	_pRenderTarget->RestoreRenderTarget();
}