/*************************************************************
*	@file   	BaseGeometry.hpp
*	@brief  	BaseGeometry�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/01
*************************************************************/

#ifndef _Include_BaseGeometry_hpp_	// �C���N���[�h�K�[�h
#define _Include_BaseGeometry_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<d3d11.h>
#include"IResource.hpp"
#include"../Utility/SmartComPtr.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BaseGeometry
	//	@brief  �W�I���g�����N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/01
	------------------------------------------------------------*/
	class BaseGeometry : public IResource
	{
	public:
		BaseGeometry(){};
		virtual	~BaseGeometry(){};

		/*!-----------------------------------------------------------
		//	@brief		���_���擾
		//  @return     ���_��
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/13
		------------------------------------------------------------*/
		virtual const u32 GetVertexCount() const {
			return 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		���_�X�g���C�h�擾
		//	@return		���_�X�g���C�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		virtual const u32 GetStride() const {
			return 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		���_�f�[�^�T�C�Y�擾
		//	@return		���_�f�[�^�T�C�Y
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		virtual const u32 GetVertexBufferSize() const {
			return 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		���_�o�b�t�@�擾
		//	@return		���_�o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/07
		------------------------------------------------------------*/
		virtual ID3D11Buffer** GetVertexBufferPtr() final{
			return this->pGpuVertexBuffer.GetComPtrPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		���_�o�b�t�@���O���ō쐬�p�Ƀ|�C���^��n��
		//  @return     ���_�o�b�t�@�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/12
		------------------------------------------------------------*/
		virtual ID3D11Buffer** ToVertexBufferCreator() final {
			return this->pGpuVertexBuffer.ToCreator();
		}

		/*!-----------------------------------------------------------
		//	@brief		���_�o�b�t�@�̎Q�ƌ����R�s�[
		//	@note		�A�v���P�[�V���������ł͎g�p���Ȃ��ł�������
		//	@param[in]	_srcSprite	�Q�ƌ��̃X�v���C�g
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/08
		------------------------------------------------------------*/
		template<class T>
		void CopyVertexBufferRef(T& _src){
			this->pGpuVertexBuffer = _src.pGpuVertexBuffer;
		}

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		virtual const ResourceType GetType() const override {
			return ResourceType::GEOMETRY;
		}

	protected:
		SmartComPtr<ID3D11Buffer> pGpuVertexBuffer;
	};
}

#endif // _Include_BaseGeometry_hpp_