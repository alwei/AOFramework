#if 0	//obj�̎d�l���o���o���Ȃ̂Ń��[�_�[�͓���

/*************************************************************
*	@file   	ObjLoader.hpp
*	@brief  	ObjLoader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/09/26
*************************************************************/

#ifndef _Include_ObjLoader_hpp_	// �C���N���[�h�K�[�h
#define _Include_ObjLoader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<vector>

#ifdef UNICODE
typedef std::wstring objString;
typedef wchar_t objChar;
#else
typedef std::objString objString;
typedef char objChar;
#endif


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  ObjLoader
	//	@brief  Obj���[�_�[
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/26
	------------------------------------------------------------*/
	class ObjLoader
	{
	public:
		ObjLoader();
		~ObjLoader();

		/*!-----------------------------------------------------------
		//	@brief		�ǂݍ���
		//	@param[in]	_filePath	�t�@�C���p�X
		//	@param[in]	_mtlPath	MTL�t�@�C���p�X[�ȗ���]
		//	@return		true�F�ǂݍ��ݐ����@false�F�ǂݍ��ݎ��s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		bool Load(const objString _filePath,const objString _mtlPath = _T(""));

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
					float x, y, z;
					float nx, ny, nz;
					float u, v;
				};
				struct{
					float pos[3];
					float n[3];
					float uv[2];
				};
			};
		};

		struct Material{
			std::array<float, 3>	ambient;	///< ����
			std::array<float, 4>	diffuse;	///< Diffuse����
			std::array<float, 4>	specular;	///< ���ʔ��˂̐���(a:�X�y�L�������x�j
			std::array<objString,4>	textures;	///< �e�N�X�`���z��
			objString name;						///< �}�e���A����
		};

	private:
		std::vector<float3> positionList;
		std::vector<float3> normalList;
		std::vector<float2> texcoordList;
		std::vector<Vertex> vertexList;
		std::vector<DWORD>	indexList;
		std::vector<Material*> materialList;
		objString directoryPath;
		objString materialName;
		DWORD dwFaceIndex;
		DWORD dwFaceCount;
		DWORD dwCurSubset;
		int iMtlCount;
		BOOL loadFaild;
	private:

		/*!-----------------------------------------------------------
		//	@brief		���_�f�[�^�ǂݍ���
		//	@param[in]	_file	�t�@�C��IO
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	void ReadPosition(std::ifstream& _file);

		/*!-----------------------------------------------------------
		//	@brief		�@���x�N�g���ǂݎ��
		//	@param[in]	_file	�t�@�C��IO
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	void ReadNormal(std::ifstream& _file);

		/*!-----------------------------------------------------------
		//	@brief		�e�N�X�`�����W�ǂݍ���
		//	@param[in]	_file	�t�@�C��IO
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	void ReadTexcoord(std::ifstream& _file);

		/*!-----------------------------------------------------------
		//	@brief		�t�F�C�X�ǂݍ���
		//	@param[in]	_file	�t�@�C��IO
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	void ReadFace(std::ifstream& _file);

		/*!-----------------------------------------------------------
		//	@brief		CreateMaterial
		//	@param[in]	_file	�t�@�C��IO
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	void CreateMaterial(std::ifstream& _file);

		/*!-----------------------------------------------------------
		//	@brief		�o�b�t�@�`�F�b�N
		//	@param[in]	_file	�t�@�C��IO
		//	@param[in]	_buffer	�ǂݍ��ݗp�̃o�b�t�@
		//	@return		true�F�������s�@false�F�����I��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		inline	bool CheckBuffer(std::ifstream& _file, char* _buffer);

		/*!-----------------------------------------------------------
		//	@brief		�}�e���A��������
		//	@param[in]	_pMaterial	�������}�e���A��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	void InitMaterial(Material* _pMaterial);

		/*!-----------------------------------------------------------
		//	@brief		�W�I���g���ǂݍ��ݏ���
		//	@param[in]	_filePath	�t�@�C���p�X
		//	@return		true�F�������s�@false�F�����I��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		inline	bool ReadGeometry(const objString& _filePath);

		/*!-----------------------------------------------------------
		//	@brief		MTL�t�@�C���̓ǂݍ���
		//	@param[in]	_mtlPath	�t�@�C���p�X
		//	@return		true�F�ǂݍ��ݐ����@false�F�ǂݍ��ݎ��s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	bool LoadMtl(const objString& _mtlPath);

		/*!-----------------------------------------------------------
		//	@brief		Mtl�ǂݍ���
		//	@param[in]	_file	�t�@�C��IO
		//	@param[in]	_buffer	�ǂݍ��ݗp�̃o�b�t�@
		//	@return		true�F�������s	false�F�����I��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	bool ReadMtl(std::ifstream& _file);
	};
}

#endif // _Include_ObjLoader_hpp_

#endif