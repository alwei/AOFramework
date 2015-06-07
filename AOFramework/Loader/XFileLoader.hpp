/*************************************************************
*	@file   	XFileLoader.hpp
*	@brief  	XFileLoader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/18
*************************************************************/

#ifndef _Include_XFileLoader_hpp_	// �C���N���[�h�K�[�h
#define _Include_XFileLoader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<fstream>
#include<Windows.h>
#include<string>
#include<vector>
#include<algorithm>
#include<functional>
#include<map>
#include<array>

#ifdef UNICODE
typedef std::wstring xString;
typedef wchar_t xChar;
#else
typedef std::string xString;
typedef char xChar;
#endif

namespace EXTRACT_BIT
{
	const long VERTEX	= 0x00000001;
	const long INDEX	= 0x00000002;
	const long NORMAL	= 0x00000004;
	const long MATERIAL	= 0x00000008;
};


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  XFileLoader
	//	@brief  X�t�@�C�����[�_�[
	//	@author	Tatsunori Aoyama
	//	@date	2013/10/19
	------------------------------------------------------------*/
	class XFileLoader
	{
	public:
		struct Vertex{
			union{
				struct{
					float x, y, z;
					float nx, ny, nz;
					float u, v;
				};
				struct{
					float baseData[8];
				};
			};
		};

		struct float3{
			union{
				float f[3];
				struct{
					float x, y, z;
				};
			};
		};

		struct Material
		{
			std::array<float, 4>	diffuse;	///< Diffuse����
			std::array<float, 4>	specular;	///< ���ʔ��˂̐���(a:�X�y�L�������x�j
			std::array<float, 3>	emissive;	///< ���Ȕ����̐���
			std::string texture;			///< �e�N�X�`���z��
			int		indexCount;				///< �}�e���A���Ɉˑ����Ă���C���f�b�N�X��
		};

		class SortIndex
		{
		public:
			int		index;			///< �}�e���A���̃C���f�b�N�X�ԍ�
			int		indexArray[4];	///< 1�v�f�̃C���f�b�N�X�z��
			bool	isQuad;			///< �l�p�`���ǂ����H
		public:
			bool	operator < (const SortIndex& inst){
				return (this->index < inst.index);
			}
			bool	operator >(const SortIndex& inst){
				return (this->index > inst.index);
			}
			bool	operator <= (const SortIndex& inst){
				return (this->index <= inst.index);
			}
			bool	operator >= (const SortIndex& inst){
				return (this->index >= inst.index);
			}
			bool	operator == (const SortIndex& inst){
				return (this->index == inst.index);
			}
		};

	public:
		/// �R���X�g���N�^
		XFileLoader();

		/// �f�X�g���N�^
		~XFileLoader();

		/*!-----------------------------------------------------------
		//	@brief		�ǂݍ��ݏ���
		//	@param[in]	_filePath	�t�@�C���p�X
		//	@return		true�F���o�����@false�F���o���s
		//	@author		Tatsunori Aoyama
		//	@date		2013/10/19
		------------------------------------------------------------*/
		bool	Load(const xString _filePath);

		typedef std::vector<INT> SectionIndex;
		std::vector<Vertex> serialVertex;
		std::vector<INT> serialIndex;
		std::vector<SectionIndex> sectionIndex;
		std::vector<Material> material;
		std::vector<SortIndex>	sorter;

	private:
		/*!-----------------------------------------------------------
		//	@brief		string���璸�_�f�[�^���o
		//	@param[in]	_buffer		������o�b�t�@
		//	@return		true�F���o����	false�F���o���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/18
		------------------------------------------------------------*/
		inline	bool	LoadVertex(const std::string& _buffer);

		/*!-----------------------------------------------------------
		//	@brief		�e�N�X�`�����W�ǂݍ���
		//	@param[in]	_buffer	������o�b�t�@
		//	@return		true�F���o�����@false�F���o���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		inline	bool	LoadTexcoord(const std::string& _buffer);

		/*!-----------------------------------------------------------
		//	@brief		string����}�e���A���f�[�^���o
		//	@param[in]	_buffer	������o�b�t�@
		//	@return		true�F���o����	false�F���o���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/18
		------------------------------------------------------------*/
		inline	bool	LoadMaterial(const std::string& _buffer);

		/*!-----------------------------------------------------------
		//	@brief		string���猋���f�[�^���o
		//	@param[in]	_buffer	������o�b�t�@
		//	@return		true�F���o����	false�F���o���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/18
		------------------------------------------------------------*/
		inline	bool	LoadIndex(const std::string& _buffer);

		/*!-----------------------------------------------------------
		//	@brief		���_�f�[�^����@�����Z�o
		//	@return		true�F���o����	false�F���o���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/18
		------------------------------------------------------------*/
		inline	bool	CreateNormal();

		/*!-----------------------------------------------------------
		//	@brief		�C���f�b�N�X���}�e���A���C���f�b�N�X�Ɋ�Â��ă\�[�g�i�[
		//	@return		true�F�i�[�\�[�g����	false�F���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/18
		------------------------------------------------------------*/
		inline	bool	StoreSortIndex();
	};
}

#endif // _Include_XFileLoader_hpp_