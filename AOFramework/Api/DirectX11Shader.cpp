#include"stdafx.hpp"
#include"DirectX11Shader.hpp"
#include"../Shader/Function/ShaderFunction.hpp"
#include"../Utility/Macro.inl"


using namespace ao;


#ifdef __AO_DEBUG__
#define AO_COMPILE_DEBUG_FLAG D3DCOMPILE_DEBUG
#define DIRECTX11_SHADER_CHECK_ERROR_CREATE_SHADER(_hr,_pShader,_shaderType1,_shaderType2,_filePath)									\
		if (FAILED(_hr))																												\
		{																																\
			delete _pShader;																											\
			ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Shader::Load%sShaderFromFile]\n"),_shaderType1);					\
			ao::debug::PrintfColor(ConsoleColor::L_RED, _T("Failed create %s shader from file \"%s\"\n"),_shaderType2, _filePath);		\
			return nullptr;																												\
		}																																\
		ao::debug::PrintfColor(ConsoleColor::H_GREEN, _T("[DirectX11Shader::Load%sShaderFromFile]\n"), _shaderType1);					\
		ao::debug::PrintfColor(ConsoleColor::L_GREEN, _T("Succeeded create %s shader from file \"%s\"\n"), _shaderType2, _filePath);
#else
#define AO_COMPILE_DEBUG_FLAG D3DCOMPILE_OPTIMIZATION_LEVEL3
#define DIRECTX11_SHADER_CHECK_ERROR_CREATE_SHADER(_hr, _pShader, _shaderType1, _shaderType2, _filePath)
#endif


//------------------------------------------------------------------------------
/**	@brief		登録用のシェーダーパス構築 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/02/26	09:39	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT GenerateFindShaderPath(
	AOLPSTR _pOutFilePath,
	const UINT outFilePathSize,
	const LPCWSTR _pInFilePath,
	const D3D_SHADER_MACRO* _pDefines)
{
	if (_pOutFilePath == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Shader::GenerateFindShaderPath_]\nFailed out put argument is null.(_pOutFilePath)\n"));
		return E_INVALIDARG;
	}

	if (_pDefines)
	{
#ifdef UNICODE
		ao::string name, define;
		std::string inName = _pDefines->Name;
		std::string inDefine = _pDefines->Definition;
		ao::macro::StringMultiToWide(&name, inName);
		ao::macro::StringMultiToWide(&define, inDefine);
		ao_sprintf_s(_pOutFilePath, outFilePathSize, _T("%s_%s_%s"), _pInFilePath, name.c_str(), define.c_str());
#else
		ao_sprintf_s(_pOutFilePath, outFilePathSize, _T("%s_%s_%s"), _pInFilePath, _pDefines->Name, _pDefines->Definition);
#endif
	}
	else
	{
		ao_sprintf_s(_pOutFilePath, outFilePathSize, _T("%s"), _pInFilePath);
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		コンパイル終了時の処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/02/26	09:57	T.Aoyama	作成
//------------------------------------------------------------------------------
template<class T>
HRESULT EndCompileProcess(
	const HRESULT _hr,
	T* const _pDeletePtr,
	const AOLPSTR _shaderName1,
	const AOLPSTR _shaderName2,
	const LPCWSTR _filePath,
	ID3DBlob* const _pErrorMsgs)
{
#ifdef __AO_DEBUG__
	if (FAILED(_hr))
	{
		delete _pDeletePtr;
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Shader::Load%sShaderFromFile]\n"), _shaderName1);
		ao::debug::PrintfColor(ConsoleColor::L_RED, _T("Failed compile %s shader from file \"%s\"\n"),_shaderName2, _filePath);

		if (_pErrorMsgs)
		{
			ao::debug::PrintfColor(ConsoleColor::H_RED, _T("compile error : \"%s\"\n"), reinterpret_cast<AOLPSTR>(_pErrorMsgs->GetBufferPointer()));
		}
	}
	else
	{
		ao::debug::PrintfColor(ConsoleColor::H_GREEN, _T("[DirectX11Shader::Load%sShaderFromFile]\n"), _shaderName1);
		ao::debug::PrintfColor(ConsoleColor::L_GREEN, _T("Succeeded compile %s shader from file \"%s\"\n"), _shaderName2, _filePath);
	}

	return _hr;
#else
	if (FAILED(_hr))
	{
		delete _pDeletePtr;
	}

	return _hr;
#endif
}


DirectX11Shader::DirectX11Shader(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	DirectX11BaseApi(_pd3dDevice,_pd3dDeviceContext)
{

}


DirectX11Shader::~DirectX11Shader()
{
	this->ReleaseUavBuffer();
	this->ReleaseVertexShader();
	this->ReleaseGeometryShader();
	this->ReleasePixelShader();
	this->ReleaseComputeShader();
}


//------------------------------------------------------------------------------
/**	@brief		頂点シェーダーを外部ファイルから読み込み */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/09	00:55	T.Aoyama	作成
//------------------------------------------------------------------------------
VertexShader*	DirectX11Shader::LoadVertexShaderFromFile(
	LPCWSTR _filePath,
	LPCSTR _entryPoint,
	LPCSTR _target,
	ID3DInclude* const _pInclude,
	const D3D_SHADER_MACRO* _pDefines,
	const D3D11_INPUT_ELEMENT_DESC* _pDesc,
	const UINT _size,
	const UINT _flags1,
	const UINT _flags2)
{
	std::array<AOCHAR, 256> findFilePath;
	findFilePath.fill('\0');

	//検索用または登録用のファイルパス生成
	GenerateFindShaderPath(findFilePath.data(), (UINT)findFilePath.size(), _filePath, _pDefines);

	//既存のリソース検索
	if (this->pVertexShaderFileMap.find(findFilePath.data()) != this->pVertexShaderFileMap.end())
	{
		return this->pVertexShaderFileMap[findFilePath.data()];
	}

	HRESULT hr;
	SmartComPtr<ID3DBlob>	pCompiledShader;
	VertexShader* pVertexShader = new VertexShader();
	ID3DBlob* pOutErrorMsgs = nullptr;

	//頂点シェーダー読み込み
	hr = D3DCompileFromFile(
		_filePath,
		_pDefines,
		_pInclude,
		_entryPoint,
		_target,
		_flags1 | AO_COMPILE_DEBUG_FLAG,
		_flags2,
		pCompiledShader.ToCreator(),
		&pOutErrorMsgs);

	//コンパイル終了時の処理
	EndCompileProcess(hr, pVertexShader, _T("Vertex"), _T("vertex"), _filePath, pOutErrorMsgs);

	//頂点シェーダー生成
	hr = this->pd3dDevice->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		nullptr,
		pVertexShader->shader.ToCreator());

#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		delete pVertexShader;
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("DirectX11Shader::LoadVertexShaderFromFile]\nFailed create vertex shader from file \"%s\".\n"), _filePath);
		return nullptr;
	}
#endif

	//定義数算出
	if (_pDesc == nullptr)
	{
		hr = this->CreateAutoInputLayout(
			pVertexShader->layout.ToCreator(),
			pCompiledShader.GetComPtr());
	}
	else
	{
		if (_size == 0)
		{
			ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Shader::LoadVertexShaderFromFile]\nmanual input layout is data size zero.\n"));
			return nullptr;
		}
		hr = this->CreateManualInputLayout(
			pVertexShader->layout.ToCreator(),
			pCompiledShader.GetComPtr(),
			_pDesc,
			_size);
	}

	//作成したシェーダーのエラーチェック
	DIRECTX11_SHADER_CHECK_ERROR_CREATE_SHADER(hr, pVertexShader, _T("Vertex"), _T("vertex"), _filePath);

	// register on vertex shader map
	this->pVertexShaderFileMap.insert(std::map<string, VertexShader*>::value_type(findFilePath.data(), pVertexShader));

	return pVertexShader;
}


//------------------------------------------------------------------------------
/**	@brief		ジオメトリシェーダーファイルの読み込み */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/08/07	15:37	T.Aoyama	作成
//------------------------------------------------------------------------------
GeometryShader* DirectX11Shader::LoadGeometryShaderFromFile(
	LPCWSTR _filePath,
	LPCSTR _entryPoint,
	LPCSTR _target,
	ID3DInclude* const _pInclude,
	const D3D_SHADER_MACRO* _pDefines,
	const UINT _flags1,
	const UINT _flags2)
{
	std::array<AOCHAR, 256> findFilePath;
	findFilePath.fill('\0');

	//検索用または登録用のファイルパス生成
	GenerateFindShaderPath(findFilePath.data(), (UINT)findFilePath.size(), _filePath, _pDefines);

	//既存のリソースを検索
	if (this->pGeometryShaderFileMap.find(findFilePath.data()) != this->pGeometryShaderFileMap.end())
	{
		return this->pGeometryShaderFileMap[findFilePath.data()];
	}

	HRESULT hr;
	SmartComPtr<ID3DBlob> pCompiledShader;
	GeometryShader* pGeometryShader = new GeometryShader();
	ID3DBlob* pOutErrorMsgs = nullptr;

	//頂点シェーダー読み込み
	hr = D3DCompileFromFile(
		_filePath,
		_pDefines,
		_pInclude,
		_entryPoint,
		_target,
		_flags1,
		_flags2,
		pCompiledShader.ToCreator(),
		&pOutErrorMsgs);

	//コンパイル終了時の処理
	hr = EndCompileProcess(hr, pGeometryShader, _T("Geometry"), _T("geometry"), _filePath, pOutErrorMsgs);
	if (FAILED(hr))
	{
		return nullptr;
	}

	//ジオメトリシェーダー作成
	hr = this->pd3dDevice->CreateGeometryShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		nullptr,
		pGeometryShader->shader.ToCreator());

	//作成したシェーダーのエラーチェック
	DIRECTX11_SHADER_CHECK_ERROR_CREATE_SHADER(hr, pGeometryShader, _T("Geometry"), _T("geometry"), _filePath);

	// register on vertex shader map
	this->pGeometryShaderFileMap.insert(std::map<string, GeometryShader*>::value_type(findFilePath.data(), pGeometryShader));

	return pGeometryShader;
}


//------------------------------------------------------------------------------
/**	@brief		ピクセルシェーダー読み込み */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/06/15	00:00	T.Aoyama	作成
//------------------------------------------------------------------------------
PixelShader*	DirectX11Shader::LoadPixelShaderFromFile(
	LPCWSTR _filePath,
	LPCSTR _entryPoint,
	LPCSTR _target,
	ID3DInclude* const _pInclude,
	const D3D_SHADER_MACRO* _pDefines,
	const UINT _flags1,
	const UINT _flags2)
{
	std::array<AOCHAR, 256> findFilePath;
	findFilePath.fill('\0');

	//検索用または登録用のファイルパス生成
	GenerateFindShaderPath(findFilePath.data(), (UINT)findFilePath.size(), _filePath, _pDefines);

	//既存のリソース検索
	if (this->pPixelShaderFileMap.find(findFilePath.data()) != this->pPixelShaderFileMap.end())
	{
		return this->pPixelShaderFileMap[findFilePath.data()];
	}

	HRESULT hr;
	SmartComPtr<ID3DBlob> pCompiledShader;
	PixelShader* pPixelShader = new PixelShader();
	ID3DBlob* pOutErrorMsgs = nullptr;

	//頂点シェーダー読み込み
	hr = D3DCompileFromFile(
		_filePath,
		_pDefines,
		_pInclude,
		_entryPoint,
		_target,
		_flags1,
		_flags2,
		pCompiledShader.ToCreator(),
		&pOutErrorMsgs);

	//コンパイル終了時の処理
	EndCompileProcess(hr, pPixelShader, _T("Pixel"), _T("pixel"), _filePath, pOutErrorMsgs);

#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		delete pPixelShader;
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Shader::LoadPixelShader]\nFailed compile pixel shader from file \"%s\"\n"), _filePath);
		return nullptr;
	}

	ao::debug::PrintfColor(ConsoleColor::L_GREEN, _T("[DirectX11Shader::LoadPixelShader]\nSucceeded compile pixel shader from file \"%s\"\n"), _filePath);
#endif

	//ピクセルシェーダー生成
	hr = this->pd3dDevice->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		nullptr,
		pPixelShader->shader.ToCreator());

	//作成したシェーダーのエラーチェック
	DIRECTX11_SHADER_CHECK_ERROR_CREATE_SHADER(hr, pPixelShader, _T("Pixel"), _T("pixel"), _filePath);

	// register on vertex shader map
	this->pPixelShaderFileMap.insert(std::map<string, PixelShader*>::value_type(findFilePath.data(), pPixelShader));

	return pPixelShader;
}


//------------------------------------------------------------------------------
/**	@brief		コンピュートシェーダーの読み込み */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/11/04	01:50	T.Aoyama	作成
//	2013/12/05	19:16	T.Aoyama	_pDefinesによるシェーダーマクロ実装
//------------------------------------------------------------------------------
ComputeShader* DirectX11Shader::LoadComputeShaderFromFile(
	LPCWSTR _filePath,
	LPCSTR _entryPoint,
	LPCSTR _target,
	ID3DInclude* const _pInclude,
	const D3D_SHADER_MACRO* _pDefines,
	const UINT _flags1,
	const UINT _flags2)
{
	std::array<AOCHAR, 256> findFilePath;
	findFilePath.fill('\0');

	//検索用または登録用のファイルパス生成
	GenerateFindShaderPath(findFilePath.data(), (UINT)findFilePath.size(), _filePath, _pDefines);

	//既存のリソース検索
	if (this->pComputeShaderFileMap.find(findFilePath.data()) != this->pComputeShaderFileMap.end())
	{
		return this->pComputeShaderFileMap[findFilePath.data()];
	}

	HRESULT hr;
	SmartComPtr<ID3DBlob> pCompiledShader;
	ComputeShader* pComputeShader = new ComputeShader();
	ID3DBlob* pOutErrorMsgs = nullptr;

	//コンピュートシェーダー読み込み
	hr = D3DCompileFromFile(
		_filePath,
		_pDefines,
		_pInclude,
		_entryPoint,
		_target,
		_flags1,
		_flags2,
		pCompiledShader.ToCreator(),
		&pOutErrorMsgs);

	//コンパイル終了時の処理
	EndCompileProcess(hr, pComputeShader, _T("Compute"), _T("compute"), _filePath, pOutErrorMsgs);

#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		delete pComputeShader;
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Shader::LoadComputeShaderFromFile]\nFailed compile compute shader from file \"%s\"\n"), _filePath);
		return nullptr;
	}

	ao::debug::PrintfColor(ConsoleColor::H_GREEN, _T("[DirectX11Shader::LoadComputeShaderFromFile]\nSucceeded compile compute shader from file \"%s\"\n"), _filePath);
#endif

	//シェーダー生成
	hr = this->pd3dDevice->CreateComputeShader(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		nullptr,
		pComputeShader->shader.ToCreator());

#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		delete pComputeShader;
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("DirectX11Shader::LoadComputeShaderFromFile]\nFailed create compute shader from file \"%s\".\n"), _filePath);
		return nullptr;
	}
#endif

	// register on vertex shader map
	this->pComputeShaderFileMap.insert(std::map<string, ComputeShader*>::value_type(findFilePath.data(), pComputeShader));

	return pComputeShader;
}


//------------------------------------------------------------------------------
/**	@brief		アンオーダーアクセスバッファの作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/11/15	20:38	T.Aoyama	作成
//------------------------------------------------------------------------------
UavBuffer* DirectX11Shader::CreateUnorderedAccessBuffer(
	const D3D11_USAGE _usage,
	const D3D11_RESOURCE_MISC_FLAG _miscFlag,
	const UINT _elementSize,
	const UINT _nCount,
	const void* _pInitData,
	const D3D11_BIND_FLAG _addBindFlag)
{
	ao::UavBuffer* pUavBuffer = new ao::UavBuffer();

	ao::CreateUnorderedAccessBuffer(
		this->pd3dDevice.GetComPtr(),
		pUavBuffer->buffer.ToCreator(),
		pUavBuffer->view.ToCreator(),
		_miscFlag,
		_elementSize,
		_nCount,
		_pInitData,
		_addBindFlag);

#ifdef __AO_DEBUG__
	if (pUavBuffer->buffer == nullptr || pUavBuffer->view == nullptr)
	{
		delete pUavBuffer;
		return nullptr;
	}
#endif

	this->pUavList.push_back(pUavBuffer);

	return pUavBuffer;
}


//------------------------------------------------------------------------------
/**	@brief		頂点シェーダーリソース解放 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/09	21:00	T.Aoyama	作成
//------------------------------------------------------------------------------
void DirectX11Shader::ReleaseVertexShader()
{
	if (this->pVertexShaderFileMap.empty())
	{
		return;
	}

	for (auto& it : this->pVertexShaderFileMap)
	{
		delete it.second;
	}
}


//------------------------------------------------------------------------------
/**	@brief		ジオメトリシェーダーリソース解放 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/09	21:00	T.Aoyama	作成
//------------------------------------------------------------------------------
void DirectX11Shader::ReleaseGeometryShader()
{
	if (this->pGeometryShaderFileMap.empty())
	{
		return;
	}

	for (auto& it : this->pGeometryShaderFileMap)
	{
		delete it.second;
	}
}


//------------------------------------------------------------------------------
/**	@brief		ピクセルシェーダーリソース解放 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/09	21:00	T.Aoyama	作成
//------------------------------------------------------------------------------
void DirectX11Shader::ReleasePixelShader()
{
	if (this->pPixelShaderFileMap.empty())
	{
		return;
	}

	for (auto& it : this->pPixelShaderFileMap)
	{
		delete it.second;
	}
}


//------------------------------------------------------------------------------
/**	@brief		コンピュートシェーダーリソース解放 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/09	21:00	T.Aoyama	作成
//------------------------------------------------------------------------------
void DirectX11Shader::ReleaseComputeShader()
{
	if (this->pComputeShaderFileMap.empty())
	{
		return;
	}

	for (auto& it : this->pComputeShaderFileMap)
	{
		delete it.second;
	}
}


//------------------------------------------------------------------------------
/**	@brief		アンオーダーアクセスバッファ解放 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/11/15	21:24	T.Aoyama	作成
//------------------------------------------------------------------------------
void DirectX11Shader::ReleaseUavBuffer()
{
	if (this->pUavList.empty())
	{
		return;
	}

	for (auto& it : this->pUavList)
	{
		delete it;
	}
}


//------------------------------------------------------------------------------
/**	@brief		頂点レイアウトの手動作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/10/13	15:20	T.aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11Shader::CreateManualInputLayout(
	ID3D11InputLayout** _ppOut,
	ID3DBlob* _pBlob,
	CONST D3D11_INPUT_ELEMENT_DESC* _pDesc,
	UINT _size
	)
{
	HRESULT hr;
	UINT numElements = _size / sizeof(D3D11_INPUT_ELEMENT_DESC);

	//頂点入力レイアウト作成
	hr = this->pd3dDevice->CreateInputLayout(
		_pDesc,
		numElements,
		_pBlob->GetBufferPointer(),
		_pBlob->GetBufferSize(),
		_ppOut);

#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Shader::CreateManualInputLayout]\nFailed create vertex input layout.\n"));
	}
#endif

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		頂点レイアウトの自動作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/10/13	14:26	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11Shader::CreateAutoInputLayout(
	ID3D11InputLayout** _ppOut,
	ID3DBlob* _pBlob)
{
	HRESULT hr = E_FAIL;
	hr = ao::CreateAutoVertexInputLayout(this->pd3dDevice.GetComPtr(), _ppOut, _pBlob);
#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Shader::CreateAutoInputLayout]\nFaield create auto input layout."));
	}
#endif

	return hr;
}