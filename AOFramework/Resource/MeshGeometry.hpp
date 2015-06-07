/*************************************************************
*	@file   	Geometry.hpp
*	@brief  	Geometry�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/04
*************************************************************/

#ifndef _Include_Geometry_hpp_	// �C���N���[�h�K�[�h
#define _Include_Geometry_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<vector>
#include"../Define/SystemDefine.hpp"
#include"BaseGeometry.hpp"
#include"Vertex.hpp"

namespace ao
{
	/// ��ꃁ�b�V���W�I���g��
	class BaseMeshGeometry : public BaseGeometry
	{
	public:
		BaseMeshGeometry(){}
		virtual ~BaseMeshGeometry(){}

		virtual void* GetVertexBuffer()
		{
			return nullptr;
		}

		/*!-----------------------------------------------------------
		//	@brief		�C���f�b�N�o�b�t�@�̃f�[�^�T�C�Y�擾
		//  @return     �C���f�b�N�X�o�b�t�@�̃f�[�^�T�C�Y
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/12
		------------------------------------------------------------*/
		virtual const u32 GetIndexBufferSize() const final
		{
			return static_cast<u32>(this->index.size() * sizeof(u32));
		}

		/*!-----------------------------------------------------------
		//	@brief		�C���f�b�N�X�o�b�t�@�̃f�[�^���擾
		//  @return     �C���f�b�N�X�o�b�t�@�̃f�[�^
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/12
		------------------------------------------------------------*/
		virtual const u32* GetIndexBuffer() const final
		{
			return this->index.data();
		}

		/*!-----------------------------------------------------------
		//	@brief		���_�o�b�t�@�擾
		//	@return		���_�o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		virtual ID3D11Buffer** GetIndexBufferPtr() final
		{
			return this->pGpuIndexBuffer.GetComPtrPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		�C���f�b�N�X�o�b�t�@���O���ō쐬�p�Ƀ|�C���^��n��
		//	@param[in]	�C���f�b�N�X�|�C���^
		//  @return     ���_�o�b�t�@�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		virtual ID3D11Buffer** ToIndexBufferCreator() final 
		{
			return this->pGpuIndexBuffer.ToCreator();
		}

		/*!-----------------------------------------------------------
		//	@brief		���_�o�b�t�@�̎Q�ƌ����R�s�[
		//	@note		�A�v���P�[�V���������ł͎g�p���Ȃ��ł�������
		//	@param[in]	_srcSprite	�Q�ƌ��̃X�v���C�g
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		template<class T>
		void CopyIndexBufferRef(T& _src)
		{
			this->pGpuIndexBuffer = _src.pGpuIndexBuffer;
		}

	public:
		std::vector<u32> index;
		SmartComPtr<ID3D11Buffer> pGpuIndexBuffer;
	};

	/// �ÓI���b�V���W�I���g��
	class StMeshGeometry : public BaseMeshGeometry
	{
	public:
		void* GetVertexBuffer() final
		{
			return this->vertex.data();
		}

		const u32 GetStride() const override final
		{
			return sizeof(StVertex);
		}

		const u32 GetVertexBufferSize() const override final
		{
			return static_cast<u32>(sizeof(StVertex)*this->vertex.size());
		}

		const ResourceType GetType() const override final
		{
			return ResourceType::ST_GEOMETRY;
		}

	public:
		std::vector<StVertex> vertex;
	};


	/// ���I���b�V���W�I���g��
	class SkMeshGeometry : public BaseMeshGeometry
	{
	public:
		void* GetVertexBuffer() final
		{
			return this->vertex.data();
		}

		const u32 GetStride() const override final
		{
			return sizeof(SkVertex);
		}

		const u32 GetVertexBufferSize() const override final
		{
			return static_cast<u32>(sizeof(SkVertex)*this->vertex.size());
		}

		const ResourceType GetType() const override final 
		{
			return ResourceType::SK_GEOMETRY;
		}

	public:
		std::vector<SkVertex> vertex;
	};
}

#endif // _Include_Geometry_hpp_