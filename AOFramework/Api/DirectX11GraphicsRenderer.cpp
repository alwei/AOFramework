#include"stdafx.hpp"
#include"DirectX11GraphicsRenderer.hpp"
#include"../Define/SingletonTypeDefine.inl"
#include"../Resource/BaseGeometry.hpp"
#include"../Shader/Function/ShaderFunction.hpp"
#include"../Math/Math.hpp"
#include"../Window/Window.hpp"
#include"../Api/DirectX11GraphicsState.hpp"


using namespace ao;
using namespace DirectX;


struct TestSprite
{
	float3 pos;
};



//�O���b�h�\���p�̒��_�V�F�[�_�[
static const char GridVertexShader[] =
{
	"cbuffer cbMatrixWVP : register( b0 )               "
	"{                                                  "
	"	float4x4	matWVP;								"
	"};                                                 "
	"struct VSInput										"
	"{                                                  "
	"	float3 pos		: POSITION;                     "
	"	float4 col		: COLOR;						"
	"};                                                 "
	"struct VSOutput									"
	"{                                                  "
	"	float4 pos		: SV_POSITION;                  "
	"	float4 col		: TEXCOORD;						"
	"};                                                 "
	"VSOutput	main(VSInput In)						"
	"{                                                  "
	"	VSOutput Out;									"
	"	Out.pos		= mul(float4(In.pos,1),matWVP);		"
	"	Out.col		= In.col;							"
	"	return Out;                                     "
	"}                                                  "
};

//�O���b�h�\���p�̃s�N�Z���V�F�[�_�[
static const char GridPixelShader[] =
{
	"cbuffer cbColor : register( b0 )           "
	"{                                          "
	"	float4 color;                           "
	"}                                          "
	"struct VSOutput                            "
	"{                                          "
	"	float4 pos		: SV_POSITION;          "
	"	float4 col		: TEXCOORD;				"
	"};                                         "
	"float4	main(VSOutput In) : SV_TARGET		"
	"{											"
	"	return color * In.col;			        "
	"}                                          "
};


DirectX11GraphicsRenderer::DirectX11GraphicsRenderer(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
isNeedSortRenderList(FALSE),
isNeedSortPreRenderList(FALSE),
isNeedSortPostRenderList(FALSE),
DirectX11BaseApi(_pd3dDevice,_pd3dDeviceContext)
{
	this->logSrViewBuffer.fill(-1);
	this->logSamplerBuffer.fill(-1);
	this->logVertexSrViewBuffer.fill(-1);
	this->logVertexSamplerBuffer.fill(-1);

	this->spriteConstBuffer.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->spriteConstBuffer.Create(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
}

DirectX11GraphicsRenderer::~DirectX11GraphicsRenderer()
{

}


//------------------------------------------------------------------------------
/**	@brief		������ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	04:47	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsRenderer::Initialize(
	SmartComPtr<IDXGISwapChain> _pSwapChain,
	DirectX11RenderTarget* _pBackBuffer,
	DirectX11GraphicsState* _pGraphicsState)
{
	this->pSwapChain = _pSwapChain;
	this->pRenderTarget = _pBackBuffer;
	this->pGraphicsState = _pGraphicsState;

	//null�p�̃e�N�X�`���쐬
	this->CreateNullTexture();
	this->CreateNullSamplerState();


	//�ȈՃe�N�X�`�������_�����O�V�F�[�_�[�ǂݍ���
	this->LoadShaderFile(
		_T("hlsl/RendererShader/RendererSpriteVertexShader.hlsl"),
		_T("hlsl/RendererShader/RendererSpritePixelShader.hlsl"),
		&this->spriteShader);

	std::array<UINT, 4> tempBuffer = {0,1,2,3};

	//�ȈՃX�v���C�g�����_�����O�p�̒��_�o�b�t�@�쐬
	CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		this->pSpriteVertexBuffer.ToCreator(),
		sizeof(UINT) * 4,
		tempBuffer.data());


	//�O���b�h�V�F�[�_�[�쐬
	ao::CreateVertexShader(
		this->pd3dDevice.GetComPtr(),
		this->gridShader.vertex.ToCreator(),
		this->gridShader.inputLayout.ToCreator(),
		(void*)GridVertexShader,
		sizeof(GridVertexShader));

	ao::CreatePixelShader(
		this->pd3dDevice.GetComPtr(),
		this->gridShader.pixel.ToCreator(),
		(void*)GridPixelShader,
		sizeof(GridPixelShader));

	//���p�̒��_�o�b�t�@�쐬
	ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		this->axisBuffer.buffer.ToCreator(),
		sizeof(AxisBuffer<2>::Vertex) * 2,
		nullptr,
		D3D11_USAGE_DYNAMIC,
		D3D11_CPU_ACCESS_WRITE);

	//�p���p�̒��_�o�b�t�@�쐬
	ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		this->poseBuffer.buffer.ToCreator(),
		sizeof(AxisBuffer<6>::Vertex) * 6,
		nullptr,
		D3D11_USAGE_DYNAMIC,
		D3D11_CPU_ACCESS_WRITE);

	//�O���b�h�p�̒��_�o�b�t�@�쐬
	ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		this->xGridBuffer.buffer.ToCreator(),
		sizeof(GridBuffer::Vertex)*MAX_GRID,
		nullptr,
		D3D11_USAGE_DYNAMIC,
		D3D11_CPU_ACCESS_WRITE);

	ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		this->zGridBuffer.buffer.ToCreator(),
		sizeof(GridBuffer::Vertex)*MAX_GRID,
		nullptr,
		D3D11_USAGE_DYNAMIC,
		D3D11_CPU_ACCESS_WRITE);

	//�ˉe�s�񏉊���
	D3D11_VIEWPORT viewport;
	_pBackBuffer->GetViewport(&viewport);
	f32 width = viewport.Width;
	f32 height = viewport.Height;
	projection.fovAngleY = ao::ToRadian(45);
	projection.aspect = width / height;
	projection.nearZ = 1.0f;
	projection.farZ = 1000;

	//���ˉe�s��쐬
	ao::MatrixOrthographicLH(&this->centerOrthoMatrix, width, height, 0, 1);
	ao::MatrixOrthographicLH(&this->leftTopOrthoMatrix, width, height, 0, 1);
	this->leftTopOrthoMatrix._41 = 0 - 1.0f;
	this->leftTopOrthoMatrix._42 = 0 + 1.0f;

	//�ˉe�s��쐬
	ao::MatrixPerspectiveFovLH(&this->projMatrix, this->projection.fovAngleY, this->projection.aspect, this->projection.nearZ, this->projection.farZ);

	//�萔�o�b�t�@�쐬
	ao::CreateConstantBuffer(
		this->pd3dDevice.GetComPtr(),
		this->gridConstBuffer.vsBuffer.ToCreator(),
		sizeof(GridConstBuffer::VsConst));

	ao::CreateConstantBuffer(
		this->pd3dDevice.GetComPtr(),
		this->gridConstBuffer.psBuffer.ToCreator(),
		sizeof(GridConstBuffer::PsConst));


	//reserve render node
	this->renderList.reserve(1000);
	this->preRenderList.reserve(1000);
	this->postRenderList.reserve(1000);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�����_�����O�J�n */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/11	01:16	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::RenderBegin(float _r, float _g, float _b, float _a)
{
	this->pRenderTarget->Clear(_r, _g, _b, _a);
	this->pGraphicsState->SetRasterizerState();
	this->pGraphicsState->SetDepthStecilState(TRUE);
	this->pGraphicsState->SetBlendState(ao::BlendState::Alpha);
}


//------------------------------------------------------------------------------
/**	@brief		�����_�����O�I�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/11	01:46	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::RenderEnd()
{
	this->pSwapChain->Present(1, 0);
}


//------------------------------------------------------------------------------
/**	@brief		�e�N�X�`���ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	11:20	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::SetTexture(
	int _srSlot,
	int _sampSlot,
	ID3D11ShaderResourceView** _ppSrView,
	ID3D11SamplerState** _ppSampler,
	BOOL _isCache,
	BOOL _isForceBind)
{
	ID3D11SamplerState** pSetSampler = (_ppSampler) ? _ppSampler : this->nullTexture[_srSlot].pSampState.GetComPtrPtr();
	int checkLogSampState = (int)((_ppSampler) ? (*_ppSampler) : this->nullTexture[_srSlot].pSampState.GetComPtr());

	if (this->logSamplerBuffer[_sampSlot] != checkLogSampState || _isForceBind)
	{
		this->pd3dDeviceContext->PSSetSamplers(_sampSlot, 1, pSetSampler);
		if (_isCache)
		{
			this->logSamplerBuffer[_sampSlot] = checkLogSampState;
		}
	}

	ID3D11ShaderResourceView** pSetTexture = (_ppSrView) ? _ppSrView : this->nullTexture[_srSlot].pSrView.GetComPtrPtr();
	int checkLogSrView = (int)((_ppSrView) ? (*_ppSrView) : this->nullTexture[_srSlot].pSrView.GetComPtr());

	if (this->logSrViewBuffer[_srSlot] != checkLogSrView || _isForceBind)
	{
		this->pd3dDeviceContext->PSSetShaderResources(_srSlot, 1, pSetTexture);
		if (_isCache)
		{
			this->logSrViewBuffer[_srSlot] = checkLogSrView;
		}
	}
}


//------------------------------------------------------------------------------
/**	@brief		�e�N�X�`���ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/16	23:34	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::SetTexture(
	int _srSlot,
	int _sampSlot,
	BaseTexture* _pTexture,
	BOOL _isCache,
	BOOL _isForceBind)
{
	BaseTexture* pUseTexture = (_pTexture) ? _pTexture : &this->nullTexture[_srSlot];


	ao_intptr sampPtr = (ao_intptr)pUseTexture->pSampState.GetComPtr();
	if ( (this->logSamplerBuffer[_sampSlot] != sampPtr && sampPtr) || _isForceBind)
	{
		this->pd3dDeviceContext->PSSetSamplers(_sampSlot, 1, pUseTexture->pSampState.GetComPtrPtr());
		if (_isCache)
		{
			this->logSamplerBuffer[_sampSlot] = sampPtr;
		}
	}

	ao_intptr texPtr = (ao_intptr)pUseTexture->pSrView.GetComPtr();
	if ( (this->logSrViewBuffer[_srSlot] != texPtr && texPtr) || _isForceBind)
	{
		this->pd3dDeviceContext->PSSetShaderResources(_srSlot, 1, pUseTexture->pSrView.GetComPtrPtr());
		if (_isCache)
		{
			this->logSrViewBuffer[_srSlot] = texPtr;
		}
	}
}


//------------------------------------------------------------------------------
/**	@brief		�e�N�X�`���ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/20	10:21	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::SetTexture(
	int _num,
	int _srSlotStart,
	int _sampSlotStart,
	BaseTexture** _pTextures)
{
	std::array<ID3D11SamplerState*, MAX_TEXTURE> sampler;
	std::array<ID3D11ShaderResourceView*, MAX_TEXTURE> srViews;
	sampler.fill(nullptr);
	srViews.fill(nullptr);
	for (int i = 0; i < _num; ++i)
	{
		if (_pTextures[i])
		{
			sampler[i] = _pTextures[i]->pSampState.GetComPtr();
			this->logSamplerBuffer[i] = (int)sampler[i];
			srViews[i] = _pTextures[i]->pSrView.GetComPtr();
			this->logSrViewBuffer[i] = (int)srViews[i];
		}
		else
		{
			sampler[i] = this->nullTexture[i].pSampState.GetComPtr();
			this->logSamplerBuffer[i] = (int)this->nullTexture[i].pSampState.GetComPtr();
			srViews[i] = this->nullTexture[i].pSrView.GetComPtr();
			this->logSrViewBuffer[i] = (int)this->nullTexture[i].pSrView.GetComPtr();
		}
	}

	this->pd3dDeviceContext->PSSetSamplers(_sampSlotStart, _num, sampler.data());
	this->pd3dDeviceContext->PSSetShaderResources(_srSlotStart, _num, srViews.data());
}


//------------------------------------------------------------------------------
/**	@brief		�e�N�X�`���ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/06	02:51	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::SetVertexTexture(
	int _srSlot,
	int _sampSlot,
	ID3D11ShaderResourceView** _ppSrView,
	ID3D11SamplerState** _ppSampler,
	BOOL _isCache,
	BOOL _isForceBind)
{
	ID3D11SamplerState** pSetSampler = (_ppSampler) ? _ppSampler : this->nullTexture[_srSlot].pSampState.GetComPtrPtr();
	int checkLogSampState = (int)((_ppSampler) ? (*_ppSampler) : this->nullTexture[_srSlot].pSampState.GetComPtr());

	if (this->logVertexSamplerBuffer[_sampSlot] != checkLogSampState || _isForceBind)
	{
		this->pd3dDeviceContext->VSSetSamplers(_sampSlot, 1, pSetSampler);
		if (_isCache)
		{
			this->logVertexSamplerBuffer[_sampSlot] = checkLogSampState;
		}
	}

	ID3D11ShaderResourceView** pSetTexture = (_ppSrView) ? _ppSrView : this->nullTexture[_srSlot].pSrView.GetComPtrPtr();
	int checkLogSrView = (int)((_ppSrView) ? (*_ppSrView) : this->nullTexture[_srSlot].pSrView.GetComPtr());

	if (this->logVertexSrViewBuffer[_srSlot] != checkLogSrView || _isForceBind)
	{
		this->pd3dDeviceContext->VSSetShaderResources(_srSlot, 1, pSetTexture);
		if (_isCache)
		{
			this->logVertexSrViewBuffer[_srSlot] = checkLogSrView;
		}
	}
}


//------------------------------------------------------------------------------
/**	@brief		�e�N�X�`���ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/06	02:51	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::SetVertexTexture(
	int _srSlot,
	int _sampSlot,
	BaseTexture* _pTexture,
	BOOL _isCache,
	BOOL _isForceBind)
{
	BaseTexture* pUseTexture = (_pTexture) ? _pTexture : &this->nullTexture[_srSlot];

	if (this->logVertexSamplerBuffer[_sampSlot] != (int)pUseTexture->pSampState.GetComPtr() || _isForceBind)
	{
		this->pd3dDeviceContext->VSSetSamplers(_sampSlot, 1, pUseTexture->pSampState.GetComPtrPtr());
		if (_isCache)
		{
			this->logVertexSamplerBuffer[_sampSlot] = (int)pUseTexture->pSampState.GetComPtr();
		}
	}

	if (this->logVertexSrViewBuffer[_srSlot] != (int)pUseTexture->pSrView.GetComPtr() || _isForceBind)
	{
		this->pd3dDeviceContext->VSSetShaderResources(_srSlot, 1, pUseTexture->pSrView.GetComPtrPtr());
		if (_isCache)
		{
			this->logVertexSrViewBuffer[_srSlot] = (int)pUseTexture->pSrView.GetComPtr();
		}
	}
}


//------------------------------------------------------------------------------
/**	@brief		�e�N�X�`���ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/06	02:51	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::SetVertexTexture(
	int _num,
	int _srSlotStart,
	int _sampSlotStart,
	BaseTexture** _pTextures)
{
	std::array<ID3D11SamplerState*, MAX_TEXTURE> sampler;
	std::array<ID3D11ShaderResourceView*, MAX_TEXTURE> srViews;
	sampler.fill(nullptr);
	srViews.fill(nullptr);
	for (int i = 0; i < _num; ++i)
	{
		if (_pTextures[i])
		{
			sampler[i] = _pTextures[i]->pSampState.GetComPtr();
			this->logVertexSamplerBuffer[i] = (int)sampler[i];
			srViews[i] = _pTextures[i]->pSrView.GetComPtr();
			this->logVertexSrViewBuffer[i] = (int)srViews[i];
		}
		else
		{
			sampler[i] = this->nullTexture[i].pSampState.GetComPtr();
			this->logVertexSamplerBuffer[i] = (int)this->nullTexture[i].pSampState.GetComPtr();
			srViews[i] = this->nullTexture[i].pSrView.GetComPtr();
			this->logVertexSrViewBuffer[i] = (int)this->nullTexture[i].pSrView.GetComPtr();
		}
	}

	this->pd3dDeviceContext->VSSetSamplers(_sampSlotStart, _num, sampler.data());
	this->pd3dDeviceContext->VSSetShaderResources(_srSlotStart, _num, srViews.data());
}



//------------------------------------------------------------------------------
/**	@brief		�v�������_�����O�o�^ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/03	16:20	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::PreRenderPushback(RenderBaseShader* _pNode, UINT _priority)
{
	RenderNode pushback;

	pushback.shader = _pNode;
	pushback.priority = _priority;

	_pNode->OnPushbackEnter();

	this->preRenderList.push_back(pushback);

	_pNode->OnPushbackQuit();

	if (_priority != 0xCCCCCCC)
	{
		this->isNeedSortPreRenderList = TRUE;
	}
}


//------------------------------------------------------------------------------
/**	@brief		�����_���[�o�^ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/16	23:03	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::RenderPushback(RenderBaseShader* _pNode, UINT _priority)
{
	RenderNode pushback;

	pushback.shader = _pNode;
	pushback.priority = _priority;

	_pNode->OnPushbackEnter();

	this->renderList.push_back(pushback);

	_pNode->OnPushbackQuit();

	if (_priority != 0xCCCCCCC)
	{
		this->isNeedSortRenderList = TRUE;
	}
}


//------------------------------------------------------------------------------
/**	@brief		�|�X�g�����_�����O�o�^ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	10:31	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::PostRenderPushback(RenderBaseShader* _pNode, UINT _priority)
{
	RenderNode pushback;

	pushback.shader = _pNode;
	pushback.priority = _priority;

	_pNode->OnPushbackEnter();

	this->postRenderList.push_back(pushback);

	_pNode->OnPushbackQuit();

	if (_priority != 0xCCCCCCC)
	{
		this->isNeedSortPostRenderList = TRUE;
	}
}


//------------------------------------------------------------------------------
/**	@brief		�v���o�[�X�g�����_�����O */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/03	16:32	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsRenderer::PreRenedrBurst()
{
	if (this->preRenderList.empty())
	{
		return S_FALSE;
	}

	if (this->isNeedSortPreRenderList)
	{
		//�����_���ɂ��v���f�B�P�[�g
		auto Predicate = [](RenderNode& inst1, RenderNode& inst2)
		{
			return (inst1.priority < inst2.priority);
		};
		std::sort(this->preRenderList.begin(), this->preRenderList.end(), Predicate);
	}

	for (auto& it : this->preRenderList)
	{
		it.shader->OnDraw();
	}

	for (auto& it : this->preRenderList)
	{
		it.shader->OnResult();
	}

	this->isNeedSortPreRenderList = FALSE;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�o�[�X�g�����_�����O */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	05:37	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsRenderer::RenderBurst()
{
	if (this->renderList.empty())
	{
		return S_FALSE;
	}

	if (this->isNeedSortRenderList)
	{
		//�����_���ɂ��v���f�B�P�[�g
		auto Predicate = [](RenderNode& inst1, RenderNode& inst2)
		{
			return (inst1.priority < inst2.priority);
		};
		std::sort(this->renderList.begin(), this->renderList.end(), Predicate);
	}

	for (auto& it : this->renderList)
	{
		it.shader->OnDraw();
	}

	for (auto& it : this->renderList)
	{
		it.shader->OnResult();
	}

	this->isNeedSortRenderList = FALSE;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�|�X�g�o�[�X�g�����_�����O */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/08	10:29	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsRenderer::PostRenderBurst()
{
	if (this->postRenderList.empty())
	{
		return S_FALSE;
	}

	if (this->isNeedSortPostRenderList)
	{
		//�����_���ɂ��v���f�B�P�[�g
		auto Predicate = [](RenderNode& inst1, RenderNode& inst2)
		{
			return (inst1.priority < inst2.priority);
		};
		std::sort(this->postRenderList.begin(), this->postRenderList.end(), Predicate);
	}

	for (auto& it : this->postRenderList)
	{
		it.shader->OnDraw();
	}

	for (auto& it : this->postRenderList)
	{
		it.shader->OnResult();
	}

	this->isNeedSortPostRenderList = FALSE;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�����_�����O��j�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/15	02:08	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::ClearRender()
{
	for (auto& it : this->preRenderList)
	{
		it.shader->OnClear();
	}
	for (auto& it : this->renderList)
	{
		it.shader->OnClear();
	}
	for (auto& it : this->postRenderList)
	{
		it.shader->OnClear();
	}

	this->preRenderList.clear();
	this->renderList.clear();
	this->postRenderList.clear();
}


//------------------------------------------------------------------------------
/**	@brief		���`�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/15	22:13	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsRenderer::RenderAxis(
	const float _sx,
	const float _sy,
	const float _sz,
	const float _ex,
	const float _ey,
	const float _ez,
	const float _r,
	const float _g,
	const float _b,
	const float _a,
	const XMFLOAT4X4* _pOrigin)
{
	HRESULT hr = E_FAIL;

	this->axisBuffer.vertex[0].x = _sx;
	this->axisBuffer.vertex[0].y = _sy;
	this->axisBuffer.vertex[0].z = _sz;
	this->axisBuffer.vertex[1].x = _ex;
	this->axisBuffer.vertex[1].y = _ey;
	this->axisBuffer.vertex[1].z = _ez;

	ao::OverWriteVertexBuffer(
		this->pd3dDeviceContext.GetComPtr(),
		this->axisBuffer.buffer.GetComPtr(),
		this->axisBuffer.vertex.data(),
		sizeof(AxisBuffer<2>::Vertex) * 2);

	this->pd3dDeviceContext->VSSetShader(this->gridShader.vertex.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->gridShader.pixel.GetComPtr(), nullptr, 0);

	const float color[4] = { _r, _g, _b, _a };
	if (_pOrigin)
	{
		this->MapAxisConstBuffer(*_pOrigin, color);
	}
	else
	{
		XMFLOAT4X4 origin;
		ao::MatrixIdentity(&origin);
		this->MapAxisConstBuffer(origin, color);
	}

	UINT offset = 0;
	UINT stride = sizeof(AxisBuffer<2>::Vertex);

	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	this->pd3dDeviceContext->IASetInputLayout(this->gridShader.inputLayout.GetComPtr());
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->axisBuffer.buffer.GetComPtrPtr(), &stride, &offset);
	this->pd3dDeviceContext->Draw(2, 0);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		���`�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/15	22:13	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsRenderer::RenderAxis(
	const AxisData* _axisData,
	const int _num,
	const float _r,
	const float _g,
	const float _b,
	const float _a,
	const XMFLOAT4X4* _pOrigin)
{
	HRESULT hr = E_FAIL;

	SmartComPtr<ID3D11Buffer> pTempBuffer;
	ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		pTempBuffer.ToCreator(),
		sizeof(AxisData)*_num,
		_axisData);

	this->pd3dDeviceContext->VSSetShader(this->gridShader.vertex.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->gridShader.pixel.GetComPtr(), nullptr, 0);

	float color[4] = { _r, _g, _b, _a };
	if (_pOrigin)
	{
		this->MapAxisConstBuffer(*_pOrigin, color);
	}
	else
	{
		XMFLOAT4X4 origin;
		ao::MatrixIdentity(&origin);
		this->MapAxisConstBuffer(origin, color);
	}

	UINT offset = 0;
	UINT stride = sizeof(AxisData)/2;

	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	this->pd3dDeviceContext->IASetInputLayout(this->gridShader.inputLayout.GetComPtr());
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, pTempBuffer.GetComPtrPtr(), &stride, &offset);
	this->pd3dDeviceContext->Draw(_num * 2, 0);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		���`�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/11/08	21:56	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsRenderer::RenderAxis(
	ID3D11Buffer* _pVB,
	const int _numRender,
	const float _r,
	const float _g,
	const float _b,
	const float _a,
	const XMFLOAT4X4* _pOrigin)
{
	HRESULT hr = E_FAIL;

	this->pd3dDeviceContext->VSSetShader(this->gridShader.vertex.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->gridShader.pixel.GetComPtr(), nullptr, 0);

	float color[4] = { _r, _g, _b, _a };

	if (_pOrigin)
	{
		this->MapAxisConstBuffer(*_pOrigin, color);
	}
	else
	{
		XMFLOAT4X4 origin;
		ao::MatrixIdentity(&origin);
		this->MapAxisConstBuffer(origin, color);
	}

	UINT offset = 0;
	UINT stride = sizeof(AxisData)/2;

	ID3D11Buffer* const setVB[] = { _pVB };
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	this->pd3dDeviceContext->IASetInputLayout(this->gridShader.inputLayout.GetComPtr());
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, setVB, &stride, &offset);
	this->pd3dDeviceContext->Draw(_numRender, 0);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		���`�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/15	22:13	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsRenderer::RenderPose(
	Transform& _pose,
	const float _length,
	float _r,
	float _g,
	float _b,
	float _a)
{
	HRESULT hr = E_FAIL;

	XMFLOAT4X4 mtx;
	_pose.GetMatrix(&mtx);
	this->poseBuffer.vertex[0].x = this->poseBuffer.vertex[2].x = this->poseBuffer.vertex[4].x = mtx._41;
	this->poseBuffer.vertex[0].y = this->poseBuffer.vertex[2].y = this->poseBuffer.vertex[4].y = mtx._42;
	this->poseBuffer.vertex[0].z = this->poseBuffer.vertex[2].z = this->poseBuffer.vertex[4].z = mtx._43;
	this->poseBuffer.vertex[1].x = mtx._41 + mtx._11 * _length;
	this->poseBuffer.vertex[1].y = mtx._42 + mtx._12 * _length;
	this->poseBuffer.vertex[1].z = mtx._43 + mtx._13 * _length;
	this->poseBuffer.vertex[3].x = mtx._41 + mtx._21 * _length;
	this->poseBuffer.vertex[3].y = mtx._42 + mtx._22 * _length;
	this->poseBuffer.vertex[3].z = mtx._43 + mtx._23 * _length;
	this->poseBuffer.vertex[5].x = mtx._41 + mtx._31 * _length;
	this->poseBuffer.vertex[5].y = mtx._42 + mtx._32 * _length;
	this->poseBuffer.vertex[5].z = mtx._43 + mtx._33 * _length;

	FLOAT vtxCol[][4] = { { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 0, 0, 1, 1 } };
	for (int i = 0; i < 3; ++i)
	{
		this->poseBuffer.vertex[i * 2].r = vtxCol[i][0];
		this->poseBuffer.vertex[i * 2].g = vtxCol[i][1];
		this->poseBuffer.vertex[i * 2].b = vtxCol[i][2];
		this->poseBuffer.vertex[i * 2].a = vtxCol[i][3];
		this->poseBuffer.vertex[i * 2 + 1].r = vtxCol[i][0];
		this->poseBuffer.vertex[i * 2 + 1].g = vtxCol[i][1];
		this->poseBuffer.vertex[i * 2 + 1].b = vtxCol[i][2];
		this->poseBuffer.vertex[i * 2 + 1].a = vtxCol[i][3];
	}

	ao::OverWriteVertexBuffer(
		this->pd3dDeviceContext.GetComPtr(),
		this->poseBuffer.buffer.GetComPtr(),
		this->poseBuffer.vertex.data(),
		sizeof(AxisBuffer<6>::Vertex) * 6);

	this->pd3dDeviceContext->VSSetShader(this->gridShader.vertex.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->gridShader.pixel.GetComPtr(), nullptr, 0);

	DirectX::XMFLOAT4X4 origin;
	float color[4] = { _r, _g, _b, _a };
	ao::MatrixIdentity(&origin);
	this->MapAxisConstBuffer(origin, color);

	UINT offset = 0;
	UINT stride = sizeof(AxisBuffer < 6 >::Vertex);

	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	this->pd3dDeviceContext->IASetInputLayout(this->gridShader.inputLayout.GetComPtr());
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->poseBuffer.buffer.GetComPtrPtr(), &stride, &offset);
	this->pd3dDeviceContext->Draw(6, 0);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�O���b�h���`�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/14	23:36	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsRenderer::RenderGrid(
	const float _x,
	const float _y,
	const float _z,
	const int _num,
	const float _space,
	const float _r,
	const float _g,
	const float _b,
	const float _a)
{
	if (_num >= MAX_GRID)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11GraphicsRenderer::RenderGrid]\n_num is too many.(over %d)\n"), MAX_GRID);
		return E_INVALIDARG;
	}

	//�O���b�h���i���_���j�����
	const int numGrid = (_num * 2 + 1) * 2;
	const float HL = _space*_num;		// half length
	const float XO = _space*_num;		// x offset

	for (int i = 0; i < numGrid; i += 2)
	{
		const float XS = i / 2 * _space;
		this->xGridBuffer.vertex[i].x = _x - XO + XS;
		this->xGridBuffer.vertex[i].y = _y;
		this->xGridBuffer.vertex[i].z = _z - HL;
		this->xGridBuffer.vertex[i + 1].x = _x - XO + XS;
		this->xGridBuffer.vertex[i + 1].y = _y;
		this->xGridBuffer.vertex[i + 1].z = _z + HL;
	}

	const float ZO = _space*_num;		// z offset
	for (int i = 0; i < numGrid; i += 2)
	{
		const float ZS = i / 2 * _space;
		this->zGridBuffer.vertex[i].x = _x - HL;
		this->zGridBuffer.vertex[i].y = _y;
		this->zGridBuffer.vertex[i].z = _z - ZO + ZS;
		this->zGridBuffer.vertex[i + 1].x = _x + HL;
		this->zGridBuffer.vertex[i + 1].y = _y;
		this->zGridBuffer.vertex[i + 1].z = _z - ZO + ZS;
	}

	ao::OverWriteVertexBuffer(
		this->pd3dDeviceContext.GetComPtr(),
		this->xGridBuffer.buffer.GetComPtr(),
		this->xGridBuffer.vertex.data(),
		sizeof(GridBuffer::Vertex) * numGrid);

	ao::OverWriteVertexBuffer(
		this->pd3dDeviceContext.GetComPtr(),
		this->zGridBuffer.buffer.GetComPtr(),
		this->zGridBuffer.vertex.data(),
		sizeof(GridBuffer::Vertex) * numGrid);


	this->pd3dDeviceContext->VSSetShader(this->gridShader.vertex.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->gridShader.pixel.GetComPtr(), nullptr, 0);

	DirectX::XMFLOAT4X4 origin;
	float color[4] = { _r, _g, _b, _a };
	ao::MatrixIdentity(&origin);
	this->MapAxisConstBuffer(origin, color);

	UINT offset = 0;
	UINT stride = sizeof(GridBuffer::Vertex);

	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	this->pd3dDeviceContext->IASetInputLayout(this->gridShader.inputLayout.GetComPtr());
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->xGridBuffer.buffer.GetComPtrPtr(), &stride, &offset);
	this->pd3dDeviceContext->Draw(numGrid, 0);
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->zGridBuffer.buffer.GetComPtrPtr(), &stride, &offset);
	this->pd3dDeviceContext->Draw(numGrid, 0);

	return S_OK;
}



//------------------------------------------------------------------------------
/**	@brief		�ȈՓI�ȃX�v���C�g�`�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/20	21:50	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsRenderer::RenderSprite(
	const float _x,
	const float _y,
	const float _width,
	const float _height,
	const color _color,
	BaseTexture* const _pTexture,
	const SamplerState sampType,
	const OrthoType _orthoType)
{
	this->pd3dDeviceContext->VSSetShader(this->spriteShader.vertex.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->spriteShader.pixel.GetComPtr(), nullptr, 0);


	UINT offset = 0;
	UINT stride = sizeof(UINT);
	this->pd3dDeviceContext->IASetInputLayout(this->spriteShader.inputLayout.GetComPtr());
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->pSpriteVertexBuffer.GetComPtrPtr(), &stride, &offset);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN,0);
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	this->pGraphicsState->SetBlendState(BlendState::Alpha);
	this->pGraphicsState->SetDepthStecilState(FALSE);
	this->pGraphicsState->SetRasterizerState(D3D11_FILL_MODE::D3D11_FILL_SOLID);

	XMFLOAT4X4A mtx;
	MatrixIdentity(&mtx);

	mtx._11 = _width;
	mtx._22 = _height;
	mtx._41 = _x;
	mtx._42 = _y;

	this->spriteConstBuffer->color = _color;

	//���ˉe�s��^�C�v
	switch (_orthoType)
	{
	case OrthoType::LeftTop:
		MatrixMultiply(&this->spriteConstBuffer->matWP.xm, mtx, this->leftTopOrthoMatrix);
		break;

	case OrthoType::Center:
		MatrixMultiply(&this->spriteConstBuffer->matWP.xm, mtx, this->centerOrthoMatrix);
		break;
	}

	MatrixTranspose(&this->spriteConstBuffer->matWP.xm, this->spriteConstBuffer->matWP.xm);
	this->spriteConstBuffer.MapVertex(0);

	this->SetTexture(0, 0, _pTexture);
	this->pd3dDeviceContext->Draw(4, 0);


	this->pGraphicsState->SetDepthStecilState(TRUE);

	return S_OK;
}




//------------------------------------------------------------------------------
/**	@brief		�r���[�s��̍X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/15	04:35	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void DirectX11GraphicsRenderer::UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx)
{
	::CopyMemory(&this->viewMatrix, _pViewMtx, sizeof(XMFLOAT4X4));
	ao::MatrixMultiply(&this->viewProjMatrix, this->viewMatrix, this->projMatrix);
	
}


//------------------------------------------------------------------------------
/**	@brief		���ˉe�s��擾(���㌴�_) */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/16	23:27	T.Aoyama	�쐬
//------------------------------------------------------------------------------
const XMFLOAT4X4* DirectX11GraphicsRenderer::GetLeftTopOrthoMatrix(XMFLOAT4X4* _pOut)
{
	if (_pOut == nullptr)
	{
		return &this->leftTopOrthoMatrix;
	}

	::CopyMemory(_pOut, &this->leftTopOrthoMatrix, sizeof(XMFLOAT4X4));

	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		���ˉe�s��擾(���S���_) */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/16	23:27	T.Aoyama	�쐬
//------------------------------------------------------------------------------
const XMFLOAT4X4* DirectX11GraphicsRenderer::GetCenterOrthoMatrix(XMFLOAT4X4* _pOut)
{
	if (_pOut == nullptr)
	{
		return &this->centerOrthoMatrix;
	}

	::CopyMemory(_pOut, &this->centerOrthoMatrix, sizeof(XMFLOAT4X4));

	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		�ˉe�s��擾 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/16	21:38	T.Aoyama	�쐬
//------------------------------------------------------------------------------
const XMFLOAT4X4* DirectX11GraphicsRenderer::GetProjMatrix(XMFLOAT4X4* _pOut)
{
	if (_pOut == nullptr)
	{
		return &this->projMatrix;
	}

	::CopyMemory(_pOut, &this->projMatrix, sizeof(XMFLOAT4X4));

	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		�r���[�s��擾 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	03:19	T.Aoyama	�쐬
//------------------------------------------------------------------------------
const XMFLOAT4X4* DirectX11GraphicsRenderer::GetViewMatrix(XMFLOAT4X4* _pOut)
{
	if (_pOut == nullptr)
	{
		return &this->viewMatrix;
	}
	
	::CopyMemory(_pOut, &this->viewMatrix, sizeof(XMFLOAT4X4));

	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		�r���[�s��擾 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	03:19	T.Aoyama	�쐬
//------------------------------------------------------------------------------
const XMFLOAT4X4* DirectX11GraphicsRenderer::GetViewProjMatrix(XMFLOAT4X4* _pOut)
{
	if (_pOut == nullptr)
	{
		return &this->viewProjMatrix;
	}

	::CopyMemory(_pOut, &this->viewProjMatrix, sizeof(XMFLOAT4X4));

	return _pOut;
}


//------------------------------------------------------------------------------
/**	@brief		null�e�N�X�`���쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	21:22	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline HRESULT DirectX11GraphicsRenderer::CreateNullTexture()
{
	D3D11_TEXTURE2D_DESC desc;

	desc.Width = 1;
	desc.Height = 1;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = FALSE;
	desc.MiscFlags = FALSE;

	UINT tempColor = 0xFFFFFFFF;
	D3D11_SUBRESOURCE_DATA subResource;

	subResource.pSysMem = &tempColor;
	subResource.SysMemPitch = sizeof(UINT);
	subResource.SysMemSlicePitch = 0;

	//NULL�p�i��1�s�N�Z���j�e�N�X�`���쐬
	HRESULT hr = S_OK;
	SmartComPtr<ID3D11Texture2D> pTempResource[MAX_TEXTURE];

	//�f�t�H���g��NullTexture�쐬->SRV�쐬
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		hr = this->pd3dDevice->CreateTexture2D(&desc, &subResource, pTempResource[i].ToCreator());

		AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create texture 2D.\n"));


		hr = this->pd3dDevice->CreateShaderResourceView(
			pTempResource[i].GetComPtr(),
			nullptr,
			this->nullTexture[i].pSrView.ToCreator());

		AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create shader resource view.\n"));
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		null�p�̃T���v���[�X�e�[�g�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	22:08	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline HRESULT DirectX11GraphicsRenderer::CreateNullSamplerState()
{
	// �e�N�X�`���C���[�W�̏���ݒ�
	D3D11_SAMPLER_DESC	samplerDesc;

	//�e�N�X�`���̃T���v���X�e�[�g��ݒ�
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// �T���v�����O���Ɏg�p����t�B���^�B
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 �` 1 �͈̔͊O�ɂ��� u �e�N�X�`���[���W�̕`����@
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 �` 1 �͈̔͊O�ɂ��� v �e�N�X�`���[���W
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 �` 1 �͈̔͊O�ɂ��� w �e�N�X�`���[���W
	samplerDesc.MipLODBias = 0;								// �v�Z���ꂽ�~�b�v�}�b�v ���x������̃o�C�A�X
	samplerDesc.MaxAnisotropy = 16;								// �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;			// ��r�I�v�V�����B
	// ���E�F���o�[�X�g�R�s�[
	::CopyMemory(samplerDesc.BorderColor, &XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(XMFLOAT4));
	//�~�b�v�}�b�v�̐ݒ�
	samplerDesc.MinLOD = 0;							// �A�N�Z�X�\�ȃ~�b�v�}�b�v�̉����l
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;			// �A�N�Z�X�\�ȃ~�b�v�}�b�v�̏���l

	//�T���v���[�X�e�[�g�쐬
	HRESULT hr = E_FAIL;
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		hr = this->pd3dDevice->CreateSamplerState(&samplerDesc, this->nullTexture[i].pSampState.ToCreator());
		AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create sampler state.\n"));
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		���p�̒萔�o�b�t�@�}�b�v */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/15	01:39	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline HRESULT DirectX11GraphicsRenderer::MapAxisConstBuffer(const XMFLOAT4X4& _worldMtx, const float* _color)
{
	ao::MatrixMultiply(&this->gridConstBuffer.rawVsBuffer.matWVP, _worldMtx, this->viewProjMatrix);
	ao::MatrixTranspose(&this->gridConstBuffer.rawVsBuffer.matWVP, this->gridConstBuffer.rawVsBuffer.matWVP);
	::CopyMemory(this->gridConstBuffer.rawPsBuffer.color, _color, sizeof(float) * 4);

	//�}�b�s���O����
	HRESULT hr = E_FAIL;

	hr = ao::MapVertexConstantBuffer(
		this->pd3dDeviceContext.GetComPtr(),
		0,
		1,
		this->gridConstBuffer.vsBuffer.GetComPtrPtr(),
		&this->gridConstBuffer.rawVsBuffer,
		sizeof(GridConstBuffer::VsConst));

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed map axis vertex constant buffer.\n"));


	hr = ao::MapPixelConstantBuffer(
		this->pd3dDeviceContext.GetComPtr(),
		0,
		1,
		this->gridConstBuffer.psBuffer.GetComPtrPtr(),
		&this->gridConstBuffer.rawPsBuffer,
		sizeof(GridConstBuffer::PsConst));

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed map axis pixel constant buffer.\n"));

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�V�F�[�_�[�t�@�C���̓ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/20	22:47	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsRenderer::LoadShaderFile(LPCWSTR _vsFilePath, LPCWSTR _psFilePath, ShaderPKG* const _pOutShader)
{
	HRESULT hr(E_FAIL);

	UINT flags = 0;
#ifdef __AO_DEBUG__
	flags = D3DCOMPILE_DEBUG;
#else
	flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	SmartComPtr<ID3DBlob> pCompiledShader;

	//���_�V�F�[�_�[�ǂݍ���
	hr = D3DCompileFromFile(
		_vsFilePath,
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		flags,
		0,
		pCompiledShader.ToCreator(),
		nullptr);

	
	_ASSERT_EXPR(SUCCEEDED(hr), "Failed load system shader file." );

	//���_�V�F�[�_�[����
	hr = this->pd3dDevice->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		nullptr,
		_pOutShader->vertex.ToCreator());

	_ASSERT_EXPR(SUCCEEDED(hr), "Failed load system shader file.");

	//���_���C�A�E�g�̃��t���N�V����
	CreateAutoVertexInputLayout(
		this->pd3dDevice.GetComPtr(),
		_pOutShader->inputLayout.ToCreator(),
		pCompiledShader.GetComPtr());

	_ASSERT_EXPR(SUCCEEDED(hr), "Failed load system shader file.");


	//�s�N�Z���V�F�[�_�[�ǂݍ���
	hr = D3DCompileFromFile(
		_psFilePath,
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		flags,
		0,
		pCompiledShader.ToCreator(),
		nullptr);

	_ASSERT_EXPR(SUCCEEDED(hr), "Failed load system shader file.");

	//�s�N�Z���V�F�[�_�[����
	hr = this->pd3dDevice->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		nullptr,
		_pOutShader->pixel.ToCreator());

	_ASSERT_EXPR(SUCCEEDED(hr), "Failed load system shader file.");


	return S_OK;
}