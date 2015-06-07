#include"stdafx.hpp"
#include<d3dcompiler.h>
#include"ShaderFunction.hpp"
#include"../../Debug/DebugSystem.hpp"
#include"../../Utility/SmartComPtr.hpp"


using namespace ao;


#if defined(__AO_DEBUG__) || defined(__AO_RDEBUG__)
#define SHADER_FUNCTION_CHECK_ERROR(_hr,_str)							\
		if (FAILED(_hr))												\
		{																\
			ao::debug::PrintfColor(ConsoleColor::H_RED, _T("%s"),_str);	\
		}

#define SHADER_FUNCTION_CHECK_ERROR_RETURN(_hr,_str)					\
		if (FAILED(_hr))												\
		{																\
			ao::debug::PrintfColor(ConsoleColor::H_RED, _T("%s"),_str);	\
			return _hr;													\
		}
#else
#define SHADER_FUNCTION_CHECK_ERROR(_hr,_str)
#define SHADER_FUNCTION_CHECK_ERROR_RETURN(_hr, _str)
#endif


//------------------------------------------------------------------------------
/**	@brief		�V�F�[�_�[�̕�����f�[�^�R���p�C���֐� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/07	01:53	T.Aoyama	�쐬
//	2014/04/24	09:23	T.Aoyama	�������œK��
//------------------------------------------------------------------------------
HRESULT	ao::CompileStringShader(
	ID3DBlob**_ppOutBlob,
	void* const _pSrcData,
	UINT _SrcDataSize,
	LPCSTR _pEntrypoint,
	LPCSTR _pTarget,
	const D3D_SHADER_MACRO* const _pDefines,
	ID3DInclude* const _pInclude,
	ID3DBlob**_ppErrorMsgs)
{
	HRESULT hr S_OK;
	UINT flag = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;

#ifdef __AO_DEBUG__
	flag |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
	flag |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif
	
	hr = D3DCompile(
		_pSrcData,
		_SrcDataSize,
		nullptr,
		_pDefines,
		_pInclude,
		_pEntrypoint,
		_pTarget,
		flag,
		0,
		_ppOutBlob,
		_ppErrorMsgs);

	SHADER_FUNCTION_CHECK_ERROR(hr, "[CompileStringShader]Failed compile shader.\n");
	

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		���_���C�A�E�g��1�v�f���擾 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	01:38	T.Aoyama	�쐬
//------------------------------------------------------------------------------
UINT ao::GetInputElementDesc(
	D3D11_INPUT_ELEMENT_DESC* _pInputElementDesc,
	ID3D11ShaderReflection* _pReflection,
	UINT _paramIndex,
	UINT _byteOffset)
{
	HRESULT hr = E_FAIL;
	D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
	hr = _pReflection->GetInputParameterDesc(_paramIndex, &paramDesc);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[GetInputElementDesc]Failed get input parameter desc.\n");


	D3D11_INPUT_ELEMENT_DESC* desc = _pInputElementDesc;
	desc->SemanticName = paramDesc.SemanticName;
	desc->SemanticIndex = paramDesc.SemanticIndex;
	desc->InputSlot = 0;
	desc->AlignedByteOffset = _byteOffset;
	desc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc->InstanceDataStepRate = 0;

	//�����t�H�[�}�b�g�̎w��ƃo�C�g�I�t�Z�b�g�̌v�Z
	UINT byte = 0;
	if (paramDesc.Mask == 1)
	{
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			desc->Format = DXGI_FORMAT_R32_UINT;
		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			desc->Format = DXGI_FORMAT_R32_SINT;
		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			desc->Format = DXGI_FORMAT_R32_FLOAT;
		byte += 4;
	}
	else if (paramDesc.Mask <= 3)
	{
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			desc->Format = DXGI_FORMAT_R32G32_UINT;
		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			desc->Format = DXGI_FORMAT_R32G32_SINT;
		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			desc->Format = DXGI_FORMAT_R32G32_FLOAT;
		byte += 8;
	}
	else if (paramDesc.Mask <= 7)
	{
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			desc->Format = DXGI_FORMAT_R32G32B32_UINT;
		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			desc->Format = DXGI_FORMAT_R32G32B32_SINT;
		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		byte += 12;
	}
	else if (paramDesc.Mask <= 15)
	{
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			desc->Format = DXGI_FORMAT_R32G32B32A32_UINT;
		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			desc->Format = DXGI_FORMAT_R32G32B32A32_SINT;
		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			desc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		byte += 16;
	}

	return byte;
}


//------------------------------------------------------------------------------
/**	@brief		���_���C�A�E�g���������o */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/02/03	01:49	T.Aoyama	�쐬
//	2014/04/24	09:23	T.Aoyama	�������œK��
//------------------------------------------------------------------------------
HRESULT	ao::CreateAutoVertexInputLayout(
	ID3D11Device* const _pd3dDevice,
	ID3D11InputLayout**_ppOut,
	ID3DBlob* const _pBlob)
{
	HRESULT hr = E_FAIL;
	ID3D11ShaderReflection* pReflection = nullptr;
	hr = D3DReflect(
		_pBlob->GetBufferPointer(),
		_pBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		(void**)&pReflection);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreateAutoVertexInputLayout]Failed D3DReflect.\n");


	D3D11_SHADER_DESC sDesc;
	hr = pReflection->GetDesc(&sDesc);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreateAutoVertexInputLayout]Failed get desc.\n");


	UINT byteOffset = 0;
	std::vector<D3D11_INPUT_ELEMENT_DESC> descVector;
	for (UINT i = 0; i < sDesc.InputParameters; ++i)
	{
		D3D11_INPUT_ELEMENT_DESC ieDesc;
		byteOffset += ::GetInputElementDesc(&ieDesc, pReflection, i, byteOffset);

		//���_���C�A�E�g�̍��ڈ�ɓo�^
		descVector.push_back(ieDesc);
	}

	//���_���̓��C�A�E�g�쐬
	hr = _pd3dDevice->CreateInputLayout(
		&descVector[0],
		static_cast<UINT>(descVector.size()),
		_pBlob->GetBufferPointer(),
		_pBlob->GetBufferSize(),
		_ppOut);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreateAutoVertexInputLayout]Failed create input layout.\n");

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		���_�V�F�[�_�[�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:07	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT ao::CreateVertexShader(
	ID3D11Device* _pd3dDevice,
	ID3D11VertexShader**_ppOutShader,
	ID3D11InputLayout**_ppOutInputLayout,
	void* const _pSrcData,
	const UINT _SrcDataSize,
	const LPCSTR _pEntrypoint,
	const LPCSTR _pTarget,
	const D3D_SHADER_MACRO* const _pDefines,
	ID3DInclude* const _pInclude,
	ID3D11ClassLinkage* _pClassLinkage,
	ID3DBlob**_ppErrorMsgs)
{
	SmartComPtr<ID3DBlob> _pBlob;

	//���_�V�F�[�_�[���R���p�C��
	HRESULT hr(E_FAIL);
	hr = ::CompileStringShader(
		_pBlob.ToCreator(),
		_pSrcData,
		_SrcDataSize,
		_pEntrypoint,
		_pTarget,
		_pDefines,
		_pInclude,
		_ppErrorMsgs);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreateVertexShader]Failed compile vertex shader.\n");


	//���_���̓��C�A�E�g�쐬
	if (_ppOutInputLayout)
	{
		hr = ::CreateAutoVertexInputLayout(
			_pd3dDevice,
			_ppOutInputLayout,
			_pBlob.GetComPtr()
			);

		SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreateVertexShader]Failed create auto vertex input layout.\n");
	}

	//���_�V�F�[�_�[�쐬
	hr = _pd3dDevice->CreateVertexShader(
		_pBlob->GetBufferPointer(),
		_pBlob->GetBufferSize(),
		_pClassLinkage,
		_ppOutShader);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreateVertexShader]Failed create vertex shader.\n");

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�W�I���g���V�F�[�_�[�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:12	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT ao::CreateGeometryShader(
	ID3D11Device* _pd3dDevice,
	ID3D11GeometryShader**_ppOutShader,
	void* const _pSrcData,
	const UINT _SrcDataSize,
	const LPCSTR _pEntrypoint,
	const LPCSTR _pTarget,
	const D3D_SHADER_MACRO* const _pDefines,
	ID3DInclude* const _pInclude,
	ID3D11ClassLinkage* _pClassLinkage,
	ID3DBlob**_ppErrorMsgs)
{
	SmartComPtr<ID3DBlob> pBlob;

	//�W�I���g���V�F�[�_�[���R���p�C��
	HRESULT hr = ::CompileStringShader(
		pBlob.ToCreator(),
		_pSrcData,
		_SrcDataSize,
		_pEntrypoint,
		_pTarget,
		_pDefines,
		_pInclude,
		_ppErrorMsgs);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreateGeometryShader]Failed compile geometry shader.\n");

	//�W�I���g���V�F�[�_�[�쐬
	hr = _pd3dDevice->CreateGeometryShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		_pClassLinkage,
		_ppOutShader);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreateGeometryShader]Failed create geometry shader.\n");

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�s�N�Z���V�F�[�_�[�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	10:29	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT ao::CreatePixelShader(
	ID3D11Device* _pd3dDevice,
	ID3D11PixelShader**_ppOutShader,
	void* const _pSrcData,
	const UINT _SrcDataSize,
	const LPCSTR _pEntrypoint,
	const LPCSTR _pTarget,
	const D3D_SHADER_MACRO* const _pDefines,
	ID3DInclude* const _pInclude,
	ID3D11ClassLinkage* _pClassLinkage,
	ID3DBlob**_ppErrorMsgs)
{
	SmartComPtr<ID3DBlob> pBlob;

	//�s�N�Z���V�F�[�_�[���R���p�C��
	HRESULT hr = ::CompileStringShader(
		pBlob.ToCreator(),
		_pSrcData,
		_SrcDataSize,
		_pEntrypoint,
		_pTarget,
		_pDefines,
		_pInclude,
		_ppErrorMsgs);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr,"[CreatePixelShader]Failed compile pixel shader.\n" );

	//�s�N�Z���V�F�[�_�[�쐬
	hr = _pd3dDevice->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		_pClassLinkage,
		_ppOutShader);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreatePixelShader]Failed create pixel shader.\n");

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�R���s���[�g�V�F�[�_�[�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/04/24	10:51	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT ao::CreateComputeShader(
	ID3D11Device* _pd3dDevice,
	ID3D11ComputeShader**_ppOutShader,
	void* const _pSrcData,
	const UINT _SrcDataSize,
	const LPCSTR _pEntrypoint,
	const LPCSTR _pTarget,
	const D3D_SHADER_MACRO* const _pDefines,
	ID3DInclude* const _pInclude,
	ID3D11ClassLinkage* const _pClassLinkage,
	ID3DBlob** const _ppErrorMsgs)
{
	SmartComPtr<ID3DBlob> _pBlob;

	//�R���s���[�g�V�F�[�_�[���R���p�C��
	HRESULT hr = ::CompileStringShader(
		_pBlob.ToCreator(),
		_pSrcData,
		_SrcDataSize,
		_pEntrypoint,
		_pTarget,
		_pDefines,
		_pInclude,
		_ppErrorMsgs);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr,"[CreateComputeShader]Failed compile compute shader.\n" );


	//�R���s���[�g�V�F�[�_�[�쐬
	hr = _pd3dDevice->CreateComputeShader(
		_pBlob->GetBufferPointer(),
		_pBlob->GetBufferSize(),
		_pClassLinkage,
		_ppOutShader);

	SHADER_FUNCTION_CHECK_ERROR(hr,"[CreateComputeShader]Failed create compute shader.\n" );

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�V�F�[�_�[�o�b�t�@�쐬�֐� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/07	01:52	T.Aoyama	�쐬
//	2014/04/24	09:23	T.Aoyama	�������œK��
//------------------------------------------------------------------------------
HRESULT	ao::CreateBuffer(
	ID3D11Device* const _pd3dDevice,
	ID3D11Buffer** const _ppOutBuffer,
	const void* const _pSrcData,
	const UINT _size,
	const D3D11_USAGE _usage,
	const UINT _cpuAccessFlag,
	const D3D11_BIND_FLAG _bindFlag)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC BufferDesc;
	D3D11_SUBRESOURCE_DATA* pResource = nullptr;

	// �����l��ݒ肷��
	if (_pSrcData)
	{
		pResource = new D3D11_SUBRESOURCE_DATA();
		pResource->pSysMem = _pSrcData;
		pResource->SysMemPitch = 0;
		pResource->SysMemSlicePitch = 0;
	}

	// �o�b�t�@�̐ݒ�
	::SecureZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.ByteWidth = _size;				// �o�b�t�@�T�C�Y
	BufferDesc.Usage = _usage;				// ���\�[�X�g�p�@����肷��
	BufferDesc.BindFlags = _bindFlag;			// �o�b�t�@�̎��
	BufferDesc.CPUAccessFlags = _cpuAccessFlag;		// CPU �A�N�Z�X
	BufferDesc.MiscFlags = 0;					// ���̑��̃t���O���ݒ肵�Ȃ�

	// �o�b�t�@���쐬����
	hr = _pd3dDevice->CreateBuffer(&BufferDesc, pResource, _ppOutBuffer);

	SHADER_FUNCTION_CHECK_ERROR(hr,"[CreateBuffer]Failed create buffer.\n" );

	if (pResource)
	{
		delete pResource;
	}

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		���_�o�b�t�@�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:28	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT	ao::CreateVertexBuffer(
	ID3D11Device* const _pd3dDevice,
	ID3D11Buffer** const _ppOutBuffer,
	const UINT _size,
	const void* const _pSrcData,
	const D3D11_USAGE _usage,
	const UINT _cpuAccessFlag)
{
	HRESULT hr = ::CreateBuffer(
		_pd3dDevice,
		_ppOutBuffer,
		_pSrcData,
		_size,
		_usage,
		_cpuAccessFlag,
		D3D11_BIND_VERTEX_BUFFER);

	SHADER_FUNCTION_CHECK_ERROR(hr,"[CreateVertexBuffer]Failed create buffer.\n" );

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�C���f�b�N�X�o�b�t�@�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:35	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT ao::CreateIndexBuffer(
	ID3D11Device* const _pd3dDevice,
	ID3D11Buffer**_ppOutBuffer,
	const UINT _size,
	const void* const _pSrcData,
	const D3D11_USAGE _usage,
	const UINT _cpuAccessFlag)
{
	HRESULT hr = ::CreateBuffer(
		_pd3dDevice,
		_ppOutBuffer,
		_pSrcData,
		_size,
		_usage,
		_cpuAccessFlag,
		D3D11_BIND_INDEX_BUFFER);

	SHADER_FUNCTION_CHECK_ERROR(hr,"[CreateIndexBuffer]Failed create buffer.\n" );

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�萔�o�b�t�@�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:39	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT	ao::CreateConstantBuffer(
	ID3D11Device* const _pd3dDevice,
	ID3D11Buffer**_ppOutBuffer,
	const UINT _size,
	const void* const _pSrcData,
	CONST D3D11_USAGE _usage,
	CONST UINT _cpuAccessFlag)
{
	HRESULT hr = ::CreateBuffer(
		_pd3dDevice,
		_ppOutBuffer,
		_pSrcData,
		_size,
		_usage,
		_cpuAccessFlag,
		D3D11_BIND_CONSTANT_BUFFER);

	SHADER_FUNCTION_CHECK_ERROR(hr, "[CreateConstantBuffer]Failed create buffer.\n");

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�A���I�[�_�[�A�N�Z�X�o�b�t�@�̍쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/11/15	20:08	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT ao::CreateUnorderedAccessBuffer(
	ID3D11Device* const _pd3dDevice,
	ID3D11Buffer** _ppOutBuffer,
	ID3D11UnorderedAccessView** _ppOutUaView,
	const D3D11_RESOURCE_MISC_FLAG _miscFlag,
	const UINT _elementSize,
	const UINT _nCount,
	const void* _pInitData,
	const D3D11_BIND_FLAG _addBindFlag)
{
	HRESULT hr = S_OK;

	//�\�����o�b�t�@�쐬
	D3D11_BUFFER_DESC bufferDesc;
	::SecureZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));


	//�o�b�t�@��񏉊���

	// �A���I�[�_�[�A�N�Z�X �� �V�F�[�_�[���\�[�X�r���[���w���Bind�t���O�ō쐬
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | _addBindFlag;
	// �o�b�t�@�̎g�p���@���w��
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// �o�b�t�@�T�C�Y���w��
	bufferDesc.ByteWidth = _elementSize * _nCount;
	// ���\�[�X�̎w��
	bufferDesc.MiscFlags = _miscFlag;
	//�\���̔z��̃o�b�t�@�Ȃ�T�C�Y���w��i�v�f�P�ʁj
	if (bufferDesc.MiscFlags == D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
		bufferDesc.StructureByteStride = _elementSize;


	//�����l��ݒ�
	D3D11_SUBRESOURCE_DATA* pResource = nullptr;
	if (_pInitData)
	{
		//�f�[�^������
		pResource = new D3D11_SUBRESOURCE_DATA;
		pResource->pSysMem = _pInitData;
		pResource->SysMemPitch = 0;
		pResource->SysMemSlicePitch = 0;
	}
	//�o�b�t�@�̍쐬
	hr = _pd3dDevice->CreateBuffer(&bufferDesc, pResource, _ppOutBuffer);

	if (pResource)
	{
		delete pResource;
	}

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr,"[ao::CreateUnorderedAccessBuffer]Failed create unordered access buffer.\n" );


	//�A���I�[�_�[�A�N�Z�X�r���[���쐬
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	::SecureZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;	// �o�b�t�@�[�ł��邱�Ƃ��w��
	uavDesc.Buffer.FirstElement = 0;					// �r���[����ŏ��ɃA�N�Z�X����v�f�̃C���f�b�N�X
	//�������ݓǂݍ��݉\�ȃo�C�g�A�h���X�o�b�t�@�[�̂�
	switch (_miscFlag)
	{
	case D3D11_RESOURCE_MISC_GENERATE_MIPS:
		break;
	case D3D11_RESOURCE_MISC_SHARED:
		break;
	case D3D11_RESOURCE_MISC_TEXTURECUBE:
		break;
	case D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS:
		break;
	case D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS:
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;		// �o�b�t�@�̃t���O�w��
		uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;				// ���\�[�X�t�H�[�}�b�g�̎w�肷��
		uavDesc.Buffer.NumElements = bufferDesc.ByteWidth / 4;	// ���\�[�X���̗v�f���i�x�N�g��/4�j
		break;
	case D3D11_RESOURCE_MISC_BUFFER_STRUCTURED:
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;		// ���\�[�X�t�H�[�}�b�g�̎w�肷��
		uavDesc.Buffer.NumElements = _nCount;		// ���\�[�X���̗v�f��
		break;
	case D3D11_RESOURCE_MISC_RESOURCE_CLAMP:
		break;
	case D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX:
		break;
	case D3D11_RESOURCE_MISC_GDI_COMPATIBLE:
		break;
	}

	//�V�F�[�_�[���\�[�X�r���[���쐬
	hr = _pd3dDevice->CreateUnorderedAccessView(*_ppOutBuffer, &uavDesc, _ppOutUaView);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr,"ao::CreateUnorderedAccessBuffer]Failed create unordered access view.\n" );

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		���_�V�F�[�_�[�̒萔�o�b�t�@���X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/10	02:27	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void	ao::UpdateVertexConstantBuffer(
	ID3D11DeviceContext* const _pd3dDeviceContext,
	const UINT _slotId,
	const UINT _numBuffer,
	ID3D11Buffer* const *_ppBuffer,
	const void* const _pSrcData,
	const D3D11_BOX* const _pDstBox)
{
	_pd3dDeviceContext->UpdateSubresource(*_ppBuffer, 0, _pDstBox, _pSrcData, 0, 0);
	_pd3dDeviceContext->VSSetConstantBuffers(_slotId, _numBuffer, _ppBuffer);
}


//------------------------------------------------------------------------------
/**	@brief		�W�I���g���V�F�[�_�[�̒萔�o�b�t�@���X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/10	02:27	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void	UpdateGeometryConstantBuffer(
	ID3D11DeviceContext* const _pd3dDeviceContext,
	const UINT _slotId,
	const UINT _numBuffer,
	ID3D11Buffer* const *_ppBuffer,
	const void* const _pSrcData,
	const D3D11_BOX* const _pDstBox)
{
	_pd3dDeviceContext->UpdateSubresource(*_ppBuffer, 0, _pDstBox, _pSrcData, 0, 0);
	_pd3dDeviceContext->GSSetConstantBuffers(_slotId, _numBuffer, _ppBuffer);
}


//------------------------------------------------------------------------------
/**	@brief		�s�N�Z���V�F�[�_�[�̒萔�o�b�t�@���X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/10	02:27	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void	ao::UpdatePixelConstantBuffer(
	ID3D11DeviceContext* const _pd3dDeviceContext,
	const UINT _slotId,
	const UINT _numBuffer,
	ID3D11Buffer* const *_ppBuffer,
	const void* const _pSrcData,
	const D3D11_BOX* const _pDstBox)
{
	_pd3dDeviceContext->UpdateSubresource(*_ppBuffer, 0, _pDstBox, _pSrcData, 0, 0);
	_pd3dDeviceContext->PSSetConstantBuffers(_slotId, _numBuffer, _ppBuffer);
}


//------------------------------------------------------------------------------
/**	@brief		�R���s���[�g�V�F�[�_�[�̒萔�o�b�t�@���X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/10	02:27	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void	ao::UpdateComputeConstantBuffer(
	ID3D11DeviceContext* const _pd3dDeviceContext,
	const UINT _slotId,
	const UINT _numBuffer,
	ID3D11Buffer* const *_ppBuffer,
	const void* const _pSrcData,
	const D3D11_BOX* const _pDstBox)
{
	_pd3dDeviceContext->UpdateSubresource(*_ppBuffer, 0, _pDstBox, _pSrcData, 0, 0);
	_pd3dDeviceContext->CSSetConstantBuffers(_slotId, _numBuffer, _ppBuffer);
}


//------------------------------------------------------------------------------
/**	@brief		���_�V�F�[�_�[�̒萔�o�b�t�@���X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/07/02	00:00	T.Aoyama	�쐬
//	2013/08/07	04:14	T.Aoyama	������ǉ��i_pd3dDeviceContext�j
//	2013/08/07	05:47	T.Aoyama	������MAP�t���O��ǉ��i_mapFlag�j
//	2013/09/05	09:47	T.Aoyama	�߂�l��HRESULT�ɕύX
//	2014/04/24	09:23	T.Aoyama	�������œK��
//------------------------------------------------------------------------------
HRESULT	ao::MapVertexConstantBuffer(
	ID3D11DeviceContext* const _pd3dDeviceContext,
	const UINT _slotId,
	const UINT _numBuffer,
	ID3D11Buffer* const *_ppBuffer,
	const void* const _pSrcData,
	const UINT _SrcDataSize,
	const D3D11_MAP _mapFlag)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = _pd3dDeviceContext->Map(*_ppBuffer, 0, _mapFlag, 0, &mappedResource);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr,"[MapVertexConstantBuffer]Failed map vertex constant buffer.\n" );


	::CopyMemory(mappedResource.pData, _pSrcData, _SrcDataSize);
	_pd3dDeviceContext->Unmap(*_ppBuffer, 0);

	//�萔�o�b�t�@���Z�b�g
	_pd3dDeviceContext->VSSetConstantBuffers(_slotId, _numBuffer, _ppBuffer);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�W�I���g���V�F�[�_�[�̒萔�o�b�t�@���X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:32	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT	ao::MapGeometryConstantBuffer(
	ID3D11DeviceContext* const _pd3dDeviceContext,
	const UINT _slotId,
	const UINT _numBuffer,
	ID3D11Buffer* const *_ppBuffer,
	const void* const _pSrcData,
	const UINT _SrcDataSize,
	const D3D11_MAP _mapFlag)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = _pd3dDeviceContext->Map(*_ppBuffer, 0, _mapFlag, 0, &mappedResource);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr,"[MapGeometryConstantBuffer]Failed map geometry constant buffer.\n" );


	::CopyMemory(mappedResource.pData, _pSrcData, _SrcDataSize);
	_pd3dDeviceContext->Unmap(*_ppBuffer, 0);

	//�萔�o�b�t�@���Z�b�g
	_pd3dDeviceContext->GSSetConstantBuffers(_slotId, _numBuffer, _ppBuffer);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�s�N�Z���V�F�[�_�[�̒萔�o�b�t�@���X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/07	11:08	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT	ao::MapPixelConstantBuffer(
	ID3D11DeviceContext* const _pd3dDeviceContext,
	const UINT _slotId,
	const UINT _numBuffer,
	ID3D11Buffer* const *_ppBuffer,
	const void* const _pSrcData,
	const UINT _SrcDataSize,
	const D3D11_MAP _mapFlag)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = _pd3dDeviceContext->Map(*_ppBuffer, 0, _mapFlag, 0, &mappedResource);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[MapPixelConstantBuffer]Failed map pixel constant buffer.\n");


	::CopyMemory(mappedResource.pData, _pSrcData, _SrcDataSize);
	_pd3dDeviceContext->Unmap(*_ppBuffer, 0);

	//�萔�o�b�t�@���Z�b�g
	_pd3dDeviceContext->PSSetConstantBuffers(_slotId, _numBuffer, _ppBuffer);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�R���s���[�g�V�F�[�_�[�̒萔�o�b�t�@���X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:33	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT	ao::MapComputeConstantBuffer(
	ID3D11DeviceContext* const _pd3dDeviceContext,
	const UINT _slotId,
	const UINT _numBuffer,
	ID3D11Buffer* const *_ppBuffer,
	const void* const _pSrcData,
	const UINT _SrcDataSize,
	const D3D11_MAP _mapFlag)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = _pd3dDeviceContext->Map(*_ppBuffer, 0, _mapFlag, 0, &mappedResource);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr,"[MapComputeConstantBuffer]Failed map compute constant buffer.\n" );


	::CopyMemory(mappedResource.pData, _pSrcData, _SrcDataSize);
	_pd3dDeviceContext->Unmap(*_ppBuffer, 0);

	//�萔�o�b�t�@���Z�b�g
	_pd3dDeviceContext->CSSetConstantBuffers(_slotId, _numBuffer, _ppBuffer);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�e�N�X�`���̐ݒ�֐� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/09	10:56	T.Aoyama	�쐬
//	2014/04/24	09:23	T.Aoyama	�������œK��
//------------------------------------------------------------------------------
void ao::SetTexture(
	ID3D11DeviceContext* const _pd3dDeviceContext,
	const UINT _srViewSlot,
	const UINT _sampStateSlot,
	ID3D11ShaderResourceView* const *_ppSrView,
	ID3D11SamplerState* const *_ppSampState)
{
	//�e�N�X�`�������݂��Ȃ��A�܂���null�Z�b�g�̏ꍇ
	if (_ppSrView == nullptr || _ppSampState == nullptr)
	{
		ID3D11SamplerState*			pNullSampler[] = { nullptr };
		ID3D11ShaderResourceView*	pNullSRView[] = { nullptr };
		_pd3dDeviceContext->PSSetSamplers(_sampStateSlot, 1, pNullSampler);
		_pd3dDeviceContext->PSSetShaderResources(_srViewSlot, 1, pNullSRView);
		return;
	}

	//�e�N�X�`�������݂���ꍇ�̏���
	_pd3dDeviceContext->PSSetSamplers(_sampStateSlot, 1, _ppSampState);
	_pd3dDeviceContext->PSSetShaderResources(_srViewSlot, 1, _ppSrView);
}