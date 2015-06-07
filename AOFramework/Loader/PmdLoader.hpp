/*************************************************************
*	@file   	PmdLoader.hpp
*	@brief  	PmdLoader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/03
*************************************************************/

#ifndef _Include_PMDLoader_hpp_	// �C���N���[�h�K�[�h
#define _Include_PMDLoader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include<array>

#ifdef UNICODE
typedef std::wstring pmdString;
typedef wchar_t pmdChar;
#else
typedef std::string pmdString;
typedef char pmdChar;
#endif


namespace ao
{
	//------------------------------------------------------------
	// �萔��`
	//------------------------------------------------------------
	const float PMD_MESH_SCALE = 1.0f;

	class Pmd
	{
	public:
#pragma pack(push,1)	//�A���C�������g������I�t

		/*!-----------------------------------------------------------
		//	@struct	PMDHeader
		//	@brief	�w�b�_�[���
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Header
		{
			unsigned char	magic[3];
			float			version;
			unsigned char	modelName[20];
			unsigned char	comment[256];
		};

		/*!-----------------------------------------------------------
		//	@struct	PMDVertex
		//	@brief	���_�f�[�^
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Vertex
		{
			float			pos[3];			///< ���_���W
			float			vNormal[3];		///< �@���x�N�g��
			float			uv[2];			///< �e�N�X�`�����W
			unsigned short	boneIndex[2];	///< �e������{�[���C���f�b�N�X
			unsigned char	boneWeight;		///< �{�[���̃E�F�C�g�l
			unsigned char	edgeFlag;		///< �G�b�W�t���O
		};

		/*!-----------------------------------------------------------
		//	@struct	PMDMaterial
		//	@brief	�ގ��f�[�^
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Material
		{
			float diffuseColor[3];		///< �f�B�t���[�Y�F
			float alpha;					///< �A���t�@�l
			float specularity;			///< �X�y�L�������x
			float specularColor[3];		///< �X�y�L�����J���[
			float mirrorColor[3];			///< �~���[�J���[
			unsigned char toonIndex;		///< �g�D�[���C���f�b�N�X
			unsigned char edgeFlag;		///< �G�b�W�t���O
			unsigned long indexNum;		///< �}�e���A���i�T�u�Z�b�g�j���Ƃ̃C���f�b�N�X��
			char textureFileName[20];		///< �e�N�X�`���t�@�C����
		};

		/*!-----------------------------------------------------------
		//	@struct	PMDBone
		//	@brief	�{�[���f�[�^
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Bone
		{
			char			boneName[20];			///< �{�[����
			unsigned short	parentBoneIndex;		///< �Ȃ��ꍇ��0xFFFF
			unsigned short	tailPosBoneIndex;		///< �Ȃ��ꍇ��0xFFFF
			unsigned char	boneType;				///< 0:��] 1:��]�ƈړ� 2:IK 3:�s�� 4:IK�e���� 5:��]�e���� 6:IK�ڑ��� 7:��\�� 8:�P�� 9:��]�^�� (8, 9��MMD4.0�ȍ~)
			unsigned short	ikParentBoneIndex;	///< �Ȃ��ꍇ��0
			float			boneHeadPos[3];		///< �{�[���擪���W
		};

		/*!-----------------------------------------------------------
		//	@struct	PMDIKBone
		//	@brief	IK�{�[��
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct IKBone
		{
			WORD	boneIndex;			///< IK�{�[���ԍ��i�C���f�b�N�X�j
			WORD	effectorBoneIndex;	///< IK�{�[���̈ʒu�ɁA���̃{�[������v������悤��IK�������s����
			BYTE	chainLength;		///< IK�`�F�[���̒���
			WORD	numIterations;		///< �ċA���Z��
			FLOAT	controlWeight;		///< �{�[���̒P�ʐ����p 1.0->4.0[rad]. �܂��u�Ђ��v�Ƃ������O�̃{�[����X�������ɂ��������Ȃ�����������
		};
#pragma pack(pop)
	};


	class Vmd
	{
	public:
#pragma pack(push,1)	//�A���C�������g������I�t
		/*!-----------------------------------------------------------
		//	@struct	VMDHeader
		//	@brief	VMD�\���̃w�b�_
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Header
		{
			char vmdHeader[30];
			char vmdModelName[20];
		};

		/*!-----------------------------------------------------------
		//	@struct	VMDMotion
		//	@brief	VMD�\����
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Motion
		{
			char			boneName[15];		///< �������{�[���̖��O
			unsigned long	frameNo;			///< �t���[���ԍ�
			float			location[3];		///< �ړ���
			float			localRotation[4];		///< ���f�����[�J�����W�n
			unsigned char	interpolation[64];	///<[4][4][4]���[�V������Ԍ`��
		};
#pragma pack(pop)
	};


	/*!-----------------------------------------------------------
	//	@class  PmdLoader
	//	@brief  PMD�ǂݍ��݃N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/03
	------------------------------------------------------------*/
	class PmdLoader
	{
		struct IKBone
		{
			WORD	boneIndex;			///< IK�{�[���ԍ��i�C���f�b�N�X�j
			WORD	effectorBoneIndex;	///< IK�{�[���̈ʒu�ɁA���̃{�[������v������悤��IK�������s����
			BYTE	chainLength;		///< IK�`�F�[���̒���
			WORD	numIterations;		///< �ċA���Z��
			FLOAT	controlWeight;		///< �{�[���̒P�ʐ����p 1.0->4.0[rad]. �܂��u�Ђ��v�Ƃ������O�̃{�[����X�������ɂ��������Ȃ�����������
			std::vector<USHORT>  childBoneIndexVector;	///< �q��IK�̃C���f�b�N�Xvector
		};

		/*!-----------------------------------------------------------
		//	@struct	VERTEX
		//	@brief	�X�L�����b�V���p�̒��_�f�[�^
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Vertex
		{
			union{
				struct{
					float	x, y, z;
					float	nx, ny, nz;
					float	u, v;
				};
				struct{
					float baseData[8];
				};
			};
			union{
				struct{
					float	weight[3];
					int		index[4];
				};
				struct{
					float skinData[7];
				};
			};
		};

		struct Material
		{
			std::array<float, 4>	diffuse;	///< Diffuse����
			std::array<float, 4>	specular;	///< ���ʔ��˂̐���(a:�X�y�L�������x�j
			std::array<float, 3>	emissive;	///< ���Ȕ����̐���
			std::string texture;			///< �e�N�X�`���z��
			int		faceCount;				///< �ʐ�
			int		indexCount;				///< �}�e���A���Ɉˑ����Ă���C���f�b�N�X��
		};

		/*!-----------------------------------------------------------
		//	@struct	BoneRelation
		//	@brief	�f�[�^���^�Ԃ��߂̃{�[���f�[�^
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct BoneRelation
		{
			char		name[30];		///< �{�[����
			WORD		firstChildID;	///< ���q�{�[���C���f�b�N�X(�Ȃ����0xffff)
			WORD		siblingID;		///< �Z��{�[���C���f�b�N�X(�Ȃ����0xffff)
			WORD		parentID;		///< �e�{�[���C���f�b�N�X(�Ȃ����0xffff)
			float		length;			///< �{�[���̒���
			float		initMtx[4][4];	///< �{�[�������s��
		};

	public:
		PmdLoader();
		~PmdLoader();

		/*!-----------------------------------------------------------
		//	@brief		�ǂݍ���
		//	@param[in]	_filePath	�t�@�C���p�X
		//	@return		true�F�ǂݍ��ݐ����@false�F�ǂݍ��ݎ��s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/03
		------------------------------------------------------------*/
		bool Load(const pmdString _filePath);

	public:
		std::vector<Vertex> vertexBuffer;
		std::vector<DWORD> indexBuffer;
		std::vector<Material> materialBuffer;
		std::vector<BoneRelation> bone;
		std::vector<IKBone> ikBone;

	private:

		/*!-----------------------------------------------------------
		//	@brief		�{�[���f�[�^�쐬
		//	@param[in]	_pOut	�o�͂���{�[���f�[�^
		//	@param[in]	_bone	���͂���{�[���f�[�^
		//	@param[in]	_maxNum	�{�[���ő吔
		//	@param[in]	_index	�{�[���C���f�b�N�X
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/03
		------------------------------------------------------------*/
		inline	void CreateBone(
			BoneRelation* _pOut,
			const Pmd::Bone* _bone,
			WORD _maxNum,
			WORD _index);
	};
}

#endif // _Include_PMDLoader_hpp_