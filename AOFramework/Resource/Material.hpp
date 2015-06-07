/*************************************************************
*	@file   	Material.hpp
*	@brief  	Materialクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/19
*************************************************************/

#ifndef _Include_Material_hpp_	// インクルードガード
#define _Include_Material_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<array>
#include"IResource.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  Material
	//	@brief  マテリアル
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/19
	------------------------------------------------------------*/
	class Material : public IResource
	{
	public:
		Material()
		{
			for (auto& it : this->diffuse)
			{
				it = 1.0f;
			}
			for (auto& it : this->ambient)
			{
				it = 0.5f;
			}
			for (auto& it : this->specular)
			{
				it = 1.0f;
			}
			for (auto& it : this->emissive)
			{
				it = 0;
			}
		};
		virtual	~Material(){};

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		virtual	const ResourceType GetType() const 
		{
			return ResourceType::MATERIAL;
		}

	public:
		std::array<float, 4> diffuse;
		std::array<float, 3> ambient;
		std::array<float, 3> specular;
		std::array<float, 3> emissive;
	};
}

#endif // _Include_Material_hpp_