#include"stdafx.hpp"

#if 0	//obj�̎d�l���o���o���Ȃ̂Ń��[�_�[�͓���

#include"ObjLoader.hpp"
#include"../Utility/Macro.inl"
#include"../Debug/DebugSystem.hpp"


using namespace ao;


ObjLoader::ObjLoader()
{

}


ObjLoader::~ObjLoader()
{
	for (auto& it : this->materialList)
		delete it;
}


//------------------------------------------------------------------------------
/**	@brief		Obj�t�@�C���ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/26	19:49	T.Aoyama	�쐬
//------------------------------------------------------------------------------
bool ObjLoader::Load(const objString _filePath,const objString _mtlPath)
{
	this->loadFaild = FALSE;
	this->dwFaceIndex = 0;
	this->dwFaceCount = 0;
	this->dwCurSubset = 0;
	this->positionList.clear();
	this->normalList.clear();
	this->texcoordList.clear();
	this->vertexList.clear();

	//�@�t�@�C���̖��[�܂Ń��[�v
	if( !_filePath.empty() )
		this->ReadGeometry(_filePath);

	//MTL�t�@�C���̓ǂݍ���
	if (!_mtlPath.empty())
		this->LoadMtl(_mtlPath);
	else
		ao::debug::PrintfColor(ConsoleColor::H_YELLOW, _T("[ObjLoader::Load]Nothing mtl file.\n"));

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		���_�f�[�^�̓ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/27	14:58	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	void ObjLoader::ReadPosition(std::ifstream& _file)
{
	float3 v;
	_file >> v.x >> v.y >> v.z;
	this->positionList.push_back(v);
}


//------------------------------------------------------------------------------
/**	@brief		�@���f�[�^�ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/27	16:20	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	void ObjLoader::ReadNormal(std::ifstream& _file)
{
	float3 n;
	_file >> n.x >> n.y >> n.z;
	this->normalList.push_back(n);
}


//------------------------------------------------------------------------------
/**	@brief		�e�N�X�`�����W�ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/27	16:14	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	void ObjLoader::ReadTexcoord(std::ifstream& _file)
{
	float2 uv;
	_file >> uv.x >> uv.y;
	this->texcoordList.push_back(uv);
}


//------------------------------------------------------------------------------
/**	@brief		�t�F�C�X�p�̓ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/27	15:45	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	void ObjLoader::ReadFace(std::ifstream& _file)
{
	DWORD dwPosition, dwTexCoord, dwNormal;
	std::array<DWORD, 4> positionArray;
	std::array<DWORD, 4> normalArray;
	std::array<DWORD, 4> texcoordArray;
	Vertex vertex;

	positionArray.fill(-1);
	normalArray.fill(-1);
	texcoordArray.fill(-1);

	++this->dwFaceIndex;
	++this->dwFaceCount;

	int count = 0;

	//�C���f�b�N�X�Ɋ�Â����_���W���Z�o
	auto BuildVertexPosition = [&](int i)
	{
		++count;
		SecureZeroMemory(&vertex, sizeof(Vertex));

		_file >> dwPosition;
		positionArray[i] = dwPosition - 1;
		::CopyMemory(vertex.pos, &this->positionList[positionArray[i]], sizeof(float) * 3);
	};

	//�C���f�b�N�X�Ɋ�Â��e�N�X�`�����W���Z�o
	auto BuildVertexTexcoord = [&](int i)
	{
		if ('/' != _file.peek())
			return;

		_file >> dwTexCoord;
		texcoordArray[i] = dwTexCoord - 1;
		::CopyMemory(vertex.uv, &this->texcoordList[texcoordArray[i]], sizeof(float) * 2);
	};

	//�C���f�b�N�X�Ɋ�Â��@�����Z�o
	auto BuildVertexNormal = [&](int i)
	{
		if ('/' != _file.peek())
			return;

		_file.ignore();
		_file >> dwNormal;
		normalArray[i] = dwNormal - 1;
		::CopyMemory(vertex.n, &this->normalList[normalArray[i]], sizeof(float) * 3);
	};


	//�O�p�`,�l�p�`�̂ݑΉ�
	for (int iFace = 0; iFace < 4; ++iFace)
	{
		BuildVertexPosition(iFace);

		if ('/' == _file.peek()){
			BuildVertexTexcoord(iFace);
			BuildVertexNormal(iFace);
		}

		//�O�p�`�Ȃ璸�_�i�[
		if (iFace < 3){
			this->vertexList.push_back(vertex);
			this->indexList.push_back(this->vertexList.size() - 1);
		}

		//���s�Ȃ�I��
		if ('\n' == _file.peek())
			break;
	}

	auto AddVertexAndIndex = [&](int i)
	{
		int j = (i + 1) % 4;
		::SecureZeroMemory(&vertex, sizeof(Vertex));

		if (positionArray[j] != 1)
			::CopyMemory(vertex.pos, &this->positionList[positionArray[j]],sizeof(float)*3);
		if (normalArray[j] != 1)
			::CopyMemory(vertex.n, &this->normalList[normalArray[j]],sizeof(float)*3);
		if (texcoordArray[j] != 1)
			::CopyMemory(vertex.uv, &this->texcoordList[texcoordArray[j]],sizeof(float)*2);

		this->vertexList.push_back(vertex);
		this->indexList.push_back(this->vertexList.size() - 1);
	};

	//�l�p�`�̏ꍇ�A�O�p�`�Ƃ��čč\�z
	if (count > 3){
		++this->dwFaceIndex;
		++this->dwFaceCount;
		for(int iFace=0; iFace<4; ++iFace)
			AddVertexAndIndex(iFace);
	}
}


//------------------------------------------------------------------------------
/**	@brief		�}�e���A���쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/27	21:24	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	void ObjLoader::CreateMaterial(std::ifstream& _file)
{
	char materialName[256] = { 0 };
	_file >> materialName;

	objString cmpString;
#ifdef UNICODE
	std::string tempString(materialName);
	macro::StringMultiToWide(&cmpString, tempString);
#else
	cmpString.assign(materialName);
#endif

	for (int i = 0; i < this->materialList.size(); ++i){
		Material* pMat = this->materialList[i];
		if (!(pMat->name == cmpString))
			continue;

		this->dwCurSubset = i;
		return;
	}

	//�����}�e���A����������Ȃ������ꍇ�A�V�����}�e���A���쐬
	Material* pMat = new Material;
	this->dwCurSubset = this->materialList.size();
	this->InitMaterial(pMat);
	pMat->name.assign(cmpString);
	this->materialList.push_back(pMat);
}


//------------------------------------------------------------------------------
/**	@brief		�o�b�t�@�`�F�b�N */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/26	20:08	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	bool ObjLoader::CheckBuffer(std::ifstream& _file,char* _buffer)
{
	if (0 == strcmp(_buffer, "#"))
	{
		//�R�����g����
	}
	else if (0 == strcmp(_buffer, "v"))
	{
		//���_����
		this->ReadPosition(_file);
	}
	else if (0 == strcmp(_buffer, "vn"))
	{
		//�@������
		this->ReadNormal(_file);
	}
	else if (0 == strcmp(_buffer, "vt"))
	{
		//�e�N�X�`������
		this->ReadTexcoord(_file);
	}
	else if (0 == strcmp(_buffer, "f"))
	{
		//�ʏ���
		this->ReadFace(_file);
	}
	else if (0 == strcmp(_buffer, "mtllib"))
	{
		//�}�e���A�����C�u��������
		char name[256];
		_file >> name;
#ifdef UNICODE
		std::string tempName(name);
		macro::StringMultiToWide(&this->materialName, tempName);
#else
		this->materialName.assign(name);
#endif
	}
	else if (0 == strcmp(_buffer, "usemtl"))
	{
		//�}�e���A���쐬
		this->CreateMaterial(_file);
	}
	else
	{
		// Unimplemented or unrecognized command
	}

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		�}�e���A�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/27	21:10	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	void ObjLoader::InitMaterial(Material* _pMaterial)
{
	_pMaterial->ambient[0] = 0.2f;
	_pMaterial->ambient[1] = 0.2f;
	_pMaterial->ambient[2] = 0.2f;
	_pMaterial->diffuse[0] = 0.8f;
	_pMaterial->diffuse[1] = 0.8f;
	_pMaterial->diffuse[2] = 0.8f;
	_pMaterial->diffuse[3] = 1.0f;
	_pMaterial->specular[0] = 1.0f;
	_pMaterial->specular[1] = 1.0f;
	_pMaterial->specular[2] = 1.0f;
	_pMaterial->specular[3] = 0;
	_pMaterial->name.assign(_T("default material"));
}


//------------------------------------------------------------------------------
/**	@brief		�W�I���g���ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/26	19:59	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	bool ObjLoader::ReadGeometry(const objString& _filePath)
{
	std::ifstream file;

	//�@�t�@�C�����J��
	file.open(_filePath.c_str(), std::ios::in);
	if (!file.is_open()){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[ObjLoader::ReadGeometry]Failed open obj file \"%s\"\n"), _filePath.c_str());
		return nullptr;
	}

	//�}�e���A��������
	Material* pMaterial = new Material();
	this->InitMaterial(pMaterial);
	this->materialList.push_back(pMaterial);

	for (;;)
	{
		char buffer[256] = { 0 };

		file >> buffer;

		if (!file)
			break;

		this->CheckBuffer(file, buffer);
	}

	//�ǂݍ��ݎ��s
	if (this->loadFaild)
		return false;

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		MTL�t�@�C���̓ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/27	17:30	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	bool ObjLoader::LoadMtl(const objString& _mtlPath)
{
	std::ifstream file;

	this->iMtlCount = -1;
	std::array<objChar, 512> directoryPath;
	macro::GetFilePathFromPath(directoryPath.data(),_mtlPath.c_str());
	this->directoryPath.assign(directoryPath.data());

	file.open(_mtlPath.c_str(), std::ios::in);
	if (file.is_open()){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[ObjLoader::ReadMtl]Failed open mtl file \"%s\"\n"),_mtlPath.c_str());
		return false;
	}

	while (this->ReadMtl(file));

	//�@�t�@�C�������
	file.close();

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		�}�e���A���̉�͓ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/27	17:41	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	bool ObjLoader::ReadMtl(std::ifstream& _file)
{
	char buffer[256] = { 0 };

	_file >> buffer;
	if (!_file)
		return false;

	if (0 == strcmp(buffer, "newmtl"))
	{
		++this->iMtlCount;
		Material* pMaterial = new Material;
		this->InitMaterial(pMaterial);
		this->materialList.push_back(pMaterial);

		char materialName[256] = { 0 };
		std::string materialPath;
		_file >> materialName;
#ifdef UNICODE
		macro::StringWideToMulti(&materialPath, this->directoryPath);
		materialPath.append(materialName);
		macro::StringMultiToWide(&this->materialList[this->iMtlCount]->name, materialPath);
#else
		this->materialList[this->iMtlCount]->name.assign(materialName);
#endif
	}
	else if (0 == strcmp(buffer, "Ka"))
	{
		auto& a = this->materialList[this->iMtlCount]->ambient;
		_file >> a[0] >> a[1] >> a[2];
	}
	else if (0 == strcmp(buffer, "Kd"))
	{
		auto& d = this->materialList[this->iMtlCount]->diffuse;
		_file >> d[0] >> d[1] >> d[2];
	}
	else if (0 == strcmp(buffer, "Ks"))
	{
		auto& s = this->materialList[this->iMtlCount]->specular;
		_file >> s[0] >> s[1] >> s[2];
	}
	else if (0 == strcmp(buffer, "d") || 0 == strcmp(buffer, "Tr"))
	{
		auto& alpha = this->materialList[this->iMtlCount]->diffuse[3];
		_file >> alpha;
	}
	else if (0 == strcmp(buffer, "Ns"))
	{
		auto& shininess = this->materialList[this->iMtlCount]->specular[3];
		_file >> shininess;
	}
	else if (0 == strcmp(buffer, "map_Ka"))
	{
		auto& ambientMap = this->materialList[this->iMtlCount]->textures[0];

		char mapKaName[256];
		std::string tempPath;
		_file >> mapKaName;
#ifdef UNICODE
		macro::StringWideToMulti(&tempPath, this->directoryPath);
		tempPath.append(mapKaName);
		macro::StringMultiToWide(&ambientMap, tempPath);
#else
		ambientMap = this->directoryPath + mapKaName;
#endif
	}
	else if (0 == strcmp(buffer, "map_Kd"))
	{
		auto& diffuseMap = this->materialList[this->iMtlCount]->textures[1];

		char mapKdName[256];
		std::string tempPath;
		_file >> mapKdName;
#ifdef UNICODE
		macro::StringWideToMulti(&tempPath, this->directoryPath);
		tempPath.append(mapKdName);
		macro::StringMultiToWide(&diffuseMap, tempPath);
#else
		diffuseMap = this->directoryPath + mapKdName;
#endif
	}
	else if (0 == strcmp(buffer, "map_Ks"))
	{
		auto& specularMap = this->materialList[this->iMtlCount]->textures[2];

		char mapKsName[256];
		std::string tempPath;
		_file >> mapKsName;
#ifdef UNICODE
		macro::StringWideToMulti(&tempPath, this->directoryPath);
		tempPath.append(mapKsName);
		macro::StringMultiToWide(&specularMap, tempPath);
#else
		specularMap = this->directoryPath + mapKsName;
#endif
	}
	else if (0 == strcmp(buffer, "map_Bump"))
	{
		auto& bumpMap = this->materialList[this->iMtlCount]->textures[3];

		char bumpMapName[256];
		std::string tempPath;
		_file >> bumpMapName;
#ifdef UNICODE
		macro::StringWideToMulti(&tempPath, this->directoryPath);
		tempPath.append(bumpMapName);
		macro::StringMultiToWide(&bumpMap, tempPath);
#else
		bumpMap = this->directoryPath + bumpMapName;
#endif
	}

	_file.ignore();

	return true;
}

#endif