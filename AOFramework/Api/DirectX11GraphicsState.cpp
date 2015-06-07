#include"stdafx.hpp"
#include"DirectX11GraphicsState.hpp"


using namespace ao;

static const D3D11_RENDER_TARGET_BLEND_DESC BlendStateDescArray[(int)BlendState::Max] =
{
	{ FALSE, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL },
	{ TRUE, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL },
	{ TRUE, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL },
	{ TRUE, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, D3D11_BLEND_OP_REV_SUBTRACT, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL },
	{ TRUE, D3D11_BLEND_ZERO, D3D11_BLEND_SRC_COLOR, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL },
};


DirectX11GraphicsState::DirectX11GraphicsState(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	DirectX11BaseApi(_pd3dDevice,_pd3dDeviceContext)
{
	this->nowBlendState.blend = BlendState::Invalid;
	this->nowBlendState.enableAlphaToCoverage = FALSE;
}


DirectX11GraphicsState::~DirectX11GraphicsState()
{
	for (auto& it : this->pBlendStateMap)
	{
		it.second->Release();
	}

	for (auto& it : this->pDepthStencilMap)
	{
		it.second->Release();
	}

	for (auto& it : this->pRasterizerMap)
	{
		it.second->Release();
	}
}


//------------------------------------------------------------------------------
/**	@brief		�u�����h�X�e�[�g�ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/17	01:02	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsState::SetBlendState(
	const BlendState _blendState,
	const UINT _numBlendState,
	const BOOL _alphaToCoverageEnable,
	const BOOL _isForceBind)
{
	//�����_�[�^�[�Q�b�g��8���Ȃ�
#ifdef __AO_DEBUG__
	if (_numBlendState > 8)
	{
		return E_INVALIDARG;
	}
#endif

	//���݂̏�ԂƓ�����������return
	BOOL f1 = (this->nowBlendState.blend == _blendState);
	BOOL f2 = (this->nowBlendState.enableAlphaToCoverage == _alphaToCoverageEnable);
	BOOL f3 = (_isForceBind == FALSE);
	if (f1 && f2 && f3)
	{
		return S_FALSE;
	}

	//�u�����h�X�e�[�g�쐬
	ID3D11BlendState* pBlendState = this->CreateBlendState(_blendState, _numBlendState, _alphaToCoverageEnable);
#ifdef __AO_DEBUG__
	if (pBlendState == nullptr)
	{
		return E_FAIL;
	}
#endif

	//�u�����h�X�e�[�g�ݒ�
	this->pd3dDeviceContext->OMSetBlendState(pBlendState, 0, 0xffffffff);

	//���݂̏�Ԃ��L��
	this->nowBlendState.blend = _blendState;
	this->nowBlendState.enableAlphaToCoverage = _alphaToCoverageEnable;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�[�x�X�e���V���ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/17	01:02	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsState::SetDepthStecilState(
	const BOOL _isEnableDepth,
	const D3D11_DEPTH_WRITE_MASK _depthWriteMask,
	const D3D11_COMPARISON_FUNC _depthFunc,
	const BOOL _isEnableStencil,
	const BOOL _isForceBind)
{
	//���݂̏�ԂƓ�����������return
	BOOL f1 = (this->nowDepthStencilState.depthEnable == _isEnableDepth);
	BOOL f2 = (this->nowDepthStencilState.stencilEnable == _isEnableStencil);
	BOOL f3 = (this->nowDepthStencilState.mask == _depthWriteMask);
	BOOL f4 = (this->nowDepthStencilState.func == _depthFunc);
	BOOL f5 = (_isForceBind == FALSE);
	if (f1 && f2 && f3 && f4 && f5)
		return S_FALSE;

	ID3D11DepthStencilState* pDepthStencilState = this->CreateDepthStencilState(_isEnableDepth, _depthWriteMask, _depthFunc, _isEnableStencil);
#ifdef __AO_DEBUG__
	if (pDepthStencilState == nullptr)
		return E_FAIL;
#endif
	this->pd3dDeviceContext->OMSetDepthStencilState(pDepthStencilState, 0);

	this->nowDepthStencilState.depthEnable = _isEnableDepth;
	this->nowDepthStencilState.stencilEnable = _isEnableStencil;
	this->nowDepthStencilState.mask = _depthWriteMask;
	this->nowDepthStencilState.func = _depthFunc;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		���X�^���C�Y�X�e�[�g�ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/03	03:29	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11GraphicsState::SetRasterizerState(
	D3D11_FILL_MODE _fillMode,
	D3D11_CULL_MODE _cullMode,
	BOOL _isForceBind,
	BOOL _frontCCW,
	INT _depthBias,
	FLOAT _depthBiasClamp,
	FLOAT _slopeScaleDepthBias,
	BOOL _depthClipEnable,
	BOOL _scissorEnable,
	BOOL _multisampleEnable,
	BOOL _antialiasedLineEnable)
{
	//���݂̏�ԂƓ�����������return
	DWORD dwFlags = 0;
	dwFlags = (dwFlags << 0) | (this->nowRasterizerState.fillMode == _fillMode);
	dwFlags = (dwFlags << 1) | (this->nowRasterizerState.cullMode == _cullMode);
	dwFlags = (dwFlags << 2) | (this->nowRasterizerState.frontCounterClockwise == _frontCCW);
	dwFlags = (dwFlags << 3) | (this->nowRasterizerState.depthBias == _depthBias);
	dwFlags = (dwFlags << 4) | (_isForceBind == FALSE);
	if (dwFlags == 0x1F)
		return S_FALSE;

	//�ݒ肷�郉�X�^���C�U���쐬
	ID3D11RasterizerState*	pRasterState = nullptr;
	pRasterState = this->CreateRasterizerState(
		_fillMode,
		_cullMode,
		_frontCCW,
		_depthBias,
		_depthBiasClamp,
		_slopeScaleDepthBias,
		_depthClipEnable,
		_scissorEnable,
		_multisampleEnable,
		_antialiasedLineEnable);
#ifdef __AO_DEBUG__
	if (pRasterState == nullptr)
		return E_FAIL;
#endif

	//���X�^���C�U��ݒ�
	this->pd3dDeviceContext->RSSetState(pRasterState);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�u�����h�X�e�[�g�̃|�C���^�擾 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/17	18:51	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	ID3D11BlendState* DirectX11GraphicsState::CreateBlendState(
	const BlendState _blendState,
	const UINT _numBlendState,
	const BOOL _alphaToCocerageEnable)
{

	USHORT blendCode = 0;
	blendCode = (USHORT)_blendState;
	blendCode = (blendCode << 4) | _numBlendState;
	blendCode = (blendCode << 1) | _alphaToCocerageEnable;
	if (this->pBlendStateMap.find(blendCode) != this->pBlendStateMap.end())
		return this->pBlendStateMap[blendCode];

	D3D11_BLEND_DESC blendDesc;
	::SecureZeroMemory(&blendDesc, sizeof(blendDesc));

	//�s�N�Z���������_�[�^�[�Q�b�g�ɐݒ肷��ہA
	//�A���t�@�g�D�J�o���b�W���}���`�T���v�����O�e�N�j�b�N�Ƃ��Ďg�p���邩�ǂ���
	blendDesc.AlphaToCoverageEnable = _alphaToCocerageEnable;

	//TRUE �F�ʂ̃����_�[�^�[�Q�b�g�ɐݒ�ł���
	//FALSE�F0�Ԗڂ̃����_�[�^�[�Q�b�g�ɐݒ肳���
	blendDesc.IndependentBlendEnable = TRUE;

	//�w�肵�������������_�[�^�[�Q�b�g�ɐݒ肷��
	for (UINT i = 0; i < _numBlendState; i++){
		::CopyMemory(&blendDesc.RenderTarget[i], &BlendStateDescArray[(int)_blendState], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	}

	ID3D11BlendState* pBlendState = nullptr;
	HRESULT hr = this->pd3dDevice->CreateBlendState(&blendDesc, &pBlendState);
#ifdef __AO_DEBUG__
	if (FAILED(hr)){
		if (pBlendState)
			pBlendState->Release();
		return nullptr;
	}
#endif

	this->pBlendStateMap.insert(blend_map::value_type(blendCode, pBlendState));
	return pBlendState;
}


//------------------------------------------------------------------------------
/**	@brief		�[�x�X�e���V���X�e�[�g�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/17	01:28	T.Aoyama	�쐬
//------------------------------------------------------------------------------
ID3D11DepthStencilState* DirectX11GraphicsState::CreateDepthStencilState(
	const BOOL _isEnableDepth,
	const D3D11_DEPTH_WRITE_MASK _depthWriteMask,
	const D3D11_COMPARISON_FUNC _depthFunc,
	const BOOL _isEnableStencil)
{
	USHORT depthStencilCode = 0;

	depthStencilCode = (USHORT)_isEnableDepth;
	depthStencilCode = (USHORT)((depthStencilCode << 1) | _depthWriteMask);
	depthStencilCode = (USHORT)((depthStencilCode << 3) | _depthFunc);
	depthStencilCode = (USHORT)((depthStencilCode << 4) | _isEnableStencil);
	if (this->pDepthStencilMap.find(depthStencilCode) != this->pDepthStencilMap.end())
		return this->pDepthStencilMap[depthStencilCode];

	D3D11_DEPTH_STENCIL_DESC dsState;
	::SecureZeroMemory(&dsState, sizeof(D3D11_DEPTH_STENCIL_DESC));

	dsState.DepthEnable = _isEnableDepth;		// �[�x�e�X�g�̗L������
	dsState.DepthWriteMask = _depthWriteMask;	// �[�x�X�e���V���o�b�t�@�̏������ݐݒ�
	dsState.DepthFunc = _depthFunc;				// �[�x�f�[�^�̔�r�ݒ�
	dsState.StencilEnable = _isEnableStencil;	// �X�e���V���L������

	ID3D11DepthStencilState* pDepthStencilState = nullptr;
	HRESULT hr = this->pd3dDevice->CreateDepthStencilState(&dsState, &pDepthStencilState);
#ifdef __AO_DEBUG__
	if (FAILED(hr)){
		if (pDepthStencilState)
			pDepthStencilState->Release();
		return nullptr;
	}
#endif

	this->pDepthStencilMap.insert(depthStencil_map::value_type(depthStencilCode, pDepthStencilState));
	return pDepthStencilState;
}


//------------------------------------------------------------------------------
/**	@brief		���X�^���C�Y�X�e�[�g�̍쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/03	03:32	T.Aoyama	�쐬
//------------------------------------------------------------------------------
ID3D11RasterizerState* DirectX11GraphicsState::CreateRasterizerState(
	D3D11_FILL_MODE _fillMode,
	D3D11_CULL_MODE _cullMode,
	BOOL _frontCCW,
	INT _depthBias,
	FLOAT _depthBiasClamp,
	FLOAT _slopeScaleDepthBias,
	BOOL _depthClipEnable,
	BOOL _scissorEnable,
	BOOL _multisampleEnable,
	BOOL _antialiasedLineEnable)
{
	//�n�b�V���쐬
	DWORD rasterizerCode = 0;

	rasterizerCode = (DWORD)_fillMode-2;
	rasterizerCode = (DWORD)((rasterizerCode << 2) | _cullMode);
	rasterizerCode = (rasterizerCode << 1) | _frontCCW;
	rasterizerCode = (rasterizerCode << 1) | _depthClipEnable;
	rasterizerCode = (rasterizerCode << 1) | _scissorEnable;
	rasterizerCode = (rasterizerCode << 1) | _multisampleEnable;
	rasterizerCode = (rasterizerCode << 1) | _antialiasedLineEnable;
	rasterizerCode = (rasterizerCode << 8) | (BYTE)_depthBiasClamp;
	rasterizerCode = (rasterizerCode << 8) | (BYTE)_slopeScaleDepthBias;
	rasterizerCode = (rasterizerCode << 8) | (BYTE)_depthBias;

	if (this->pRasterizerMap.find(rasterizerCode) != this->pRasterizerMap.end())
		return this->pRasterizerMap[rasterizerCode];

	HRESULT hr = E_FAIL;
	D3D11_RASTERIZER_DESC	desc;

	desc.FillMode = _fillMode;
	desc.CullMode = _cullMode;
	desc.FrontCounterClockwise = _frontCCW;
	desc.DepthBias = _depthBias;
	desc.DepthBiasClamp = _depthBiasClamp;
	desc.SlopeScaledDepthBias = _slopeScaleDepthBias;
	desc.ScissorEnable = _scissorEnable;
	desc.MultisampleEnable = _multisampleEnable;
	desc.AntialiasedLineEnable = _antialiasedLineEnable;

	//�ݒ肷�郉�X�^���C�U���쐬
	ID3D11RasterizerState*	pRasterState = nullptr;
	hr = this->pd3dDevice->CreateRasterizerState(&desc, &pRasterState);
#ifdef __AO_DEBUG__
	if (FAILED(hr)){
		if (pRasterState)
			pRasterState->Release();
		return nullptr;
	}
#endif

	this->pRasterizerMap.insert(rasterizer_map::value_type(rasterizerCode, pRasterState));
	return pRasterState;
}