/*************************************************************
*	@file   	Sprite.hpp
*	@brief  	Sprite�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/01
*************************************************************/

#ifndef _Include_Sprite_hpp_	// �C���N���[�h�K�[�h
#define _Include_Sprite_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"../Define/SystemDefine.hpp"
#include"BaseGeometry.hpp"
#include"BaseTexture.hpp"

namespace ao
{
	enum class SpritePivot : int
	{
		LeftTop = 0,	//�����ς�
		RightTop,		//TODO:������
		LeftButtom,		//TODO:������
		RightButtom,	//TODO:������
		Center,			//�����ς�
		TypeMax
	};

	/*!-----------------------------------------------------------
	//	@class  Sprite
	//	@brief  �X�v���C�g
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/01
	------------------------------------------------------------*/
	class Sprite : public BaseGeometry
	{
	public:
		Sprite();
		virtual	~Sprite(){};

		struct Vertex
		{
			float x, y, z;
			float nx, ny, nz;
			float u, v;
		};

		union UV
		{
			struct
			{
				float x, y;
			};
			struct
			{
				float f[2];
			};
		};

		/*!-----------------------------------------------------------
		//	@brief		���_�o�b�t�@�쐬(���㌴�_)
		//	@param[in]	�f�o�C�X�|�C���^
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/07
		------------------------------------------------------------*/
		HRESULT CreateCornerVertexBuffer(ID3D11Device* _pd3dDevice);

		/*!-----------------------------------------------------------
		//	@brief		���_�o�b�t�@�쐬(���S���_)
		//	@param[in]	�f�o�C�X�|�C���^
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/07
		------------------------------------------------------------*/
		HRESULT CreateCenterVertexBuffer(ID3D11Device* _pd3dDevice);

		/*!-----------------------------------------------------------
		//	@brief		�O�p�`�̒��_���擾
		//  @return     �����_�����O���钸�_��
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/13
		------------------------------------------------------------*/
		const u32 GetVertexCount() const override {
			return 3;
		}

		/*!-----------------------------------------------------------
		//	@brief		���_�X�g���C�h�擾
		//	@return		���_�X�g���C�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		const u32 GetStride() const override {
			return sizeof(Vertex);
		}

		/*!-----------------------------------------------------------
		//	@brief		���_�f�[�^�T�C�Y�擾
		//	@return		���_�f�[�^�T�C�Y
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		const u32 GetVertexBufferSize() const override{
			return this->vBufferSize;
		}

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const override {
			return ResourceType::SPRITE;
		}

		/*!-----------------------------------------------------------
		//	@brief		�s�{�b�g�^�C�v�擾
		//  @return     �s�{�b�g�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/08
		------------------------------------------------------------*/
		const SpritePivot GetPivotType() const {
			return this->pivotType;
		}
			
		/*!-----------------------------------------------------------
		//	@brief		�m�ۂ������_�f�[�^���擾
		//  @param[out]  _pOutBufferData	�o�͂���CPU���_�o�b�t�@�f�[�^[�ȗ���]
		//  @return     �o�͂���CPU���_�o�b�t�@�f�[�^
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/08
		------------------------------------------------------------*/
		const void* GetVertexBufferData(void* const _pOutBufferData=nullptr);

	public:
		BaseTexture* pTexture;

	protected:
		SpritePivot pivotType;
		u32 vBufferSize;

		/*!-----------------------------------------------------------
		//	@brief		���ʂ̒��_�o�b�t�@�쐬
		//	@param[out]	_pOutVBuffer	�o�͂��钸�_�o�b�t�@
		//	@param[in]	_pd3dDevice		�f�o�C�X�|�C���^
		//	@param[in]	_vBufferSize	���_�o�b�t�@�T�C�Y
		//	@param[in]	_pSrcData		�쐬���錳�f�[�^
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/07
		------------------------------------------------------------*/
		HRESULT CreateCommonVertexBuffer(
			ID3D11Buffer** _ppOutVBuffer,
			ID3D11Device* const _pd3dDevice,
			const UINT _vBufferSize,
			const void* const _pSrcData);
	};
}

#endif // _Include_Sprite_hpp_