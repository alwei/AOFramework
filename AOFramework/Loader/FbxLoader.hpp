#if 1
/*************************************************************
*	@file   	FbxLoader.hpp
*	@brief  	FbxLoader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/09/28
*************************************************************/

#ifndef _Include_FbxLoader_hpp_	// �C���N���[�h�K�[�h
#define _Include_FbxLoader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<fbxsdk.h>
#include<string>
#include<list>
#include<map>
#include<vector>
#include<array>
#include<Windows.h>

#ifdef UNICODE
typedef std::wstring fbxString;
typedef wchar_t fbxChar;
#else
typedef std::string fbxString;
typedef char ffbxChar;
#endif

#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"libfbxsdk-mt.lib")
#pragma comment(lib,"libfbxsdk.lib")

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  FbxLoader
	//	@brief  Fbx���[�_�[
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/28
	------------------------------------------------------------*/
	class FbxLoader
	{
	public:
		enum Type{
			Mesh = 0x1,
			Motion = 0x2
		};

	public:
		FbxLoader();
		~FbxLoader();

		/*!-----------------------------------------------------------
		//	@brief		�ǂݍ���
		//	@param[in]	_filePath	�t�@�C���p�X
		//	@param[in]	_loadType	�ǂݍ��݃^�C�v[�ȗ���]
		//	@return		true�F�ǂݍ��ݐ����@false�F�ǂݍ��ݎ��s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/28
		------------------------------------------------------------*/
		bool Load(const fbxString _filePath, int _loadType = Type::Mesh | Type::Motion);

	private:
		struct float2{
			float x, y;
		};

		struct float3{
			float x, y, z;
		};

		struct Vertex{
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

		struct FbxMaterial
		{
			std::string name;					///< ���O
			std::array<float, 4>	diffuse;		///< �f�B�t���[�Y
			std::array<float, 3>	ambient;		///< �A���r�G���g
			std::array<float, 3>	specular;	///< �X�y�L����
			std::array<float, 3>	emissive;	///< �G�~�b�V�u
			std::string diffuseTexPath;		///< �f�B�t���[�Y�̃e�N�X�`���p�X
			std::string specularTexPath;	///< �X�y�L�����̃e�N�X�`��
			std::string emissiveTexPath;	///< �G�~�b�V�u�̃e�N�X�`���p�X
			std::string normalTexPath;		///< �m�[�}���}�b�v�̃e�N�X�`���p�X
			std::string bumpTexPath;		///< �o���v�}�b�v�̃e�N�X�`���p�X
			unsigned long indexOffset;		///< �C���f�b�N�X�I�t�Z�b�g
			unsigned long indexCount;		///< �C���f�b�N�X��
			unsigned long faceCount;		///< �|���S����(�O�p�`)
		};

		struct BuildMesh{
			std::vector<Vertex> vertex;
			std::vector<DWORD> index;
			std::vector<DWORD> meshIndex;
			std::vector<FbxMaterial> material;
		};

		struct Float4x4{
			union{
				struct{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};
				struct{
					float m[16];
				};
			};
		};

		struct Frame{
			std::vector<Float4x4> mtx;
		};

		struct Animation{
			int beginFrame;				///< �A�j���[�V�����̊J�n�t���[��
			int endFrame;				///< �A�j���[�V�����̏I���t���[��
			int frameCount;				///< �t���[����
			std::vector<Frame> frame;	///< �t���[�����
		};

		struct MeshAnimation{
			FbxNode* pNode;					///< �m�[�h�|�C���^
			DWORD materialId;				///< �}�e���A��ID
		};

	public:
		BuildMesh buildMesh;				///< �\�z��̃��b�V��
		std::vector<Animation> animation;	///< �A�j���[�V�������
		int numMesh;						///< ���b�V����
		BOOL enableAnimation;				///< �A�j���[�V�����̗L��
	private:
		typedef std::vector<DWORD> IndexBuffer;
		typedef std::vector<Vertex> VertexBuffer;
		typedef std::vector<DWORD> MeshIdBuffer;
		FbxManager* lSdkManager;
		FbxIOSettings* ios;
		FbxImporter* lImporter;
		FbxScene* lScene;
		DWORD sumVertexCount;
		DWORD sumIndexCount;
		int materialCount;
		int nowMaterialId;
		int nowMeshId;
		std::map<DWORD, IndexBuffer> indexBuffer;		///< �}�e���A�����Ƃ̃C���f�b�N�X�f�[�^
		std::map<DWORD, VertexBuffer> vertexBuffer;		///< �}�e���A�����Ƃ̒��_�f�[�^
		std::map<DWORD, VertexBuffer> uvVertexBuffer;	///< UV�x�[�X�̒��_�o�b�t�@
		std::map<DWORD, MeshIdBuffer> meshIdBuffer;		///< ���b�V��ID�o�b�t�@
		std::map<DWORD, MeshIdBuffer> uvMeshIdBuffer;	///< UV�x�[�X�̃��b�V��ID�o�b�t�@
		std::vector<DWORD> vertexCount;					///< �}�e���A�����Ƃ̒��_��
		std::vector<DWORD> uvVertexCount;				///< UV���_��
		std::vector<MeshAnimation> meshAnimationInfo;	///< ���b�V���A�j���[�V�������
		std::vector<FbxMaterial> materialBuffer;		///< �}�e���A��

	private:
		/*!-----------------------------------------------------------
		//	@brief		�t�H�����ǂݍ���
		//	@param[out]	_pOut		�o�͂���}�e���A��
		//	@param[in]	_pMaterial	�}�e���A��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void ReadPhongMaterial(FbxMaterial* _pOut, FbxSurfaceMaterial* _pMaterial);

		/*!-----------------------------------------------------------
		//	@brief		�����o�[�g���ǂݍ���
		//	@param[out]	_pOut		�o�͂���}�e���A��
		//	@param[in]	_pMaterial	�}�e���A��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void ReadLambertMaterial(FbxMaterial* _pOut, FbxSurfaceMaterial* _pMaterial);

		/*!-----------------------------------------------------------
		//	@brief		�}�e���A���擾
		//	@param[in]	_node	�m�[�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void GetMaterial(FbxNode* _node);

		/*!-----------------------------------------------------------
		//	@brief		���b�V���擾
		//	@param[in]	_node	�m�[�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/28
		------------------------------------------------------------*/
		void GetMesh(FbxNode* _node);

		/*!-----------------------------------------------------------
		//	@brief		�W�I���g�����擾
		//	@param[in]	_mesh	���b�V��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void GetGeometry(FbxMesh* _mesh);

		/*!-----------------------------------------------------------
		//	@brief		���_�擾
		//	@param[in]	_mesh	���b�V���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/28
		------------------------------------------------------------*/
		void GetVertex(FbxMesh* _mesh);

		/*!-----------------------------------------------------------
		//	@brief		�@���擾
		//	@param[in]	_mesh	���b�V���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/28
		------------------------------------------------------------*/
		void GetNormal(FbxMesh* _mesh);

		/*!-----------------------------------------------------------
		//	@brief		UV�擾
		//	@param[in]	_mesh	���b�V���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/28
		------------------------------------------------------------*/
		void GetVertexUV(FbxMesh* _mesh);

		/*!-----------------------------------------------------------
		//	@brief		�E�F�C�g�l�擾
		//	@param[in]	_mesh	���b�V���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/30
		------------------------------------------------------------*/
		void GetWeight(FbxMesh* _mesh);

		/*!-----------------------------------------------------------
		//	@brief		���b�V���A�j���[�V�����ǂݍ���
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/30
		------------------------------------------------------------*/
		void GetMeshAnimation();

		/*!-----------------------------------------------------------
		//	@brief		���b�V���̍\�z
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void BuildMesh();
	};
}

#endif // _Include_FbxLoader_hpp_

#endif