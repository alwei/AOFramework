/*************************************************************
*	@file   	LightMaterial.hpp
*	@brief  	LightMaterialクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/04/29
*************************************************************/

#ifndef _Include_LightMaterial_hpp_	// インクルードガード
#define _Include_LightMaterial_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<array>
#include"IResource.hpp"
#include"../Math/MathDefine.hpp"
#include"../Math/GameArithmetic.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectinalLightMaterial
	//	@brief  ライトマテリアル
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/29
	------------------------------------------------------------*/
	class BaseLightMaterial : public IResource
	{
	public:
		/// コンストラクタ
		BaseLightMaterial():lightColor(color::white),range(10000),isEnable(false)
		{
			float attn = 1.0f / 3.0f;
			this->attenuation[0] = attn;
			this->attenuation[1] = attn;
			this->attenuation[2] = attn;
		};

		/// デストラクタ
		virtual ~BaseLightMaterial()override{};

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		virtual	const ResourceType GetType() const override
		{
			return ResourceType::INVALID;
		}

		inline void operator=(const bool _isEnable)
		{
			this->isEnable = _isEnable;
		}

	public:
		Transform transform;
		color lightColor;
		std::array<float, 3> attenuation;
		float range;
		bool isEnable;
	};


	/*!-----------------------------------------------------------
	//	@class  DirectinalLightMaterial
	//	@brief  ディレクショナルライトマテリアル
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/29
	------------------------------------------------------------*/
	class DirectinalLightMaterial : public BaseLightMaterial
	{
	public:
		/// コンストラクタ
		DirectinalLightMaterial():lightDir(float3::down){};

		/// デストラクタ
		~DirectinalLightMaterial()override final{};

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const override final
		{
			ResourceType::DIRECTIONAL_LIGHT;
		}

	public:
		float3 lightDir;
	};


	/*!-----------------------------------------------------------
	//	@class  PointLightMaterial
	//	@brief  ポイントライトマテリアル
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/29
	------------------------------------------------------------*/
	class PointLightMaterial : public BaseLightMaterial
	{
	public:
		/// コンストラクタ
		PointLightMaterial(){};

		/// デストラクタ
		~PointLightMaterial()override final{};

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const override final
		{
			ResourceType::POINT_LIGHT;
		}
	};


	/*!-----------------------------------------------------------
	//	@class  PointLightMaterial
	//	@brief  ポイントライトマテリアル
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/29
	------------------------------------------------------------*/
	class SpotLightMaterial : public BaseLightMaterial
	{
	public:
		/// コンストラクタ
		SpotLightMaterial(){};

		/// デストラクタ
		~SpotLightMaterial()override final{};

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const override final
		{
			ResourceType::SPOT_LIGHT;
		}
	};
}

#endif // _Include_LightMaterial_hpp_