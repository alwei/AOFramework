/*************************************************************
*	@file   	CustomSprite.hpp
*	@brief  	CustomSprite�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/03/08
*************************************************************/

#ifndef _Include_CustomSprite_hpp_	// �C���N���[�h�K�[�h
#define _Include_CustomSprite_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"Sprite.hpp"
#include"../Math/MathDefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  CustomSprite
	//	@brief  �J�X�^���X�v���C�g
	//	@author	Tatsunori Aoyama
	//	@date	2015/03/08
	------------------------------------------------------------*/
	template<class T>
	class CustomSprite : public Sprite
	{
	public:
		CustomSprite():pBuffer(nullptr){};
		~CustomSprite(){
			if( this->pBuffer)
				delete this->pBuffer
		};

		/*!-----------------------------------------------------------
		//	@brief		�O�p�`�̒��_���擾
		//  @return     �����_�����O���钸�_��
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/13
		------------------------------------------------------------*/
		const size_t GetVertexCount() const override {
			return this->nVertexCount;
		}

		/*!-----------------------------------------------------------
		//	@brief		���_�X�g���C�h�擾
		//	@return		���_�X�g���C�h
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/13
		------------------------------------------------------------*/
		const u32 GetStride() const override {
			return this->nStride;
		}

		/*!-----------------------------------------------------------
		//	@brief		�}�j���A���Œ��_���o�b�t�@�쐬
		//	@param[in]	_pd3dDevice		�f�o�C�X�|�C���^
		//	@param[in]	_pSrcData		���_�f�[�^
		//  @param[in]  _nVertexCount	���_��
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/08
		------------------------------------------------------------*/
		HRESULT CreateCustomVertexBuffer(
			ID3D11Device* const _pd3dDevice,
			const T* const _pSrcData,
			const size_t _nVertexCount)
		{
			this->nStride = _nVertexCount;
			this->vBufferSize = sizeof(T)*_nVertexCount;
			this->pBuffer = new T[_nVertexCount];
			::CopyMemory(this->pBuffer, _pSrcData, this->vBufferSize);

			this->CreateCommonVertexBuffer(
				this->pGpuVertexBuffer.ToCreator(),
				_pd3dDevice,
				this->vBufferSize,
				_pSrcData);
		}

		/*!-----------------------------------------------------------
		//	@brief		�m�ۂ������_�f�[�^���擾
		//  @param[out]  _pOutBufferData	�o�͂���CPU���_�o�b�t�@�f�[�^[�ȗ���]
		//  @return     �o�͂���CPU���_�o�b�t�@�f�[�^
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/13
		------------------------------------------------------------*/
		const void* GetVertexBufferData(void* const _pOutBufferData = nullptr)
		{
			if (_pOutBufferData == nullptr)
				return this->pGpuVertexBuffer;

			::CopyMemory(_pOutBufferData, this->pGpuVertexBuffer, this->vBufferSize);
			return _pOutBufferData;
		}

	public:
		size_t nStride;
		size_t nVertexCount;
		T* pCpuVertexBuffer;
	};
}

#endif // _Include_CustomSprite_hpp_