/*************************************************************
*	@file   	PointSprite.hpp
*	@brief  	PointSprite�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/01
*************************************************************/

#ifndef _Include_PointSprite_hpp_	// �C���N���[�h�K�[�h
#define _Include_PointSprite_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"BaseGeometry.hpp"
#include"BaseTexture.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  PointSprite
	//	@brief  �|�C���g�X�v���C�g
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/01
	------------------------------------------------------------*/
	class PointSprite : public BaseGeometry
	{
	public:
		PointSprite(){};
		virtual	~PointSprite(){};

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const override {
			return ResourceType::POINTSPRITE;
		}

	public:
		BaseTexture texture;
	};
}

#endif // _Include_PointSprite_hpp_