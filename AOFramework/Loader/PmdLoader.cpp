#include"stdafx.hpp"
#include"PmdLoader.hpp"
#include"../Utility/Macro.inl"
#include"../Debug/DebugSystem.hpp"


using namespace ao;


PmdLoader::PmdLoader()
{

}


PmdLoader::~PmdLoader()
{

}


//------------------------------------------------------------------------------
/**	@brief		�ǂݍ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/03	16:57	T.Aoyama	�쐬
//------------------------------------------------------------------------------
bool PmdLoader::Load(const pmdString _filePath)
{
	std::ifstream ifs(_filePath.c_str(), std::ios::binary);
	if (ifs.fail()){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[PmdLoader::Load]Failed open pmd file \"%s\"\n"), _filePath.c_str());
		return false;
	}

	// PMD�w�b�_�̃��[�h
	Pmd::Header pmdHeader;
	ifs.read((char*)&pmdHeader, sizeof(pmdHeader));

	// PMD���_�̃��[�h
	unsigned long numPMDVertex;
	ifs.read((char*)&numPMDVertex, sizeof(numPMDVertex));
	Pmd::Vertex* pPMDVertices = new Pmd::Vertex[numPMDVertex];
	ifs.read((char*)pPMDVertices, sizeof(Pmd::Vertex)*numPMDVertex);

	// PMD�ʂ̃��[�h
	unsigned long numPMDIndex;
	ifs.read((char*)&numPMDIndex, sizeof(numPMDIndex));
	unsigned short* pPMDFaceIndexes = new unsigned short[numPMDIndex];
	ifs.read((char*)pPMDFaceIndexes, sizeof(unsigned short)*numPMDIndex);

	// PMD�}�e���A���̃��[�h
	unsigned long numPMDMaterial;
	ifs.read((char*)&numPMDMaterial, sizeof(numPMDMaterial));
	Pmd::Material* pPMDMaterial = new Pmd::Material[numPMDMaterial];
	ifs.read((char*)pPMDMaterial, sizeof(Pmd::Material)*numPMDMaterial);

	// �{�[�������[�h
	WORD numPMDBones;
	ifs.read((char*)&(numPMDBones), sizeof(numPMDBones));
	Pmd::Bone* pPMDBone = new Pmd::Bone[numPMDBones];
	ifs.read((char*)pPMDBone, sizeof(Pmd::Bone)*numPMDBones);

	// IK�{�[�������[�h
	WORD numPMDIKBone;
	ifs.read((char*)&numPMDIKBone, sizeof(numPMDIKBone));

	//IK�{�[���̍쐬
	auto CreateIKBone = [&](IKBone* _pOut,const Pmd::IKBone& _ikBone,const std::vector<WORD>& _child)
	{
		_pOut->boneIndex = _ikBone.boneIndex;
		_pOut->effectorBoneIndex = _ikBone.effectorBoneIndex;
		_pOut->chainLength = _ikBone.chainLength;
		_pOut->numIterations = _ikBone.numIterations;
		_pOut->controlWeight = _ikBone.controlWeight;
		//�C���f�b�N�X�����݂��Ă�����
		if (!_child.empty())
			_pOut->childBoneIndexVector = _child;
	};

	// IK�{�[���̊i�[�p�𐶐�
	this->ikBone.resize(numPMDIKBone);
	// �t�@�C���ǂݍ��݂Ɠ�����IK�{�[�����쐬
	for (WORD i = 0; i<numPMDIKBone; ++i)
	{
		Pmd::IKBone	pmdIKBone;
		ifs.read((char*)&pmdIKBone, sizeof(Pmd::IKBone));
		std::vector<WORD> ikChildBoneIndexVector(pmdIKBone.chainLength);
		//�q�������݂��鎞�����ǂݍ���
		if (pmdIKBone.chainLength != 0)
			ifs.read((char*)&ikChildBoneIndexVector[0], sizeof(WORD)*pmdIKBone.chainLength);
		//IK�{�[���֌W�f�[�^���쐬
		CreateIKBone(&this->ikBone[i], pmdIKBone, ikChildBoneIndexVector);
	}


	//���_�̃o�b�t�@�쐬
	this->vertexBuffer.resize(numPMDVertex);
	for (DWORD i = 0; i<numPMDVertex; ++i)
	{
		this->vertexBuffer[i].x = pPMDVertices[i].pos[0];
		this->vertexBuffer[i].y = pPMDVertices[i].pos[1];
		this->vertexBuffer[i].z = pPMDVertices[i].pos[2];
		this->vertexBuffer[i].x *= PMD_MESH_SCALE;
		this->vertexBuffer[i].y *= PMD_MESH_SCALE;
		this->vertexBuffer[i].z *= PMD_MESH_SCALE;
		this->vertexBuffer[i].weight[0] = (float)pPMDVertices[i].boneWeight / 100.0f;
		this->vertexBuffer[i].weight[1] = 1.0f - this->vertexBuffer[i].weight[0];
		this->vertexBuffer[i].weight[2] = 0.0f;
		this->vertexBuffer[i].index[0] = (INT)pPMDVertices[i].boneIndex[0];
		this->vertexBuffer[i].index[1] = (INT)pPMDVertices[i].boneIndex[1];
		this->vertexBuffer[i].index[2] = 0;
		this->vertexBuffer[i].index[3] = 0;
		this->vertexBuffer[i].nx = pPMDVertices[i].vNormal[0];
		this->vertexBuffer[i].ny = pPMDVertices[i].vNormal[1];
		this->vertexBuffer[i].nz = pPMDVertices[i].vNormal[2];
		this->vertexBuffer[i].u = pPMDVertices[i].uv[0];
		this->vertexBuffer[i].v = pPMDVertices[i].uv[1];
	}

	//�C���f�b�N�X�f�[�^�R�s�[
	this->indexBuffer.resize(numPMDIndex);
	for (DWORD i = 0; i<numPMDIndex; ++i)
		this->indexBuffer[i] = pPMDFaceIndexes[i];

	//�{�[���f�[�^�̃R�s�[
	this->bone.resize(numPMDBones);
	for (WORD i = 0; i<numPMDBones; ++i)
		this->CreateBone(&this->bone[i], pPMDBone, numPMDBones, i);

	//�ގ��̈ꕔ���R�s�[
	this->materialBuffer.resize(numPMDMaterial);
	for (DWORD i = 0;i < numPMDMaterial; ++i)
	{
		// �}�e���A���̃C���f�b�N�X��
		this->materialBuffer[i].indexCount = pPMDMaterial[i].indexNum;

		//�O�p�`�J�E���g
		this->materialBuffer[i].faceCount = pPMDMaterial[i].toonIndex * 3;

		// �ގ��R�s�[
		this->materialBuffer[i].diffuse[3] = pPMDMaterial[i].alpha;
		this->materialBuffer[i].specular[3] = pPMDMaterial[i].specularity;
		::memcpy(this->materialBuffer[i].diffuse.data(), pPMDMaterial[i].diffuseColor, sizeof(float) * 3);
		::memcpy(this->materialBuffer[i].specular.data(), pPMDMaterial[i].specularColor, sizeof(float) * 3);
		::memcpy(this->materialBuffer[i].emissive.data(), pPMDMaterial[i].mirrorColor, sizeof(float) * 3);

		// �e�N�X�`��
		std::string texFileName;
		texFileName.assign(pPMDMaterial[i].textureFileName);

		//�e�N�X�`����������΃X�L�b�v
		if (texFileName.empty())
			continue;

		texFileName = texFileName.substr(0, texFileName.find("*"));

		//�e�N�X�`���t�@�C���p�X�L��
		this->materialBuffer[i].texture.assign(texFileName);
	}

	delete[] pPMDVertices;
	delete[] pPMDFaceIndexes;
	delete[] pPMDMaterial;
	delete[] pPMDBone;

	return true;
}

//------------------------------------------------------------------------------
/**	@brief		�{�[���f�[�^�̍쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/03	17:39	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void PmdLoader::CreateBone(
	BoneRelation* _pOut,
	const Pmd::Bone* _bone,
	WORD _maxNum,
	WORD _index)
{
	//�{�[�����R�s�[
	::CopyMemory(_pOut->name, _bone[_index].boneName, sizeof(_bone[_index].boneName));

	//�e��ID�R�s�[
	_pOut->parentID = _bone[_index].parentBoneIndex;

	//��1�q��ID�R�s�[
	_pOut->firstChildID = 0xFFFF;
	for (WORD i = 0; i<_maxNum; ++i)
	{
		if (_index == _bone[i].parentBoneIndex){
			_pOut->firstChildID = i;
			break;
		}
	}

	auto BulidBrotherBoneId = [&]()
	{
		for (WORD i = _index + 1; i<_maxNum; ++i){
			if (_bone[_index].parentBoneIndex == _bone[i].parentBoneIndex){
				_pOut->siblingID = i;
				return;
			}
		}
	};

	//�Z���ID�R�s�[
	_pOut->siblingID = 0xFFFF;
	if (_bone[_index].parentBoneIndex != 0xFFFF)
		BulidBrotherBoneId();

	//�����s����쐬
	::SecureZeroMemory(_pOut->initMtx, sizeof(_pOut->initMtx));
	_pOut->initMtx[0][0] = 1;
	_pOut->initMtx[1][1] = 1;
	_pOut->initMtx[2][2] = 1;
	_pOut->initMtx[3][3] = 1;
	_pOut->initMtx[3][0] = _bone[_index].boneHeadPos[0] * PMD_MESH_SCALE;
	_pOut->initMtx[3][1] = _bone[_index].boneHeadPos[1] * PMD_MESH_SCALE;
	_pOut->initMtx[3][2] = _bone[_index].boneHeadPos[2] * PMD_MESH_SCALE;
}