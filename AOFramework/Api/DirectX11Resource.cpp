#include"stdafx.hpp"
#include"DirectX11Resource.hpp"
#include"../Utility/Macro.inl"
#include"../DirectXTex/DirectXTex.hpp"
#include"../Shader/Function/ShaderFunction.hpp"
#include"../Resource/CutSprite.hpp"
#include"../Loader/XFileLoader.hpp"
#include"../Loader/PmdLoader.hpp"
#include"../Loader/ObjLoader.hpp"
#include"../Loader/FbxLoader.hpp"
#include"../Math/Math.hpp"
#include"../Math/MathDefine.hpp"


using namespace ao;





const AOLPSTR SAMPLER_STRING[] =
{
	_T("[LINEAR]"),
	_T("[ANISOTROPIC]"),
	_T("[POINT]")
};


DirectX11Resource::DirectX11Resource(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) : 
	DirectX11BaseApi(_pd3dDevice,_pd3dDeviceContext)
{
	setlocale(LC_CTYPE, "");
}


DirectX11Resource::~DirectX11Resource()
{
	for (auto& it : this->pStaMotionMap)
	{
		delete it.second;
	}

	for (auto& it : this->pMeshMap)
	{
		delete it.second;
	}

	for (auto& it : this->pSpriteArrayMap)
	{
		delete it.second;
	}

	for (auto& it : this->pSpriteMap)
	{
		delete it.second;
	}

	for (auto& it : this->pTextureMap)
	{
		delete it.second;
	}
}


//------------------------------------------------------------------------------
/**	@brief		������ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/01	20:25	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11Resource::Initialize()
{
	//�f�t�H���g�̃T���v���[�X�e�[�g�쐬
	this->CreateDefaultSamplerState();
	//�X�v���C�g�p�̋��ʒ��_�o�b�t�@�쐬
	this->CreateSpriteVertexBuffer();

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�e�N�X�`���ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/01	20:32	T.Aoyama	�쐬
//------------------------------------------------------------------------------
BaseTexture* DirectX11Resource::LoadTextureFromFile(const string _filePath, const SamplerState _filterType)
{
#ifdef __AO_DEBUG__
	if (_filterType == SamplerState::MAX)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resoure::LoadTextureFromFile]\n_filterType is TYPE MAX %s\n"), _filePath.c_str());
		return nullptr;
	}
#endif

	//�o�^������쐬
	string findPath(_filePath);
	findPath.append(SAMPLER_STRING[(int)_filterType]);

	//�������\�[�X��������΁A�����̃e�N�X�`����Ԃ�
	if (this->pTextureMap.find(findPath) != this->pTextureMap.end())
	{
		return this->pTextureMap[findPath];
	}

	HRESULT hr = E_FAIL;
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	//�e�N�X�`���t�@�C���ǂݍ���
	std::wstring loadStr;
#ifdef UNICODE
	loadStr = _filePath;
#else
	ao::macro::StringMultiToWide(&loadStr, _filePath);
#endif
	hr = DirectX::LoadFromWICFile(loadStr.c_str(), 0, &metaData, image);
#ifdef __AO_DEBUG__
	if (FAILED(hr)){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resoure::LoadTextureFromFile]\nFailed load texture file from %s\n"), _filePath.c_str());
		return nullptr;
	}
#endif

	//�V�F�[�_�[���\�[�X�r���[�쐬
	SmartComPtr<ID3D11ShaderResourceView> srView;
	hr = DirectX::CreateShaderResourceView(
		this->pd3dDevice.GetComPtr(),
		image.GetImages(),
		image.GetImageCount(),
		metaData,
		srView.ToCreator()
		);

	//�e�N�X�`������
	BaseTexture* p = new BaseTexture;
	p->pSrView = srView;
	p->pSampState = this->pDefaultSamplerState[(int)_filterType];
	p->filePath.assign(_filePath);

	//�t�@�C�������o(size*2��UNICODE���l��)
	AOCHAR* name = new AOCHAR[_filePath.size() * 2];
	::SecureZeroMemory(name, _filePath.size());
	ao::macro::GetFileNameFromPath(name, _filePath.size(), _filePath.c_str());
	p->fileName.assign(name);
	delete[] name;

	//MAP�ɓo�^�i�r���I�j
	std::lock_guard<std::mutex> lock(this->textureMutex);
	this->pTextureMap.insert(std::map<string, BaseTexture*>::value_type(findPath, p));

	return p;
}


//------------------------------------------------------------------------------
/**	@brief		�X�v���C�g�̓ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/16	21:59	T.Aoyama	�쐬
//------------------------------------------------------------------------------
Sprite* DirectX11Resource::LoadSpriteFromFile(
	const string _filePath,
	const SpritePivot _pivot,
	const SamplerState _filterType)
{
#ifdef __AO_DEBUG__
	if (_filterType == SamplerState::MAX)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resoure::LoadSpriteFromFile]\n_filterType is TYPE_MAX %s\n"), _filePath.c_str());
		return nullptr;
	}
#endif

	//�o�^������쐬
	string findPath(_filePath);
	findPath.append(SAMPLER_STRING[(int)_filterType]);

	//�������\�[�X��������΁A�����̃e�N�X�`����Ԃ�
	if (this->pSpriteMap.find(findPath) != this->pSpriteMap.end())
	{
		return this->pSpriteMap[findPath];
	}

	Sprite* p = new Sprite();
	p->pTexture = this->LoadTextureFromFile(_filePath, _filterType);
#ifdef __AO_DEBUG__
	if (p->pTexture == nullptr){
		delete p;
		return nullptr;
	}
#endif

	switch (_pivot)
	{
	case SpritePivot::LeftTop:
		p->CopyVertexBufferRef(this->leftTopSprite);
		break;
	case SpritePivot::Center:
		p->CopyVertexBufferRef(this->centerSprite);
		break;
	}

	//MAP�ɓo�^�i�r���I�j
	std::lock_guard<std::mutex> lock(this->spriteMutex);
	this->pSpriteMap.insert(std::map<string, Sprite*>::value_type(findPath, p));

	return p;
}


//------------------------------------------------------------------------------
/**	@brief		�X�v���C�g�̓ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/16	21:59	T.Aoyama	�쐬
//------------------------------------------------------------------------------
Sprite* DirectX11Resource::LoadSpriteFromFile(
	const string _filePath,
	const int _top, const int _left,
	const int _buttom, const int _right,
	const int _xSize, const int _ySize,
	const BOOL _useCache,
	const SpritePivot _pivot,
	const SamplerState _filterType)
{
#ifdef __AO_DEBUG__
	if (_filterType == SamplerState::MAX)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resoure::LoadSpriteFromFile]\n_filterType is TYPE_MAX %s\n"), _filePath.c_str());
		return nullptr;
	}
#endif

	std::array<AOCHAR, 256> findFilePath;
	findFilePath.fill('\0');
	ao_sprintf_s(
		findFilePath.data(),
		findFilePath.size(),
		_T("%s%s_%d%d%d%d%d%d"),
		_filePath.c_str(), SAMPLER_STRING[(int)_filterType],
		_top, _left, _buttom, _right, _xSize, _ySize);

	//�������\�[�X��������΁A�����̃e�N�X�`����Ԃ�
	if (_useCache)
	{
		if (this->pSpriteMap.find(findFilePath.data()) != this->pSpriteMap.end())
			return this->pSpriteMap[findFilePath.data()];
	}

	CutSprite* p = new CutSprite();
	p->pTexture = this->LoadTextureFromFile(_filePath, _filterType);
#ifdef __AO_DEBUG__
	if (p->pTexture == nullptr){
		delete p;
		return nullptr;
	}
#endif

	switch (_pivot)
	{
	case SpritePivot::LeftTop:
		p->CreateCornerCutVertexBuffer(
			this->pd3dDevice.GetComPtr(),
			float2(_left / (float)_xSize, _top / (float)_ySize),
			float2(_right / (float)_xSize, _top / (float)_ySize),
			float2(_left / (float)_xSize, _buttom / (float)_ySize),
			float2(_right / (float)_xSize, _buttom / (float)_ySize));
		break;
	case SpritePivot::Center:
		p->CreateCornerCutVertexBuffer(
			this->pd3dDevice.GetComPtr(),
			float2(_left / (float)_xSize, _top / (float)_ySize),
			float2(_right / (float)_xSize, _top / (float)_ySize),
			float2(_left / (float)_xSize, _buttom / (float)_ySize),
			float2(_right / (float)_xSize, _buttom / (float)_ySize));
		break;
	}

	//MAP�ɓo�^�i�r���I�j
	std::lock_guard<std::mutex> lock(this->spriteMutex);
	this->pSpriteMap.insert(std::map<string, Sprite*>::value_type(findFilePath.data(), p));

	return p;
}


//------------------------------------------------------------------------------
/**	@brief		�X�v���C�g�̓ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/11/03	16:08	T.Aoyama	�쐬
//------------------------------------------------------------------------------
Sprite* DirectX11Resource::LoadDivSpriteFromFile(
	Sprite** _ppOut,
	const int _xDiv,
	const int _yDiv,
	const string _filePath,
	const SpritePivot _pivot,
	const SamplerState _filterType)
{
#ifdef __AO_DEBUG__
	if (_filterType == SamplerState::MAX){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resoure::LoadSpriteFromFile]\n_filterType is TYPE_MAX %s\n"), _filePath.c_str());
		return nullptr;
	}
#endif

	std::array<AOCHAR, 256> findFilePath;
	findFilePath.fill('\0');
	ao_sprintf_s(findFilePath.data(), findFilePath.size(), _T("%s%s_%d%d"), _filePath.c_str(), SAMPLER_STRING[(int)_filterType],_xDiv,_yDiv);
	
	//�������\�[�X��������΁A�����̃e�N�X�`����Ԃ�
	if (this->pSpriteArrayMap.find(findFilePath.data()) != this->pSpriteArrayMap.end())
	{
		SpriteArray* pSpriteArray = this->pSpriteArrayMap[findFilePath.data()];
		for (UINT i = 0; i < pSpriteArray->size(); ++i)
		{
			_ppOut[i] = pSpriteArray->at(i);
		}
		return (*_ppOut);
	}

	//�L���b�V���p
	SpriteArray* pSpriteArray = new SpriteArray(_xDiv*_yDiv);

	//�����ǂݍ���
	for (int i = 0; i < _yDiv; ++i)
	{
		for (int j = 0; j < _xDiv; ++j)
		{
			int index = i*_xDiv + j;
			(*pSpriteArray)[index] = this->LoadSpriteFromFile(_filePath, i, j, i + 1, j + 1, _xDiv, _yDiv, TRUE, _pivot, _filterType);
			_ppOut[index] = (*pSpriteArray)[index];
		}
	}	

	//MAP�ɓo�^�i�r���I�j
	std::lock_guard<std::mutex> lock(this->spriteMutex);
	this->pSpriteArrayMap.insert(std::map<string, SpriteArray*>::value_type(findFilePath.data(), pSpriteArray));

	return (*_ppOut);
}



//------------------------------------------------------------------------------
/**	@brief		X�t�@�C���ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/19	21:26	T.Aoyama	�쐬
//------------------------------------------------------------------------------
StMesh* DirectX11Resource::LoadMeshFromX(const string _filePath)
{
	//�������\�[�X��������΁A�����̃��\�[�X��Ԃ�
	if (this->pMeshMap.find(_filePath) != this->pMeshMap.end())
		return reinterpret_cast<StMesh*>(this->pMeshMap[_filePath]);

	XFileLoader loader;
	if (loader.Load(_filePath) == false)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("Failed load x file from %s\n"), _filePath.c_str());
		return nullptr;
	}

	int index = 0;
	StMesh* pMesh = new StMesh();
	pMesh->geometry.vertex.resize(loader.serialVertex.size());
	auto& vertex = pMesh->geometry.vertex;
	for (auto& it : loader.serialVertex)
	{
		vertex[index].x = it.x;
		vertex[index].y = it.y;
		vertex[index].z = it.z;
		vertex[index].nx = it.nx;
		vertex[index].ny = it.ny;
		vertex[index].nz = it.nz;
		vertex[index].u = it.u;
		vertex[index].v = it.v;
		++index;
	}

	index = 0;
	pMesh->geometry.index.resize(loader.serialIndex.size());
	for (auto& it : loader.serialIndex){
		pMesh->geometry.index[index] = (DWORD)it;
		++index;
	}

	// create vertex buffer
	ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		pMesh->geometry.ToVertexBufferCreator(),
		pMesh->geometry.GetVertexBufferSize(),
		pMesh->geometry.GetVertexBuffer());

	// create index buffer
	ao::CreateIndexBuffer(
		this->pd3dDevice.GetComPtr(),
		pMesh->geometry.ToIndexBufferCreator(),
		pMesh->geometry.GetIndexBufferSize(),
		pMesh->geometry.GetIndexBuffer());

	pMesh->subset.resize(loader.sectionIndex.size());
	for (UINT i = 0; i < loader.sectionIndex.size(); ++i)
	{
		const int numIndex = (int)loader.sectionIndex[i].size();

		// copy material
		pMesh->subset[i].index.resize(numIndex);
		::CopyMemory(pMesh->subset[i].material.diffuse.data(), loader.material[i].diffuse.data(), sizeof(float) * 4);
		::CopyMemory(pMesh->subset[i].material.specular.data(), loader.material[i].specular.data(), sizeof(float) * 4);
		::CopyMemory(pMesh->subset[i].material.emissive.data(), loader.material[i].emissive.data(), sizeof(float) * 3);

		// copy texture
		for (auto& it : pMesh->subset[i].pTextures)
			it = nullptr;
		if (!loader.material[i].texture.empty())
		{
			ao::string texFilePath;
#ifdef UNICODE
			ao::macro::StringMultiToWide(&texFilePath, loader.material[i].texture);
#else
			texFilePath = loader.material[i].texture;
#endif
			pMesh->subset[i].pTextures[0] = this->LoadTextureFromFile(texFilePath);
		}

		// copy section index
		for (int j = 0; j < numIndex; ++j)
			pMesh->subset[i].index[j] = loader.sectionIndex[i][j];
	}

	//MAP�ɓo�^�i�r���I�j
	std::lock_guard<std::mutex> lock(this->meshMutex);
	this->pMeshMap.insert(std::map<string, BaseMesh*>::value_type(_filePath, pMesh));

	return pMesh;
}


//------------------------------------------------------------------------------
/**	@brief		PMD�̓ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/03	19:05	T.Aoyama	�쐬
//------------------------------------------------------------------------------
BaseMesh* DirectX11Resource::LoadMeshFromPmd(const string _filePath, ResourceType _meshType, BOOL _enableFixPath)
{
	if (!(_meshType == ResourceType::ST_MESH || _meshType == ResourceType::SK_MESH)){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resource::LoadMeshFromPmd]_meshType argument is not match.\n"));
		return nullptr;
	}

	//�������\�[�X��������΁A�����̃��\�[�X��Ԃ�
	if (this->pMeshMap.find(_filePath) != this->pMeshMap.end())
		return this->pMeshMap[_filePath];

	PmdLoader loader;
	if (loader.Load(_filePath) == false){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("Failed load pmd file from %s\n"), _filePath.c_str());
		return nullptr;
	}

	BaseMesh* pMesh = nullptr;
	BaseMeshGeometry* pGeometry = nullptr;
	switch (_meshType)
	{
	case ResourceType::ST_MESH:
		pMesh = new StMesh();
		pGeometry = reinterpret_cast<BaseMeshGeometry*>(&reinterpret_cast<StMesh*>(pMesh)->geometry);
		reinterpret_cast<StMesh*>(pMesh)->geometry.vertex.resize(loader.vertexBuffer.size());
		break;
	case ResourceType::SK_MESH:
		pMesh = new SkMesh();
		pGeometry = reinterpret_cast<BaseMeshGeometry*>(&reinterpret_cast<StMesh*>(pMesh)->geometry);
		reinterpret_cast<SkMesh*>(pMesh)->geometry.vertex.resize(loader.vertexBuffer.size());
		break;
	}

	//���_�f�[�^�i�[
	int index = 0;
	for (auto& it : loader.vertexBuffer)
	{
		switch (_meshType)
		{
		case ao::ResourceType::ST_MESH:
		{
			auto& st = reinterpret_cast<StMesh*>(pMesh)->geometry.vertex[index];
			::CopyMemory(st.data, it.baseData, sizeof(st.data));
			break;
		}
		case ao::ResourceType::SK_MESH:
		{
			auto& sk = reinterpret_cast<SkMesh*>(pMesh)->geometry.vertex[index];
			::CopyMemory(sk.data, &it, sizeof(sk.data));
			break;
		}
		}

		++index;
	}

	index = 0;

	pGeometry->index.resize(loader.indexBuffer.size());
	for (auto& it : loader.indexBuffer){
		pGeometry->index[index] = (DWORD)it;
		++index;
	}

	ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		pGeometry->ToVertexBufferCreator(),
		pGeometry->GetVertexBufferSize(),
		pGeometry->GetVertexBuffer());

	// create index buffer
	ao::CreateIndexBuffer(
		this->pd3dDevice.GetComPtr(),
		pGeometry->ToIndexBufferCreator(),
		pGeometry->GetIndexBufferSize(),
		pGeometry->GetIndexBuffer());

	int indexOffset = 0;
	pMesh->subset.resize(loader.materialBuffer.size());
	for (UINT i = 0; i < loader.materialBuffer.size(); ++i)
	{
		const int numIndex = (int)loader.materialBuffer[i].indexCount;

		// copy material
		pMesh->subset[i].index.resize(numIndex);
		::CopyMemory(pMesh->subset[i].material.diffuse.data(), loader.materialBuffer[i].diffuse.data(), sizeof(float) * 4);
		::CopyMemory(pMesh->subset[i].material.specular.data(), loader.materialBuffer[i].specular.data(), sizeof(float) * 4);
		::CopyMemory(pMesh->subset[i].material.emissive.data(), loader.materialBuffer[i].emissive.data(), sizeof(float) * 3);

		// copy texture
		for (auto& it : pMesh->subset[i].pTextures)
			it = nullptr;
		if (!loader.materialBuffer[i].texture.empty())
		{
			ao::string texFilePath;
#ifdef UNICODE
			if (_enableFixPath)
			{
				std::array<AOCHAR, 512> tempPath;
				std::array<AOCHAR, 128> tempName;
				ao::macro::GetFilePathFromPath(&tempPath[0], &_filePath[0]);
				ao::macro::StringMultiToWide(&texFilePath, loader.materialBuffer[i].texture.c_str());
				ao::macro::GetFileNameFromPath(tempName.data(), sizeof(AOCHAR)*tempName.size(),texFilePath.c_str() );
				texFilePath.assign(tempPath.data());
				texFilePath.append(tempName.data());
			}else{
				ao::macro::StringMultiToWide(&texFilePath,loader.materialBuffer[i].texture.c_str());
			}
#else
			if( _enableFixPath){
				std::array<AOCHAR, 512> tempPath;
				std::array<AOCHAR, 128> tempName;
				ao::macro::GetFilePathFromPath(&tempPath[0], &_filePath[0]);
				ao::macro::GetFileNameFromPath(tempName.data(), sizeof(AOCHAR)*tempName.size(), loader.materialBuffer[i].texture.data());
				texFilePath.assign(tempPath.data());
				texFilePath.append(tempName.data());
			}
			else{
				texFilePath = loader.materialBuffer[i].texture;
			}
#endif

			pMesh->subset[i].pTextures[0] = this->LoadTextureFromFile(texFilePath);
		}

		// copy section index
		for (int j = 0; j < numIndex; ++j)
			pMesh->subset[i].index[j] = loader.indexBuffer[indexOffset + j];
		indexOffset += numIndex;
	}

	//MAP�ɓo�^�i�r���I�j
	std::lock_guard<std::mutex> lock(this->meshMutex);
	this->pMeshMap.insert(std::map<string, BaseMesh*>::value_type(_filePath, pMesh));

	return pMesh;
}


//------------------------------------------------------------------------------
/**	@brief		Obj�t�@�C���̓ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/26	19:18	T.Aoyama	�쐬
//------------------------------------------------------------------------------
StMesh* DirectX11Resource::LoadMeshFromObj(const string _filePath, const string _mtlPath)
{
#if 0	//obj�̎d�l���o���o�����o�O���Ă���̂œ���
	ObjLoader loader;
	loader.Load(_filePath, _mtlPath);
#endif

	return nullptr;
}


//------------------------------------------------------------------------------
/**	@brief		Fbx�t�@�C���̓ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/10	13:58	T.Aoyama	�쐬
//------------------------------------------------------------------------------
BaseMesh* DirectX11Resource::LoadMeshFromFbx(
	const string _filePath,
	ResourceType _meshType,
	BOOL _enbaleFixPath,
	IMeshMotionController* _pMotionCtrl)
{
	if (!(_meshType == ResourceType::ST_MESH || _meshType == ResourceType::SK_MESH || _meshType == ResourceType::STA_MESH)){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resource::LoadMeshFromFbx]_meshType argument is not match.\n"));
		return nullptr;
	}

	StaMeshMotionController* st_cast(nullptr);

	switch (_meshType)
	{
	case ao::ResourceType::ST_MESH:
		break;
	case ao::ResourceType::STA_MESH:
		st_cast = reinterpret_cast<StaMeshMotionController*>(_pMotionCtrl);
		break;
	case ao::ResourceType::SK_MESH:
		//������
		break;
	}

	auto StoreCacheMotionData = [&]()
	{
		if (this->pStaMotionMap.find(_filePath) == this->pStaMotionMap.end())
			return;

		auto& cache = this->pStaMotionMap[_filePath];
		st_cast->motionData.list.clear();
		st_cast->motionData.list.resize(cache->list.size());
		st_cast->enable = TRUE;
		st_cast->numMesh = cache->numMesh;
		st_cast->mapData.resize(cache->numMesh);
		for (auto& it : st_cast->mapData)
			ao::MatrixIdentity(&it);
		for (UINT i = 0; i < cache->list.size(); ++i)
		{
			st_cast->motionData.list[i].beginFrame = cache->list[i].beginFrame;
			st_cast->motionData.list[i].endFrame = cache->list[i].endFrame;
			st_cast->motionData.list[i].frameCount = cache->list[i].frameCount;
			auto& srcFrame = cache->list[i].frame;
			auto& dstFrame = st_cast->motionData.list[i].frame;
			dstFrame.resize(srcFrame.size());
			for (UINT j = 0; j < cache->list[i].frame.size(); ++j){
				dstFrame[j].mtx.resize(srcFrame[j].mtx.size());
				::CopyMemory(dstFrame[j].mtx.data(), srcFrame[j].mtx.data(), sizeof(XMFLOAT4X4)*dstFrame[j].mtx.size());
			}
		}
	};

	//�������\�[�X��������΁A�����̃��\�[�X��Ԃ�
	if (this->pMeshMap.find(_filePath) != this->pMeshMap.end()){
		if (_meshType == ResourceType::STA_MESH && st_cast != nullptr)
			StoreCacheMotionData();
		return this->pMeshMap[_filePath];
	}

	FbxLoader loader;
	int loadType = (int)FbxLoader::Type::Mesh | (int)((_pMotionCtrl) ? FbxLoader::Type::Motion : 0);
	if (loader.Load(_filePath, loadType) == false){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("Failed load fbx file from %s\n"), _filePath.c_str());
		return nullptr;
	}

	BaseMesh* pMesh(nullptr);
	StMesh* pTempSt(nullptr);
	StaMesh* pTempSta(nullptr);
	SkMesh* pTempSk(nullptr);
	BaseMeshGeometry* pGeometry(nullptr);
	switch (_meshType)
	{
	case ResourceType::ST_MESH:
		pMesh = new StMesh();
		pTempSt = reinterpret_cast<StMesh*>(pMesh);
		pGeometry = reinterpret_cast<BaseMeshGeometry*>(&pTempSt->geometry);
		pTempSt->geometry.vertex.resize(loader.buildMesh.vertex.size());
		break;
	case ResourceType::STA_MESH:
		pMesh = new StaMesh();
		pTempSta = reinterpret_cast<StaMesh*>(pMesh);
		pGeometry = reinterpret_cast<BaseMeshGeometry*>(&pTempSta->geometry);
		pTempSta->geometry.vertex.resize(loader.buildMesh.vertex.size());
		pTempSta->meshIndex.resize(loader.buildMesh.meshIndex.size());
		::CopyMemory(pTempSta->meshIndex.data(), loader.buildMesh.meshIndex.data(), sizeof(int)*pTempSta->meshIndex.size());
		break;
	case ResourceType::SK_MESH:
		pMesh = new SkMesh();
		pTempSk = reinterpret_cast<SkMesh*>(pMesh);
		pGeometry = reinterpret_cast<BaseMeshGeometry*>(&pTempSk->geometry);
		pTempSk->geometry.vertex.resize(loader.buildMesh.vertex.size());
		break;
	}

	//���_�f�[�^�i�[
	int index = 0;
	for (auto& it : loader.buildMesh.vertex)
	{
		switch (_meshType)
		{
		case ao::ResourceType::ST_MESH:
		case ao::ResourceType::STA_MESH:
		{
			auto& st = reinterpret_cast<StMesh*>(pMesh)->geometry.vertex[index];
			::CopyMemory(st.data, it.baseData, sizeof(st.data));
			break;
		}
		case ao::ResourceType::SK_MESH:
		{
			auto& sk = reinterpret_cast<SkMesh*>(pMesh)->geometry.vertex[index];
			::CopyMemory(sk.data, &it, sizeof(sk.data));
			break;
		}
		}

		++index;
	}

	auto ReadMotion = [&]()
	{
		if (loader.animation.empty() || loader.enableAnimation == FALSE)
			return TRUE;

		st_cast->motionData.list.clear();
		st_cast->motionData.list.resize(loader.animation.size());
		int index = 0;
		auto& motion = st_cast->motionData.list[index];
		for (auto& it : loader.animation)
		{
			motion.beginFrame = it.beginFrame;
			motion.endFrame = it.endFrame;
			motion.frameCount = it.frameCount;
			motion.frame.resize(it.frame.size());
			for (UINT i = 0; i < motion.frame.size(); ++i){
				motion.frame[i].mtx.resize(it.frame[i].mtx.size());
				::CopyMemory(motion.frame[i].mtx.data(), it.frame[i].mtx.data(), sizeof(XMFLOAT4X4)*motion.frame[i].mtx.size());
			}
			++index;
		}

		return TRUE;
	};

	auto CacheMotionData = [&]()
	{
		StaMotionData* pStaMotion = new StaMotionData;
		pStaMotion->numMesh = loader.numMesh;
		if (st_cast->motionData.list.size())
		{
			pStaMotion->list.resize(st_cast->motionData.list.size());
			auto& motion = st_cast->motionData.list;
			for (UINT i = 0; i < motion.size(); ++i)
			{
				pStaMotion->list[i].beginFrame = motion[i].beginFrame;
				pStaMotion->list[i].endFrame = motion[i].endFrame;
				pStaMotion->list[i].frameCount = motion[i].frameCount;
				auto& srcFrame = motion[i].frame;
				auto& dstFrame = pStaMotion->list[i].frame;
				dstFrame.resize(srcFrame.size());
				for (int j = 0; j < motion[i].frameCount; ++j){
					dstFrame[j].mtx.resize(srcFrame[j].mtx.size());
					::CopyMemory(dstFrame[j].mtx.data(), srcFrame[j].mtx.data(), sizeof(XMFLOAT4X4)*dstFrame[j].mtx.size());
				}
			}
		}

		std::lock_guard<std::mutex> lock(this->meshMutex);
		this->pStaMotionMap.insert(std::map<string, StaMotionData*>::value_type(_filePath, pStaMotion));
	};

	//���[�V�����̓ǂݍ���
	if (_meshType == ao::ResourceType::STA_MESH && st_cast != nullptr){
		st_cast->Initialize();
		st_cast->enable = ReadMotion();
		st_cast->numMesh = loader.numMesh;
		st_cast->mapData.resize(loader.numMesh);
		for (auto& it : st_cast->mapData)
			ao::MatrixIdentity(&it);
		if (st_cast->enable)
			CacheMotionData();
	}


	//�C���f�b�N�X�ۑ�
	pGeometry->index.resize(loader.buildMesh.index.size());
	::CopyMemory(pGeometry->index.data(), loader.buildMesh.index.data(), sizeof(int)*pGeometry->index.size());

	//���_�o�b�t�@�쐬
	ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		pGeometry->ToVertexBufferCreator(),
		(UINT)pGeometry->GetVertexBufferSize(),
		pGeometry->GetVertexBuffer());

	//�ÓI���b�V���A�j���[�V�����������b�V���C���f�b�N�X�i�[
	if (_meshType == ao::ResourceType::STA_MESH && pTempSta != nullptr)
	{
		//���[�V�������Ȃ���΃��b�V���C���f�b�N�X��-1�ɍč\�z
		if (st_cast && st_cast->enable == FALSE)
			for (auto& it : pTempSta->meshIndex)
				it = -1;

		ao::CreateVertexBuffer(
			this->pd3dDevice.GetComPtr(),
			pTempSta->meshIndexBuffer.ToCreator(),
			(UINT)pTempSta->GetMeshIndexSize(),
			pTempSta->GetMeshIndexData());
	}

	// �C���f�b�N�X�o�b�t�@�쐬
	ao::CreateIndexBuffer(
		this->pd3dDevice.GetComPtr(),
		pGeometry->ToIndexBufferCreator(),
		(UINT)pGeometry->GetIndexBufferSize(),
		pGeometry->GetIndexBuffer());

	auto ReadTexture = [&](int _matIndex, int _texIndex, const std::string& _texture)
	{
		pMesh->subset[_matIndex].pTextures[_texIndex] = nullptr;

		if (!_texture.empty())
		{
			ao::string texFilePath;
#ifdef UNICODE
			ao::macro::StringMultiToWide(&texFilePath, _texture);
#else
			texFilePath = _texture;
#endif
			ao::string loadPath;

			if (_enbaleFixPath){
				std::array<AOCHAR, 128> fileName;
				std::array<AOCHAR, 512> filePath;
				ao::macro::GetFilePathFromPath(filePath.data(), _filePath.c_str());
				ao::macro::GetFileNameFromPath(fileName.data(), sizeof(AOCHAR)*fileName.size(), texFilePath.c_str());
				loadPath.assign(filePath.data());
				loadPath.append(fileName.data());
			}
			else{
				loadPath = texFilePath;
			}

			pMesh->subset[_matIndex].pTextures[_texIndex] = this->LoadTextureFromFile(loadPath);
		}
};

	int indexOffset = 0;
	pMesh->subset.resize(loader.buildMesh.material.size());
	for (UINT i = 0; i < loader.buildMesh.material.size(); ++i)
	{
		const int numIndex = (int)loader.buildMesh.material[i].indexCount;

		// copy material
		pMesh->subset[i].index.resize(numIndex);
		::CopyMemory(pMesh->subset[i].material.diffuse.data(), loader.buildMesh.material[i].diffuse.data(), sizeof(float) * 4);
		::CopyMemory(pMesh->subset[i].material.ambient.data(), loader.buildMesh.material[i].ambient.data(), sizeof(float) * 3);
		::CopyMemory(pMesh->subset[i].material.specular.data(), loader.buildMesh.material[i].specular.data(), sizeof(float) * 3);
		::CopyMemory(pMesh->subset[i].material.emissive.data(), loader.buildMesh.material[i].emissive.data(), sizeof(float) * 3);

		// copy texture
		ReadTexture(i, 0, loader.buildMesh.material[i].diffuseTexPath);
		ReadTexture(i, 1, loader.buildMesh.material[i].specularTexPath);
		ReadTexture(i, 2, loader.buildMesh.material[i].emissiveTexPath);
		ReadTexture(i, 3, loader.buildMesh.material[i].normalTexPath);
		ReadTexture(i, 4, loader.buildMesh.material[i].bumpTexPath);

		// copy section index
		for (int j = 0; j < numIndex; ++j)
			pMesh->subset[i].index[j] = loader.buildMesh.index[indexOffset + j];
		indexOffset += numIndex;
	}

	//MAP�ɓo�^�i�r���I�j
	std::lock_guard<std::mutex> lock(this->meshMutex);
	this->pMeshMap.insert(std::map<string, BaseMesh*>::value_type(_filePath, pMesh));

	return pMesh;
}


//------------------------------------------------------------------------------
/**	@brief		FBX���[�V������ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/11/03	02:37	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11Resource::LoadMotionFromFbx(IMeshMotionController* _pOut, const string _filePath, const ResourceType _meshType, const BOOL _isAdd)
{
	if (!(_meshType == ResourceType::SK_MESH || _meshType == ResourceType::STA_MESH)){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resource::LoadMotionFromFbx]_meshType argument is not match.\n"));
		return E_INVALIDARG;
	}

	StaMeshMotionController* st_cast(nullptr);

	switch (_meshType)
	{
	case ao::ResourceType::STA_MESH:
		st_cast = reinterpret_cast<StaMeshMotionController*>(_pOut);
		break;
	case ao::ResourceType::SK_MESH:
		//������
		break;
	}


	auto StoreCacheMotionData = [&]()
	{
		if (this->pStaMotionMap.find(_filePath) == this->pStaMotionMap.end())
			return FALSE;

		auto& cache = this->pStaMotionMap[_filePath];


		size_t indexOffset(0);
		if (_isAdd)
		{
			indexOffset = st_cast->motionData.list.size();
			for (UINT i = 0; i < cache->list.size(); ++i)
				st_cast->motionData.list.push_back(StaMotion());
		}
		else
		{
			st_cast->motionData.list.clear();
			st_cast->motionData.list.resize(cache->list.size());
		}

		st_cast->enable = TRUE;
		st_cast->numMesh = cache->numMesh;
		st_cast->mapData.resize(cache->numMesh);
		for (UINT i = 0; i < cache->list.size(); ++i)
		{
			UINT index = (UINT)(i + indexOffset);
			st_cast->motionData.list[index].beginFrame = cache->list[i].beginFrame;
			st_cast->motionData.list[index].endFrame = cache->list[i].endFrame;
			st_cast->motionData.list[index].frameCount = cache->list[i].frameCount;
			auto& srcFrame = cache->list[i].frame;
			auto& dstFrame = st_cast->motionData.list[index].frame;
			dstFrame.resize(srcFrame.size());
			for (UINT j = 0; j < cache->list[i].frame.size(); ++j)
			{
				dstFrame[j].mtx.resize(srcFrame[j].mtx.size());
				::CopyMemory(dstFrame[j].mtx.data(), srcFrame[j].mtx.data(), sizeof(XMFLOAT4X4)*dstFrame[j].mtx.size());
			}
		}

		st_cast->nowMotionIndex = 0;

		return TRUE;
	};


	//�������\�[�X��������΁A�����̃��\�[�X��Ԃ�
	if (_meshType == ResourceType::STA_MESH && st_cast != nullptr)
		if (StoreCacheMotionData())
			return S_FALSE;


	FbxLoader loader;
	if (loader.Load(_filePath, FbxLoader::Type::Motion) == false){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("Failed load fbx file from %s\n"), _filePath.c_str());
		return E_FAIL;
	}


	auto ReadMotion = [&]()
	{
		if (loader.animation.empty() || loader.enableAnimation == FALSE)
			return FALSE;

		size_t indexOffset(0);
		if (_isAdd)
		{
			indexOffset = st_cast->motionData.list.size();
			for (UINT i = 0; i < loader.animation.size(); ++i)
				st_cast->motionData.list.push_back(StaMotion());
		}
		else
		{
			st_cast->motionData.list.clear();
			st_cast->motionData.list.resize(loader.animation.size());
		}

		auto& motion = st_cast->motionData.list;
		for (UINT i = 0; i < loader.animation.size(); ++i)
		{
			UINT index = (UINT)(i + indexOffset);
			st_cast->motionData.list[indexOffset].beginFrame = loader.animation[i].beginFrame;
			st_cast->motionData.list[indexOffset].endFrame = loader.animation[i].endFrame;
			st_cast->motionData.list[indexOffset].frameCount = loader.animation[i].frameCount;
			auto& srcFrame = loader.animation[i].frame;
			auto& dstFrame = st_cast->motionData.list[indexOffset].frame;
			dstFrame.resize(srcFrame.size());
			for (UINT j = 0; j < loader.animation[i].frame.size(); ++j)
			{
				dstFrame[j].mtx.resize(srcFrame[j].mtx.size());
				::CopyMemory(dstFrame[j].mtx.data(), srcFrame[j].mtx.data(), sizeof(XMFLOAT4X4)*dstFrame[j].mtx.size());
			}
		}

		return TRUE;
	};

	auto CacheMotionData = [&]()
	{
		StaMotionData* pStaMotion = new StaMotionData;
		pStaMotion->numMesh = loader.numMesh;
		pStaMotion->list.resize(st_cast->motionData.list.size());
		auto& motion = st_cast->motionData.list;
		for (UINT i = 0; i < motion.size(); ++i)
		{
			pStaMotion->list[i].beginFrame = motion[i].beginFrame;
			pStaMotion->list[i].endFrame = motion[i].endFrame;
			pStaMotion->list[i].frameCount = motion[i].frameCount;
			auto& srcFrame = motion[i].frame;
			auto& dstFrame = pStaMotion->list[i].frame;
			dstFrame.resize(srcFrame.size());
			for (int j = 0; j < motion[i].frameCount; ++j)
			{
				dstFrame[j].mtx.resize(srcFrame[j].mtx.size());
				::CopyMemory(dstFrame[j].mtx.data(), srcFrame[j].mtx.data(), sizeof(XMFLOAT4X4)*dstFrame[j].mtx.size());
			}
		}

		std::lock_guard<std::mutex> lock(this->meshMutex);
		this->pStaMotionMap.insert(std::map<string, StaMotionData*>::value_type(_filePath, pStaMotion));
	};

	//���[�V�����̓ǂݍ���
	if (_meshType == ao::ResourceType::STA_MESH && st_cast != nullptr)
	{
		st_cast->Initialize();
		st_cast->enable = ReadMotion();
		st_cast->numMesh = loader.numMesh;
		st_cast->mapData.resize(loader.numMesh);
		if (st_cast->enable)
		{
			CacheMotionData();
		}
	}


	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�f�t�H���g�̃T���v���[�X�e�[�g�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/01	20:54	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	HRESULT DirectX11Resource::CreateDefaultSamplerState()
{
	HRESULT hr;
	D3D11_SAMPLER_DESC desc;

	//�e�N�X�`���̃T���v���X�e�[�g��ݒ�
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 �` 1 �͈̔͊O�ɂ��� u �e�N�X�`���[���W�̕`����@
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 �` 1 �͈̔͊O�ɂ��� v �e�N�X�`���[���W
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 �` 1 �͈̔͊O�ɂ��� w �e�N�X�`���[���W
	desc.MipLODBias = 0;								// �v�Z���ꂽ�~�b�v�}�b�v ���x������̃o�C�A�X
	desc.MaxAnisotropy = 16;							// �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;		// ��r�I�v�V�����B
	// ���E�F���o�[�X�g�R�s�[
	::SecureZeroMemory(desc.BorderColor, sizeof(float) * 4);
	//�~�b�v�}�b�v�̐ݒ�
	desc.MinLOD = 0;					// �A�N�Z�X�\�ȃ~�b�v�}�b�v�̉����l
	desc.MaxLOD = D3D11_FLOAT32_MAX;	// �A�N�Z�X�\�ȃ~�b�v�}�b�v�̏���l

	//�T���v���[�X�e�[�g�쐬
	auto& samplerState = this->pDefaultSamplerState;

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = this->pd3dDevice->CreateSamplerState(&desc, samplerState[(int)SamplerState::LINEAR].ToCreator());
#ifdef __AO_DEBUG__
	if (FAILED(hr)){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resource::CreateDefaultSamplerState]\nFailed create sampler state.[LINEAR]\n"));
		return hr;
	}
#endif

	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	hr = this->pd3dDevice->CreateSamplerState(&desc, samplerState[(int)SamplerState::ANISOTROPIC].ToCreator());
#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resource::CreateDefaultSamplerState]\nFailed create sampler state.[ANISOTROPIC]\n"));
		return hr;
	}
#endif

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = this->pd3dDevice->CreateSamplerState(&desc, samplerState[(int)SamplerState::POINT].ToCreator());
#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resource::CreateDefaultSamplerState]\nFailed create sampler state.[POINT]\n"));
		return hr;
	}
#endif

	return hr;
}

//------------------------------------------------------------------------------
/**	@brief		�X�v���C�g�p�̒��_�o�b�t�@�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/16	22:09	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11Resource::CreateSpriteVertexBuffer()
{
	HRESULT hr(E_FAIL);
	hr = this->leftTopSprite.CreateCornerVertexBuffer(this->pd3dDevice.GetComPtr());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create sprite vertex buffer[Left top]\n"));


	hr = this->leftTopSprite.CreateCenterVertexBuffer(this->pd3dDevice.GetComPtr());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create sprite vertex buffer[Center]\n"));


	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�{�b�N�X���b�V���ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/14	02:31	T.Aoyama	�쐬
//------------------------------------------------------------------------------
StMesh* DirectX11Resource::LoadBoxMesh()
{
	if (this->pMeshMap.find(_T("BoxMesh")) != this->pMeshMap.end())
		return reinterpret_cast<StMesh*>(this->pMeshMap[_T("BoxMesh")]);

	//     		���_�C���f�b�N�X���
	//     10,13,17________4,6,9,18
	//           /|        / |
	//  1,14,16 / |_2,5,19/  |
	//         |�P|_______|__|7,8,21
	//         | /11,12,22|  / 
	//         |/_________| /
	//      0,15,23      3,4,20

	StMesh* pMesh = new StMesh();
	pMesh->geometry.index.resize(36);
	pMesh->geometry.vertex.resize(24);

	for (int i = 0; i < 6; ++i)
	{
		int vIndex = i * 4;
		int idx = i * 6;
		pMesh->geometry.index[idx + 0] = vIndex + 0;
		pMesh->geometry.index[idx + 1] = vIndex + 1;
		pMesh->geometry.index[idx + 2] = vIndex + 2;

		pMesh->geometry.index[idx + 3] = vIndex + 2;
		pMesh->geometry.index[idx + 4] = vIndex + 3;
		pMesh->geometry.index[idx + 5] = vIndex + 0;
	}

	auto StoreVertex = [&](
		int _idx,
		float _x, float _y, float _z,
		float _nx, float _ny, float _nz,
		float _u, float _v)
	{
		pMesh->geometry.vertex[_idx].x = _x;
		pMesh->geometry.vertex[_idx].y = _y;
		pMesh->geometry.vertex[_idx].z = _z;
		pMesh->geometry.vertex[_idx].nx = _nx;
		pMesh->geometry.vertex[_idx].ny = _ny;
		pMesh->geometry.vertex[_idx].nz = _nz;
		pMesh->geometry.vertex[_idx].u = _u;
		pMesh->geometry.vertex[_idx].v = _v;

	};

	StoreVertex(0, -1.0f, -1.0f, -1.0f, 0, 0, -1, 0, 1);
	StoreVertex(1, -1.0f, +1.0f, -1.0f, 0, 0, -1, 0, 0);
	StoreVertex(2, +1.0f, +1.0f, -1.0f, 0, 0, -1, 1, 1);
	StoreVertex(3, +1.0f, -1.0f, -1.0f, 0, 0, -1, 0, 0);

	StoreVertex(4, +1.0f, -1.0f, -1.0f, 1, 0, 0, 0, 1);
	StoreVertex(5, +1.0f, +1.0f, -1.0f, 1, 0, 0, 0, 0);
	StoreVertex(6, +1.0f, +1.0f, +1.0f, 1, 0, 0, 1, 1);
	StoreVertex(7, +1.0f, -1.0f, +1.0f, 1, 0, 0, 0, 0);

	StoreVertex(8, +1.0f, -1.0f, +1.0f, 0, 0, 1, 0, 1);
	StoreVertex(9, +1.0f, +1.0f, +1.0f, 0, 0, 1, 0, 0);
	StoreVertex(10, -1.0f, +1.0f, +1.0f, 0, 0, 1, 1, 1);
	StoreVertex(11, -1.0f, -1.0f, +1.0f, 0, 0, 1, 0, 0);

	StoreVertex(12, -1.0f, -1.0f, +1.0f, -1, 0, 0, 0, 1);
	StoreVertex(13, -1.0f, +1.0f, +1.0f, -1, 0, 0, 0, 0);
	StoreVertex(14, -1.0f, +1.0f, -1.0f, -1, 0, 0, 1, 1);
	StoreVertex(15, -1.0f, -1.0f, -1.0f, -1, 0, 0, 0, 0);

	StoreVertex(16, -1.0f, +1.0f, -1.0f, 0, 1, 0, 0, 1);
	StoreVertex(17, -1.0f, +1.0f, +1.0f, 0, 1, 0, 0, 0);
	StoreVertex(18, +1.0f, +1.0f, +1.0f, 0, 1, 0, 1, 1);
	StoreVertex(19, +1.0f, +1.0f, -1.0f, 0, 1, 0, 0, 0);

	StoreVertex(20, +1.0f, -1.0f, -1.0f, 0, -1, 0, 0, 1);
	StoreVertex(21, +1.0f, -1.0f, +1.0f, 0, -1, 0, 0, 0);
	StoreVertex(22, -1.0f, -1.0f, +1.0f, 0, -1, 0, 1, 1);
	StoreVertex(23, -1.0f, -1.0f, -1.0f, 0, -1, 0, 0, 0);

	ao::CreateIndexBuffer(
		this->pd3dDevice.GetComPtr(),
		pMesh->geometry.ToIndexBufferCreator(),
		pMesh->geometry.GetIndexBufferSize(),
		pMesh->geometry.GetIndexBuffer());

	ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		pMesh->geometry.ToVertexBufferCreator(),
		pMesh->geometry.GetVertexBufferSize(),
		pMesh->geometry.GetVertexBuffer());

	pMesh->subset.resize(1);
	pMesh->subset[0].index = pMesh->geometry.index;
	pMesh->subset[0].material.diffuse[0] = 0.8f;
	pMesh->subset[0].material.diffuse[1] = 0.8f;
	pMesh->subset[0].material.diffuse[2] = 0.8f;
	pMesh->subset[0].material.diffuse[3] = 1;

	pMesh->subset[0].material.specular[0] = 0.0f;
	pMesh->subset[0].material.specular[1] = 0.0f;
	pMesh->subset[0].material.specular[2] = 0.0f;
	pMesh->subset[0].material.specular[3] = 1;

	pMesh->subset[0].material.emissive[0] = 0;
	pMesh->subset[0].material.emissive[1] = 0;
	pMesh->subset[0].material.emissive[2] = 0;

	for (auto& it : pMesh->subset[0].pTextures)
		it = nullptr;

	std::lock_guard<std::mutex> lock(this->meshMutex);
	this->pMeshMap.insert(std::map<string, BaseMesh*>::value_type(_T("BoxMesh"), pMesh));

	return pMesh;
}


//------------------------------------------------------------------------------
/**	@brief		�X�t�B�A���b�V���ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/14	02:32	T.Aoyama	�쐬
//------------------------------------------------------------------------------
StMesh* DirectX11Resource::LoadSphereMesh(int _division)
{
	std::array<AOCHAR, 256> findFilePath;
	findFilePath.fill('\0');

	ao_sprintf_s(findFilePath.data(), findFilePath.size(), _T("%s_div[%d]\0"), _T("SphereMesh"), _division);
	if (this->pMeshMap.find(findFilePath.data()) != this->pMeshMap.end())
		return reinterpret_cast<StMesh*>(this->pMeshMap[findFilePath.data()]);

	if (_division < 2){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resource::LoadSphereMesh]_division argument is less than 2\n"));
		return nullptr;
	}

	//���_���v�Z( �㕔�Ɖ�����2���_+(�c�����̕���*���������̕���) )
	UINT numVertices = 2 + (_division - 1)*(_division * 2 + 1);

	StMesh* pMesh = new StMesh();
	pMesh->geometry.vertex.resize(numVertices);

	//�㕔�̒��_
	UINT index = 0;
	pMesh->geometry.vertex[index].x = 0;
	pMesh->geometry.vertex[index].y = 1;
	pMesh->geometry.vertex[index].z = 0;

	ao::float3 upNormal(0, 1, 0);
	ao::Vector3Normalize(&upNormal.xm, upNormal.xm);
	pMesh->geometry.vertex[index].nx = upNormal.x;
	pMesh->geometry.vertex[index].ny = upNormal.y;
	pMesh->geometry.vertex[index].nz = upNormal.z;

	pMesh->geometry.vertex[index].u = 0;
	pMesh->geometry.vertex[index].v = 0;

	++index;

	float r = ao::PI / (float)_division;
	float y = 0;
	float z = 0;

	auto CalcVertex = [&](int _j)
	{
		auto& v = pMesh->geometry.vertex[index];
		v.x = z * ::sinf(r*(float)_j);
		v.y = y;
		v.z = z * ::cosf(r*(float)_j);
		ao::float3 n(v.x, v.y, v.z);
		ao::Vector3Normalize(&n.xm, n.xm);
		v.nx = n.x;
		v.ny = n.y;
		v.nz = n.z;
	};

	auto StoreVertex = [&]()
	{
		auto& v = pMesh->geometry.vertex;
		v[index].x = v[index - _division * 2].x;
		v[index].y = v[index - _division * 2].y;
		v[index].z = v[index - _division * 2].z;
		v[index].nx = v[index - _division * 2].nx;
		v[index].ny = v[index - _division * 2].ny;
		v[index].nz = v[index - _division * 2].nz;
	};

	//�c�����̕������[�v
	for (int i = 1; i < _division; ++i)
	{
		y = ::cosf(r * (float)i);
		z = ::sinf(r * (float)i);

		//���������̕������[�v
		for (int j = 0; j < _division * 2 + 1; ++j)
		{
			if (j != _division * 2)
				CalcVertex(j);
			else
				StoreVertex();

			auto& v = pMesh->geometry.vertex;
			v[index].u = (float)j / (float)(_division * 2);
			v[index].v = (float)i / (float)(_division);

			++index;
		}
	}

	//�����̒��_
	pMesh->geometry.vertex[index].x = 0;
	pMesh->geometry.vertex[index].y = -1;
	pMesh->geometry.vertex[index].z = 0;

	ao::float3 downNormal(0, -1, 0);
	ao::Vector3Normalize(&downNormal.xm, downNormal.xm);
	pMesh->geometry.vertex[index].nx = downNormal.x;
	pMesh->geometry.vertex[index].ny = downNormal.y;
	pMesh->geometry.vertex[index].nz = downNormal.z;

	pMesh->geometry.vertex[index].u = 0;
	pMesh->geometry.vertex[index].v = 0;

	//�o�O�`�F�b�N
	if (pMesh->geometry.vertex.size() != index + 1){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resource::LoadSphereMesh]Failed build vertex\n"));
		return nullptr;
	}

	//�C���f�b�N�X�����v�Z(�㕔���̃|���S��+�㉺�ȊO�̕����̃|���S��)
	pMesh->geometry.index.resize(3 * _division * 2 * 2 + 3 * (_division - 2)*_division * 2 * 2);


	//�㕔
	index = 0;
	for (int i = 0; i < _division * 2; ++i)
	{
		pMesh->geometry.index[index++] = 0;
		pMesh->geometry.index[index++] = i + 1;
		pMesh->geometry.index[index++] = i + 2;
	}

	//����
	for (int i = 0; i < _division - 2; ++i)
	{
		for (int j = 0; j < _division * 2; ++j)
		{
			pMesh->geometry.index[index++] = 1 + i * (_division * 2 + 1) + j;
			pMesh->geometry.index[index++] = 1 + (i + 1) * (_division * 2 + 1) + j;
			pMesh->geometry.index[index++] = 1 + (i + 1) * (_division * 2 + 1) + 1 + j;

			pMesh->geometry.index[index++] = 1 + i * (_division * 2 + 1) + j;
			pMesh->geometry.index[index++] = 1 + (i + 1) * (_division * 2 + 1) + 1 + j;
			pMesh->geometry.index[index++] = 1 + i * (_division * 2 + 1) + 1 + j;
		}
	}

	//����
	for (int i = 0; i < _division * 2; ++i)
	{
		pMesh->geometry.index[index++] = 1 + (_division - 2) * (_division * 2 + 1) + i;
		pMesh->geometry.index[index++] = (DWORD)(pMesh->geometry.vertex.size() - 1);
		pMesh->geometry.index[index++] = 1 + (_division - 2) * (_division * 2 + 1) + 1 + i;
	}

	//�o�O�`�F�b�N
	if (pMesh->geometry.index.size() != index){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Resource::LoadSphereMesh]Failed build index\n"));
		return nullptr;
	}

	ao::CreateIndexBuffer(
		this->pd3dDevice.GetComPtr(),
		pMesh->geometry.ToIndexBufferCreator(),
		(UINT)pMesh->geometry.GetIndexBufferSize(),
		pMesh->geometry.GetIndexBuffer());

	ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		pMesh->geometry.ToVertexBufferCreator(),
		(UINT)pMesh->geometry.GetVertexBufferSize(),
		pMesh->geometry.GetVertexBuffer());

	pMesh->subset.resize(1);
	pMesh->subset[0].index = pMesh->geometry.index;
	pMesh->subset[0].material.diffuse[0] = 0.8f;
	pMesh->subset[0].material.diffuse[1] = 0.8f;
	pMesh->subset[0].material.diffuse[2] = 0.8f;
	pMesh->subset[0].material.diffuse[3] = 1;

	pMesh->subset[0].material.specular[0] = 0.0f;
	pMesh->subset[0].material.specular[1] = 0.0f;
	pMesh->subset[0].material.specular[2] = 0.0f;
	pMesh->subset[0].material.specular[3] = 1;

	pMesh->subset[0].material.emissive[0] = 0;
	pMesh->subset[0].material.emissive[1] = 0;
	pMesh->subset[0].material.emissive[2] = 0;

	for (auto& it : pMesh->subset[0].pTextures)
		it = nullptr;

	std::lock_guard<std::mutex> lock(this->meshMutex);
	this->pMeshMap.insert(std::map<string, BaseMesh*>::value_type(findFilePath.data(), pMesh));

	return pMesh;
}