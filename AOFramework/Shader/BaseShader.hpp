/*************************************************************
*	@file   	BaseShader.hpp
*	@brief  	BaseShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/10
*************************************************************/

#ifndef _Include_BaseShader_hpp_	// インクルードガード
#define _Include_BaseShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../Define/SystemDefine.hpp"
#include"../Api/DirectX11BaseApi.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BaseShader
	//	@brief  シェーダー基底クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/10
	------------------------------------------------------------*/
	class BaseShader : public DirectX11BaseApi
	{
	public:
		BaseShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
			DirectX11BaseApi(_pd3dDevice,_pd3dDeviceContext)
		{
			this->shaderName.assign(_T("no name shader"));
		};

		virtual	~BaseShader()override{};

		/*!-----------------------------------------------------------
		//	@brief		シェーダー名を命名
		//	@param[in]	_name	設定する命名
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/14
		------------------------------------------------------------*/
		void Naming(ao::string _name){
			this->shaderName.assign(_name);
		}

		/*!-----------------------------------------------------------
		//	@brief		シェーダー名取得
		//	@return		シェーダー名
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/14
		------------------------------------------------------------*/
		AOLPSTR GetName() const{
			this->shaderName.c_str();
		}

	protected:
		ao::string shaderName;
	};
}

#endif // _Include_BaseShader_hpp_