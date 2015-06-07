/*************************************************************
*	@file   	PointSprite.hpp
*	@brief  	PointSpriteクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/01
*************************************************************/

#ifndef _Include_PointSprite_hpp_	// インクルードガード
#define _Include_PointSprite_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"BaseGeometry.hpp"
#include"BaseTexture.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  PointSprite
	//	@brief  ポイントスプライト
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/01
	------------------------------------------------------------*/
	class PointSprite : public BaseGeometry
	{
	public:
		PointSprite(){};
		virtual	~PointSprite(){};

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
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