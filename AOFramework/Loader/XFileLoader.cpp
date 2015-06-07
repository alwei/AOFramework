#include"stdafx.hpp"
#include"XFileLoader.hpp"


using namespace ao;


XFileLoader::XFileLoader()
{

}


XFileLoader::~XFileLoader()
{

}



//------------------------------------------------------------------------------
/**	@brief		X�t�@�C���̃f�[�^���o���� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/10/19	00:11	T.Aoyama	�쐬
//------------------------------------------------------------------------------
bool	XFileLoader::Load(const xString _filePath)
{
	std::fstream fs;

	fs.open(_filePath.c_str(), std::ios::in);
	if (fs.fail())
		return false;

	char temp;
	std::string	buffer;
	while (fs.get(temp))
		buffer += temp;

	fs.close();

	this->LoadVertex(buffer);
	this->LoadTexcoord(buffer);
	this->LoadIndex(buffer);
	this->LoadMaterial(buffer);
	this->StoreSortIndex();
	this->CreateNormal();

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		string���璸�_�f�[�^���o */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/18	08:01	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	bool	XFileLoader::LoadVertex(const std::string& _buffer)
{
	std::string::size_type index;

	//"Mesh {"�̕�����T��	
	index = _buffer.rfind("Mesh {");

	//�����񂪌�����Ȃ��ꍇ���o���s
	if (index == std::string::npos)
		return false;

	//���l��������܂ŒT��
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
		++index;

	//���_���̕����񒊏o
	char temp, numStr[16] = { '\0' };
	for (int i = 0; (temp = _buffer.at(index)) != ';'; ++i, ++index)
		numStr[i] = temp;

	//���_�̐��������z���p��
	int nVertex = atoi(numStr);
	this->serialVertex.resize(nVertex);

	//���l��������܂ŒT��
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9') && _buffer.at(index) != '-')
		++index;

	//���_���W�𒊏o
	int nCount = 0;
	int	nLoopCount = 0;
	std::string	nStrData;
	float store[3];
	int endCode = 0;

	//���̕������
	auto Next = [&](){
		endCode = 0;
	};

	//�i�[����
	auto Store = [&](char _n){
		endCode += 0x01;
		if (endCode & 0x02)
			return;
		nStrData += '\0';
		sscanf_s(nStrData.c_str(), "%f", &store[nLoopCount]);
		nStrData.clear();
		nLoopCount = (nLoopCount + 1) % 3;
		if (nLoopCount == 0)
		{
			this->serialVertex[nCount].x = store[0];
			this->serialVertex[nCount].y = store[1];
			this->serialVertex[nCount].z = store[2];
			++nCount;
		}
	};

	//���̕�����
	auto NextChar = [&](char _n){
		endCode = 0;
		nStrData += _n;
	};

	for (; !(endCode & 0x02);)
	{
		char nChar = _buffer.at(index);

		switch (nChar)
		{
		case ' ':
		case ',':
		case '\n':
			Next();
			break;
		case ';':
			Store(nChar);
			break;
		default:
			NextChar(nChar);
			break;
		}
		++index;
	}

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		�e�N�X�`�����W�ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/19	20:12	T.Aoyama	�쐬
//------------------------------------------------------------------------------
bool XFileLoader::LoadTexcoord(const std::string& _buffer)
{
	std::string::size_type index;

	//"Mesh {"�̕�����T��	
	index = _buffer.rfind("MeshTextureCoords {");

	//�����񂪌�����Ȃ��ꍇ���o���s
	if (index == std::string::npos)
		return false;

	//���l��������܂ŒT��
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
		++index;

	//���_���̕����񒊏o
	char temp, numStr[16] = { '\0' };
	for (int i = 0; (temp = _buffer.at(index)) != ';'; ++i, ++index)
		numStr[i] = temp;

	//���_�̐��������z���p��
	int nTex = atoi(numStr);
	this->serialVertex.resize(nTex);

	//���l��������܂ŒT��
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9') && _buffer.at(index) != '-')
		++index;

	//���_���W�𒊏o
	int nCount = 0;
	int	nLoopCount = 0;
	std::string	nStrData;
	float store[2];
	int endCode = 0;

	//���̕������
	auto Next = [&](){
		endCode = 0;
	};

	//�i�[����
	auto Store = [&](char _n){
		endCode += 0x01;
		if (endCode & 0x02)
			return;
		nStrData += '\0';
		sscanf_s(nStrData.c_str(), "%f", &store[nLoopCount]);
		nStrData.clear();
		nLoopCount = (nLoopCount + 1) % 2;
		if (nLoopCount == 0)
		{
			this->serialVertex[nCount].u = store[0];
			this->serialVertex[nCount].v = store[1];
			++nCount;
		}
	};

	//���̕�����
	auto NextChar = [&](char _n){
		endCode = 0;
		nStrData += _n;
	};

	for (; !(endCode & 0x02);)
	{
		char nChar = _buffer.at(index);

		switch (nChar)
		{
		case ' ':
		case ',':
		case '\n':
			Next();
			break;
		case ';':
			Store(nChar);
			break;
		default:
			NextChar(nChar);
			break;
		}
		++index;
	}

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		string����C���f�b�N�X�f�[�^���o */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/18	08:01	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	bool	XFileLoader::LoadIndex(const std::string& _buffer)
{
	std::string::size_type index;

	//Mesh�̕�����T���iIndex�Ƃ��������񂪂Ȃ����߁j	
	index = _buffer.rfind("Mesh {");

	//�����񂪌�����Ȃ��ꍇ���o���s
	if (index == std::string::npos)
		return false;

	//�������̕�����܂ŃV�t�g
	int isMeshEnd = 0;
	while (!(isMeshEnd & 0x02))
	{
		++index;
		if (_buffer.at(index) == ';')
			isMeshEnd += 0x01;
		else
			isMeshEnd &= 0x00;
	}

	//���_�̏I���_�u���Z�~�R�������΂�
	index += 2;

	//�C���f�b�N�X�����܂ŃV�t�g
	while (_buffer.at(index) != ';')
		++index;

	//�ŏ��̃C���f�b�N�X�̔ԍ���������܂ŃV�t�g
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
		++index;

	//�������W�𒊏o
	int store[4];
	std::string	nStrData;

	int endCode = 0;
	SortIndex	pushSortIndex;

	//���̕������
	auto Next = [&](){
		endCode = 0;
	};

	//�O�p�`�̏ꍇ
	auto Triangle = [&](){
		++index;
		sscanf_s(&_buffer[index], "%d,%d,%d", &store[0], &store[1], &store[2]);
		for (int i = 0; i < 3; ++i)
			pushSortIndex.indexArray[i] = store[i];
		pushSortIndex.isQuad = false;
		this->sorter.push_back(pushSortIndex);
	};

	//�l�p�`�̏ꍇ
	auto Quad = [&](){
		++index;
		sscanf_s(&_buffer[index], "%d,%d,%d,%d", &store[0], &store[1], &store[2], &store[3]);
		for (int i = 0; i < 4; ++i)
			pushSortIndex.indexArray[i] = store[i];
		pushSortIndex.isQuad = true;
		this->sorter.push_back(pushSortIndex);
	};

	//�i�[����
	auto Store = [&](){
		endCode += 0x01;
		if (endCode & 0x02)
			return;

		sscanf_s(nStrData.c_str(), "%d", &store[0]);
		switch (store[0])
		{
		case 3:
			Triangle();
			break;
		case 4:
			Quad();
			break;
		}
		nStrData.clear();
		while (_buffer.at(index) != ';')
			++index;
	};

	//���̕�����
	auto NextChar = [&](char _n){
		endCode = 0;
		nStrData += _n;
	};

	for (; !(endCode & 0x02);)
	{
		char nChar;
		nChar = _buffer.at(index);

		switch (nChar)
		{
		case ' ':
		case ',':
		case '\n':
			Next();
			break;
		case ';':
			Store();
			break;
		default:
			NextChar(nChar);
			break;
		}
		++index;
	}

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		string����}�e���A���f�[�^���o */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/18	08:01	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	bool XFileLoader::LoadMaterial(const std::string& _buffer)
{
	std::string::size_type index;

	//"MeshMaterialList {"�̕�����T��	
	index = _buffer.rfind("MeshMaterialList {");

	//�����񂪌�����Ȃ��ꍇ���o���s
	if (index == std::string::npos)
		return false;

	//���l��������܂ŒT��
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
		++index;

	//�}�e���A�����̕����񒊏o
	char temp, numStr[16];
	for (int i = 0; (temp = _buffer.at(index)) != ';'; ++i, ++index)
		numStr[i] = temp;

	//�}�e���A���̐��������z���p��
	int nMaterial = atoi(numStr);
	this->material.resize(nMaterial);

	//�������킹
	++index;

	//�\������ʐ����X�L�b�v
	while (_buffer.at(index) != ';')
		++index;

	//�ŏ��̓K�p����ʂ̃}�e���A���C���f�b�N�X�܂ŃV�t�g
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
		++index;

	int mIndex = 0;

	//�\�[�g�p�̃C���f�b�N�X�f�[�^�Ƀ}�e���A���f�[�^�z�u
	for (int i = 0;; ++i)
	{
		sscanf_s(&_buffer[index], "%d", &mIndex);

		//�}�e���A���̃C���f�b�N�X�ݒ�
		this->sorter[i].index = mIndex;

		//�}�e���A���i�T�u�Z�b�g�j�ɃC���f�b�N�X�������Z
		this->material[mIndex].indexCount += (this->sorter[i].isQuad) ? 6 : 3;

		//���փV�t�g
		++index;

		//���̐��l�܂��͏I�[�܂ŃV�t�g
		while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9') && _buffer.at(index) != ';')
			++index;

		//�Ō�Ȃ�break
		if (_buffer.at(index) == ';')
			break;
	}

	//"Material"��'M'������������܂ŒT��
	while (!(_buffer[index] == 'M'))
		++index;

	//���l��������܂ŒT��
	while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9') && _buffer.at(index) != '-')
		++index;

	//�}�e���A�������𒊏o
	int	nLoopCount = 0;

	for (int i = 0; i < nMaterial; ++i)
	{
		//�����܂ŃX�L�b�v����
		while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
			++index;

		//�|�C���^�i�[
		float diffuse[4];
		float specular[4];
		float emissive[3];

		//Diffuse���o
		sscanf_s(&_buffer[index], "%f;%f;%f;%f", &diffuse[0], &diffuse[1], &diffuse[2], &diffuse[3]);

		//���݂̍s���΂�
		while (_buffer.at(index) != '\n')
			++index;
		//�����܂ŃX�L�b�v����
		while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
			++index;
		//�X�y�L�������x���o
		sscanf_s(&_buffer[index], "%f", &specular[3]);

		//���݂̍s���΂�
		while (_buffer.at(index) != '\n')
			++index;
		//�����܂ŃX�L�b�v����
		while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
			++index;
		//�X�y�L�����F���o
		sscanf_s(&_buffer[index], "%f;%f;%f", &specular[0], &specular[1], &specular[2]);

		//���݂̍s���΂�
		while (_buffer.at(index) != '\n')
			++index;
		//�����܂ŃX�L�b�v����
		while (!(_buffer.at(index) >= '0' && _buffer.at(index) <= '9'))
			++index;
		//���Ȕ����̐����𒊏o
		sscanf_s(&_buffer[index], "%f;%f;%f", &emissive[0], &emissive[1], &emissive[2], &emissive[3]);

		//���o�����f�[�^���R�s�[
		::CopyMemory(this->material[i].diffuse.data(), diffuse, sizeof(float) * 4);
		::CopyMemory(this->material[i].specular.data(), specular, sizeof(float) * 4);
		::CopyMemory(this->material[i].emissive.data(), emissive, sizeof(float) * 3);

		//�e�N�X�`�������������
		while (!(_buffer.at(index) == '{' || _buffer.at(index) == '}'))
			++index;

		if (_buffer.at(index) == '}')
			continue;

		//�t�@�C���p�X�܂Ŕ�΂�
		while (_buffer.at(index) != '\"')
			++index;

		++index;
		//�e�N�X�`���p�X��1���������o
		std::string texFilePath;
		while (_buffer.at(index) != '\"'){
			texFilePath += _buffer.at(index);
			++index;
		}

		// �e�N�X�`���p�X�����S���o
		this->material[i].texture.assign(texFilePath);
	}

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		���_�f�[�^����@���Z�o */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/18	08:01	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	bool	XFileLoader::CreateNormal()
{
	auto Sub = [=](float3* _pOut, const Vertex& v1, const Vertex& v2){
		_pOut->x = v2.x - v1.x;
		_pOut->y = v2.y - v1.y;
		_pOut->z = v2.z - v1.z;
	};
	auto Cross = [=](float3* _pOut, const float3& _v1, const float3& _v2){
		_pOut->x = (_v1.y*_v2.z - _v1.z*_v2.y);
		_pOut->y = (_v1.z*_v2.x - _v1.x*_v2.z);
		_pOut->z = (_v1.x*_v2.y - _v1.y*_v2.x);
	};
	auto Normalize = [=](float3* _pOut, float3& _v){
		float d = std::sqrtf((_v.x*_v.x) + (_v.y*_v.y) + (_v.z*_v.z));
		_pOut->x = _v.x / d;
		_pOut->y = _v.y / d;
		_pOut->z = _v.z / d;
	};

	for (auto& it : this->serialVertex)
	{
		it.nx = 0;
		it.ny = 0;
		it.nz = 0;
	}

	for (UINT i = 0; i < this->serialIndex.size(); i += 3)
	{
		float3 normal, v1, v2;
		Sub(&v1, this->serialVertex[this->serialIndex[i]], this->serialVertex[this->serialIndex[i + 1]]);
		Sub(&v2, this->serialVertex[this->serialIndex[i]], this->serialVertex[this->serialIndex[i + 2]]);
		Cross(&normal, v1, v2);
		Normalize(&normal, normal);
		for (int j = 0; j < 3; j++){
			this->serialVertex[this->serialIndex[i + j]].nx += normal.x;
			this->serialVertex[this->serialIndex[i + j]].ny += normal.y;
			this->serialVertex[this->serialIndex[i + j]].nz += normal.z;
		}
	}

	for (auto& it : this->serialVertex)
	{
		float3 vN = {it.nx,it.ny,it.nz};
		Normalize(&vN, vN);
		it.nx = vN.x;
		it.ny = vN.y;
		it.nz = vN.z;
	}

	return true;
}


//------------------------------------------------------------------------------
/**	@brief		�C���f�b�N�X���}�e���A���C���f�b�N�X�Ɋ�Â��ă\�[�g�i�[ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/10/20	14:56	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	bool XFileLoader::StoreSortIndex()
{
	//�C���f�b�N�X�z��\�[�g�i�����j
	std::sort(this->sorter.begin(), this->sorter.end());

	int iVertex = 0;
	int mLogIndex = 0;
	int iSerial = 0;
	std::vector<INT> vIndex;
	std::vector<Vertex> vList;
	for (auto& it : this->sorter)
	{
		if (mLogIndex != it.index)
		{
			this->sectionIndex.push_back(vIndex);
			vIndex.clear();
		}

		//�C���f�b�N�X�i�[
		Vertex v[3];
		::CopyMemory( &v[0], &this->serialVertex[it.indexArray[0]], sizeof(Vertex) );
		::CopyMemory( &v[1], &this->serialVertex[it.indexArray[1]], sizeof(Vertex) );
		::CopyMemory( &v[2], &this->serialVertex[it.indexArray[2]], sizeof(Vertex) );
		vList.push_back(v[0]);
		vList.push_back(v[1]);
		vList.push_back(v[2]);
		this->serialIndex.push_back(it.indexArray[0]);
		this->serialIndex.push_back(it.indexArray[1]);
		this->serialIndex.push_back(it.indexArray[2]);
		vIndex.push_back(iSerial+0);
		vIndex.push_back(iSerial+1);
		vIndex.push_back(iSerial+2);

		//�l�p�`�Ȃ�O�p�`�C���f�b�N�X�𑝂₷
		if (it.isQuad)
		{
			this->serialIndex.push_back(it.indexArray[0]);
			this->serialIndex.push_back(it.indexArray[2]);
			this->serialIndex.push_back(it.indexArray[3]);
			::CopyMemory(&v[0], &this->serialVertex[it.indexArray[0]], sizeof(Vertex));
			::CopyMemory(&v[1], &this->serialVertex[it.indexArray[2]], sizeof(Vertex));
			::CopyMemory(&v[2], &this->serialVertex[it.indexArray[3]], sizeof(Vertex));
			vList.push_back(v[0]);
			vList.push_back(v[1]);
			vList.push_back(v[2]);
			vIndex.push_back(iSerial + 3);
			vIndex.push_back(iSerial + 4);
			vIndex.push_back(iSerial + 5);
			iSerial += 3;
		}

		mLogIndex = it.index;
		iSerial += 3;
	}

	this->sectionIndex.push_back(vIndex);

	//�V���A���C���f�b�N�X�𐮗�
	iSerial = 0;
	for (auto& it : this->sectionIndex)
	{
		for (auto& idx : it){
			this->serialIndex[iSerial] = idx;
			++iSerial;
		}
	}

	//���_���O�p�`�����������̂ɓ���ւ�
	this->serialVertex.swap(vList);

	return true;
}