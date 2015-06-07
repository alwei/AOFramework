/*************************************************************
*	@file   	BaseTexture.hpp
*	@brief  	BaseTextureクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/01
*************************************************************/

#ifndef _Include_BaseTexture_hpp_	// インクルードガード
#define _Include_BaseTexture_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<d3d11.h>
#include"IResource.hpp"
#include"../Utility/SmartComPtr.hpp"
#include"../Define/SystemDefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BaseTexture
	//	@brief  テクスチャ基底クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/01
	------------------------------------------------------------*/
	class BaseTexture : public IResource
	{
	public:
		BaseTexture(){};
		virtual	~BaseTexture(){};

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const override {
			return ResourceType::TEXTURE;
		}

	public:
		string filePath;
		string fileName;
		SmartComPtr<ID3D11ShaderResourceView>	pSrView;
		SmartComPtr<ID3D11SamplerState>			pSampState;
	};
}

#endif // _Include_BaseTexture_hpp_