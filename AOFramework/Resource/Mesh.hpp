/*************************************************************
*	@file   	Mesh.hpp
*	@brief  	Mesh�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/19
*************************************************************/

#ifndef _Include_Mesh_hpp_	// �C���N���[�h�K�[�h
#define _Include_Mesh_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<array>
#include<vector>
#include"../Define/SystemDefine.hpp"
#include"BaseGeometry.hpp"
#include"BaseTexture.hpp"
#include"Material.hpp"
#include"MeshGeometry.hpp"

namespace ao
{
	enum class EdgeType : int
	{
		Soft,
		Hard
	};

	/*!-----------------------------------------------------------
	//	@class  BaseMesh
	//	@brief  ��ꃁ�b�V���N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class BaseMesh : public IResource
	{
	public:
		BaseMesh(){};
		virtual ~BaseMesh(){};

		/*!-----------------------------------------------------------
		//	@brief		���_�o�b�t�@�擾
		//	@return		���_�o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		virtual ID3D11Buffer** GetVertexBuffer()
		{
			return nullptr;
		}

		/*!-----------------------------------------------------------
		//	@brief		�C���f�b�N�X�o�b�t�@�擾
		//	@return		�C���f�b�N�X�o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		virtual ID3D11Buffer* GetIndexBuffer() 
		{
			return nullptr;
		}

		/*!-----------------------------------------------------------
		//	@brief		�X�g���C�h�擾
		//	@return		�X�g���C�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		virtual const u32 GetStride() const 
		{
			return 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		virtual const ResourceType GetType() const override 
		{
			return ResourceType::BASE_MESH;
		}

	public:	/// ��`
		enum class TextureType : int
		{
			Diffuse = 0,
			Specular,
			Emissive,
			NormalMap,
			BumpMap,
			Max
		};

	public:
		/// �T�u�Z�b�g
		class Subset : public IResource
		{
		public:
			const ResourceType GetType() const override final
			{
				return ResourceType::SUBSET;
			}

			Material material;
			std::vector<u32> index;
			std::array<BaseTexture*, (int)TextureType::Max> pTextures;
		};

	public:
		typedef std::vector<Subset> Subsets;
		Subsets subset;
	};


	/*!-----------------------------------------------------------
	//	@class  StMesh
	//	@brief  �ÓI���b�V���N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/19
	------------------------------------------------------------*/
	class StMesh : public BaseMesh
	{
	public:
		StMesh(){};
		~StMesh(){};

		/*!-----------------------------------------------------------
		//	@brief		���_�o�b�t�@�擾
		//	@return		���_�o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		ID3D11Buffer** GetVertexBuffer() override final
		{
			return this->geometry.GetVertexBufferPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		�C���f�b�N�X�o�b�t�@�擾
		//	@return		�C���f�b�N�X�o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		ID3D11Buffer* GetIndexBuffer() final
		{
			return this->geometry.pGpuIndexBuffer.GetComPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		const ResourceType GetType() const override
		{
			return ResourceType::ST_MESH;
		}

		/*!-----------------------------------------------------------
		//	@brief		�X�g���C�h�擾
		//	@return		�X�g���C�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		const u32 GetStride() const override final
		{
			return sizeof(StVertex);
		}

	public:
		StMeshGeometry geometry;
	};


	/*!-----------------------------------------------------------
	//	@class  StaMesh
	//	@brief  �ÓI�A�j���[�V�������b�V���N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/19
	------------------------------------------------------------*/
	class StaMesh : public StMesh
	{
	public:
		StaMesh(){};
		~StaMesh() final{};

		/*!-----------------------------------------------------------
		//	@brief		���b�V���C���f�b�N�X�o�b�t�@�擾
		//	@return		���b�V���C���f�b�N�X�o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/30
		------------------------------------------------------------*/
		ID3D11Buffer** GetMeshIndexBuffer()
		{
			return this->meshIndexBuffer.GetComPtrPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		�A�j���[�V�����C���f�b�N�X�̃T�C�Y�擾
		//  @return     �A�j���[�V�����C���f�b�N�X�̃T�C�Y
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		const size_t GetMeshIndexSize() const 
		{
			return sizeof(int)*this->meshIndex.size();
		}

		/*!-----------------------------------------------------------
		//	@brief		�A�j���[�V�����C���f�b�N�X�̃|�C���^�擾
		//  @return     ���b�V��ID�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		void* GetMeshIndexData()
		{
			return this->meshIndex.data();
		}

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		const ResourceType GetType() const override final
		{
			return ResourceType::STA_MESH;
		}

	public:
		std::vector<s32> meshIndex;
		SmartComPtr<ID3D11Buffer> meshIndexBuffer;
	};


	/*!-----------------------------------------------------------
	//	@class  SkMesh
	//	@brief  ���I���b�V���N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class SkMesh : public BaseMesh
	{
	public:
		SkMesh(){};
		~SkMesh(){};

		/*!-----------------------------------------------------------
		//	@brief		���_�o�b�t�@�擾
		//	@return		���_�o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		ID3D11Buffer** GetVertexBuffer() override final 
		{
			return this->geometry.GetVertexBufferPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		�C���f�b�N�X�o�b�t�@�擾
		//	@return		�C���f�b�N�X�o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		ID3D11Buffer* GetIndexBuffer() override final
		{
			return this->geometry.pGpuIndexBuffer.GetComPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		const ResourceType GetType() const override final
		{
			return ResourceType::ST_MESH;
		}

		/*!-----------------------------------------------------------
		//	@brief		�X�g���C�h�擾
		//	@return		�X�g���C�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		const u32 GetStride() const override final
		{
			return sizeof(SkVertex);
		}

	public:
		SkMeshGeometry geometry;
	};
}

#endif // _Include_Mesh_hpp_