/*************************************************************
*	@file   	LightingPackage.hpp
*	@brief  	LightingPackageクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/04/30
*************************************************************/

#ifndef _Include_LightingPackage_hpp_	// インクルードガード
#define _Include_LightingPackage_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<d3d11.h>
#include"LightMaterial.hpp"
#include"../Define/SystemDefine.hpp"


namespace ao
{
	namespace MAX_LIGHT
	{
		static const u16 DIR = 8;
		static const u16 POINT = 512;
		static const u16 SPOT = 8;
	}

	/*!-----------------------------------------------------------
	//	@class  LightingPackage
	//	@brief  ライティングパッケージ
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/30
	------------------------------------------------------------*/
	class LightingPackage : public IResource
	{
	public:
		/// コンストラクタ
		LightingPackage(){};

		/// デストラクタ
		~LightingPackage()override final{};

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const override final
		{
			return ResourceType::LIGHTING_PKG;
		};

	public:
		std::array<DirectinalLightMaterial, MAX_LIGHT::DIR> dirLightMat;
		std::array<PointLightMaterial, MAX_LIGHT::POINT> pointLightMat;
		std::array<SpotLightMaterial, MAX_LIGHT::SPOT> spotLightMat;
		std::array<float3, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE> eyePosition;
	};
}

#endif // _Include_LightingPackage_hpp_