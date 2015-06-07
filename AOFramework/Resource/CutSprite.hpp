/*************************************************************
*	@file   	CutSprite.hpp
*	@brief  	CutSprite�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/03/12
*************************************************************/

#ifndef _Include_CutSprite_hpp_	// �C���N���[�h�K�[�h
#define _Include_CutSprite_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"Sprite.hpp"
#include"../Math/MathDefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  CutSprite
	//	@brief  UV�w�肵���X�v���C�g
	//	@author	Tatsunori Aoyama
	//	@date	2015/03/12
	------------------------------------------------------------*/
	class CutSprite : public Sprite
	{
	public:
		CutSprite(){};
		~CutSprite(){};

		/*!-----------------------------------------------------------
		//	@brief		�����_��UV�w�肵�Ē��_�o�b�t�@�𐶐�
		//	@param[in]	_pd3dDevice		�f�o�C�X�|�C���^
		//	@param[in]	_leftTop		UV����
		//	@param[in]	_rightTop		UV�E��
		//	@param[in]	_leftButtom		UV����
		//	@param[in]	_rightButtom	UV�E��
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		HRESULT	CreateCornerCutVertexBuffer(
			ID3D11Device* _pd3dDevice,
			const float2& _leftTop,
			const float2& _rightTop,
			const float2& _leftButtom,
			const float2& _rightButtom);

		/*!-----------------------------------------------------------
		//	@brief		���S��_��UV�w�肵�Ē��_�o�b�t�@�𐶐�
		//	@param[in]	_pd3dDevice		�f�o�C�X�|�C���^
		//	@param[in]	_leftTop		UV����
		//	@param[in]	_rightTop		UV�E��
		//	@param[in]	_leftButtom		UV����
		//	@param[in]	_rightButtom	UV�E��
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		HRESULT	CreateCenterCutVertexBuffer(
			ID3D11Device* _pd3dDevice,
			const float2& _leftTop,
			const float2& _rightTop,
			const float2& _leftButtom,
			const float2& _rightButtom);

		/*!-----------------------------------------------------------
		//	@brief		�����_��UV�w�肵�Ē��_�o�b�t�@�𐶐�
		//	@param[in]	_pd3dDevice		�f�o�C�X�|�C���^
		//	@param[in]	_leftTop		UV����
		//	@param[in]	_rightTop		UV�E��
		//	@param[in]	_leftButtom		UV����
		//	@param[in]	_rightButtom	UV�E��
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		HRESULT	CreateCornerCutVertexBuffer(
			ID3D11Device* _pd3dDevice,
			const float2&& _leftTop,
			const float2&& _rightTop,
			const float2&& _leftButtom,
			const float2&& _rightButtom);

		/*!-----------------------------------------------------------
		//	@brief		���S��_��UV�w�肵�Ē��_�o�b�t�@�𐶐�
		//	@param[in]	_pd3dDevice		�f�o�C�X�|�C���^
		//	@param[in]	_leftTop		UV����
		//	@param[in]	_rightTop		UV�E��
		//	@param[in]	_leftButtom		UV����
		//	@param[in]	_rightButtom	UV�E��
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		HRESULT	CreateCenterCutVertexBuffer(
			ID3D11Device* _pd3dDevice,
			const float2&& _leftTop,
			const float2&& _rightTop,
			const float2&& _leftButtom,
			const float2&& _rightButtom);

		/*!-----------------------------------------------------------
		//	@brief		�O�p�`�̒��_���擾
		//  @return     �����_�����O���钸�_��
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/13
		------------------------------------------------------------*/
		const u32 GetVertexCount() const override {
			return 4;
		}
	};
}

#endif // _Include_CutSprite_hpp_