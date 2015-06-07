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
/**	@brief		シェーダーの文字列データコンパイル関数 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/07	01:53	T.Aoyama	作成
//	2014/04/24	09:23	T.Aoyama	処理を最適化
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
/**	@brief		頂点レイアウトの1要素を取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	01:38	T.Aoyama	作成
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

	//属性フォーマットの指定とバイトオフセットの計算
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
/**	@brief		頂点レイアウトを自動抽出 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/02/03	01:49	T.Aoyama	作成
//	2014/04/24	09:23	T.Aoyama	処理を最適化
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

		//頂点レイアウトの項目一つに登録
		descVector.push_back(ieDesc);
	}

	//頂点入力レイアウト作成
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
/**	@brief		頂点シェーダー作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:07	T.Aoyama	作成
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

	//頂点シェーダーをコンパイル
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


	//頂点入力レイアウト作成
	if (_ppOutInputLayout)
	{
		hr = ::CreateAutoVertexInputLayout(
			_pd3dDevice,
			_ppOutInputLayout,
			_pBlob.GetComPtr()
			);

		SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreateVertexShader]Failed create auto vertex input layout.\n");
	}

	//頂点シェーダー作成
	hr = _pd3dDevice->CreateVertexShader(
		_pBlob->GetBufferPointer(),
		_pBlob->GetBufferSize(),
		_pClassLinkage,
		_ppOutShader);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreateVertexShader]Failed create vertex shader.\n");

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		ジオメトリシェーダー作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:12	T.Aoyama	作成
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

	//ジオメトリシェーダーをコンパイル
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

	//ジオメトリシェーダー作成
	hr = _pd3dDevice->CreateGeometryShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		_pClassLinkage,
		_ppOutShader);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreateGeometryShader]Failed create geometry shader.\n");

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		ピクセルシェーダー作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	10:29	T.Aoyama	作成
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

	//ピクセルシェーダーをコンパイル
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

	//ピクセルシェーダー作成
	hr = _pd3dDevice->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		_pClassLinkage,
		_ppOutShader);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr, "[CreatePixelShader]Failed create pixel shader.\n");

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		コンピュートシェーダー作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/04/24	10:51	T.Aoyama	作成
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

	//コンピュートシェーダーをコンパイル
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


	//コンピュートシェーダー作成
	hr = _pd3dDevice->CreateComputeShader(
		_pBlob->GetBufferPointer(),
		_pBlob->GetBufferSize(),
		_pClassLinkage,
		_ppOutShader);

	SHADER_FUNCTION_CHECK_ERROR(hr,"[CreateComputeShader]Failed create compute shader.\n" );

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		シェーダーバッファ作成関数 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/07	01:52	T.Aoyama	作成
//	2014/04/24	09:23	T.Aoyama	処理を最適化
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

	// 初期値を設定する
	if (_pSrcData)
	{
		pResource = new D3D11_SUBRESOURCE_DATA();
		pResource->pSysMem = _pSrcData;
		pResource->SysMemPitch = 0;
		pResource->SysMemSlicePitch = 0;
	}

	// バッファの設定
	::SecureZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.ByteWidth = _size;				// バッファサイズ
	BufferDesc.Usage = _usage;				// リソース使用法を特定する
	BufferDesc.BindFlags = _bindFlag;			// バッファの種類
	BufferDesc.CPUAccessFlags = _cpuAccessFlag;		// CPU アクセス
	BufferDesc.MiscFlags = 0;					// その他のフラグも設定しない

	// バッファを作成する
	hr = _pd3dDevice->CreateBuffer(&BufferDesc, pResource, _ppOutBuffer);

	SHADER_FUNCTION_CHECK_ERROR(hr,"[CreateBuffer]Failed create buffer.\n" );

	if (pResource)
	{
		delete pResource;
	}

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		頂点バッファ作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:28	T.Aoyama	作成
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
/**	@brief		インデックスバッファ作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:35	T.Aoyama	作成
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
/**	@brief		定数バッファ作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:39	T.Aoyama	作成
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
/**	@brief		アンオーダーアクセスバッファの作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/11/15	20:08	T.Aoyama	作成
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

	//構造化バッファ作成
	D3D11_BUFFER_DESC bufferDesc;
	::SecureZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));


	//バッファ情報初期化

	// アンオーダーアクセス ＆ シェーダーリソースビュー＆指定のBindフラグで作成
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | _addBindFlag;
	// バッファの使用方法を指定
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// バッファサイズを指定
	bufferDesc.ByteWidth = _elementSize * _nCount;
	// リソースの指定
	bufferDesc.MiscFlags = _miscFlag;
	//構造体配列のバッファならサイズを指定（要素単位）
	if (bufferDesc.MiscFlags == D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
		bufferDesc.StructureByteStride = _elementSize;


	//初期値を設定
	D3D11_SUBRESOURCE_DATA* pResource = nullptr;
	if (_pInitData)
	{
		//データ初期化
		pResource = new D3D11_SUBRESOURCE_DATA;
		pResource->pSysMem = _pInitData;
		pResource->SysMemPitch = 0;
		pResource->SysMemSlicePitch = 0;
	}
	//バッファの作成
	hr = _pd3dDevice->CreateBuffer(&bufferDesc, pResource, _ppOutBuffer);

	if (pResource)
	{
		delete pResource;
	}

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr,"[ao::CreateUnorderedAccessBuffer]Failed create unordered access buffer.\n" );


	//アンオーダーアクセスビューを作成
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	::SecureZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;	// バッファーであることを指定
	uavDesc.Buffer.FirstElement = 0;					// ビューから最初にアクセスする要素のインデックス
	//書き込み読み込み可能なバイトアドレスバッファーのみ
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
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;		// バッファのフラグ指定
		uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;				// リソースフォーマットの指定する
		uavDesc.Buffer.NumElements = bufferDesc.ByteWidth / 4;	// リソース内の要素数（ベクトル/4）
		break;
	case D3D11_RESOURCE_MISC_BUFFER_STRUCTURED:
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;		// リソースフォーマットの指定する
		uavDesc.Buffer.NumElements = _nCount;		// リソース内の要素数
		break;
	case D3D11_RESOURCE_MISC_RESOURCE_CLAMP:
		break;
	case D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX:
		break;
	case D3D11_RESOURCE_MISC_GDI_COMPATIBLE:
		break;
	}

	//シェーダーリソースビューを作成
	hr = _pd3dDevice->CreateUnorderedAccessView(*_ppOutBuffer, &uavDesc, _ppOutUaView);

	SHADER_FUNCTION_CHECK_ERROR_RETURN(hr,"ao::CreateUnorderedAccessBuffer]Failed create unordered access view.\n" );

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		頂点シェーダーの定数バッファを更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/10	02:27	T.Aoyama	作成
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
/**	@brief		ジオメトリシェーダーの定数バッファを更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/10	02:27	T.Aoyama	作成
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
/**	@brief		ピクセルシェーダーの定数バッファを更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/10	02:27	T.Aoyama	作成
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
/**	@brief		コンピュートシェーダーの定数バッファを更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/10	02:27	T.Aoyama	作成
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
/**	@brief		頂点シェーダーの定数バッファを更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/07/02	00:00	T.Aoyama	作成
//	2013/08/07	04:14	T.Aoyama	引数を追加（_pd3dDeviceContext）
//	2013/08/07	05:47	T.Aoyama	引数にMAPフラグを追加（_mapFlag）
//	2013/09/05	09:47	T.Aoyama	戻り値をHRESULTに変更
//	2014/04/24	09:23	T.Aoyama	処理を最適化
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

	//定数バッファをセット
	_pd3dDeviceContext->VSSetConstantBuffers(_slotId, _numBuffer, _ppBuffer);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		ジオメトリシェーダーの定数バッファを更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:32	T.Aoyama	作成
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

	//定数バッファをセット
	_pd3dDeviceContext->GSSetConstantBuffers(_slotId, _numBuffer, _ppBuffer);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		ピクセルシェーダーの定数バッファを更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/07	11:08	T.Aoyama	作成
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

	//定数バッファをセット
	_pd3dDeviceContext->PSSetConstantBuffers(_slotId, _numBuffer, _ppBuffer);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		コンピュートシェーダーの定数バッファを更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/10	02:33	T.Aoyama	作成
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

	//定数バッファをセット
	_pd3dDeviceContext->CSSetConstantBuffers(_slotId, _numBuffer, _ppBuffer);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		テクスチャの設定関数 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/09	10:56	T.Aoyama	作成
//	2014/04/24	09:23	T.Aoyama	処理を最適化
//------------------------------------------------------------------------------
void ao::SetTexture(
	ID3D11DeviceContext* const _pd3dDeviceContext,
	const UINT _srViewSlot,
	const UINT _sampStateSlot,
	ID3D11ShaderResourceView* const *_ppSrView,
	ID3D11SamplerState* const *_ppSampState)
{
	//テクスチャが存在しない、またはnullセットの場合
	if (_ppSrView == nullptr || _ppSampState == nullptr)
	{
		ID3D11SamplerState*			pNullSampler[] = { nullptr };
		ID3D11ShaderResourceView*	pNullSRView[] = { nullptr };
		_pd3dDeviceContext->PSSetSamplers(_sampStateSlot, 1, pNullSampler);
		_pd3dDeviceContext->PSSetShaderResources(_srViewSlot, 1, pNullSRView);
		return;
	}

	//テクスチャが存在する場合の処理
	_pd3dDeviceContext->PSSetSamplers(_sampStateSlot, 1, _ppSampState);
	_pd3dDeviceContext->PSSetShaderResources(_srViewSlot, 1, _ppSrView);
}