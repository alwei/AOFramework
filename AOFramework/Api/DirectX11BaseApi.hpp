/*************************************************************
*	@file   	DirectX11BaseApi.hpp
*	@brief  	DirectX11BaseApiクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/06/27
*************************************************************/

#ifndef _Include_DirectX11BaseApi_hpp_	// インクルードガード
#define _Include_DirectX11BaseApi_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"DirectX11BaseCore.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11BaseApi
	//	@brief  11専用API基底クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/06/26
	------------------------------------------------------------*/
	class DirectX11BaseApi : public DirectX11BaseCore
	{
	public:
		DirectX11BaseApi(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) : 
			pd3dDeviceContext(_pd3dDeviceContext),
			DirectX11BaseCore(_pd3dDevice){}
		virtual	~DirectX11BaseApi(){}

		/*!-----------------------------------------------------------
		//	@brief		デバイスコンテキスト取得
		//	@return		デバイスコンテキストポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		SmartComPtr<ID3D11DeviceContext> GetDeviceContext()
		{
			return this->pd3dDeviceContext;
		}

	protected:
		SmartComPtr<ID3D11DeviceContext>	pd3dDeviceContext;	///< デバイスコンテキストポインタ
	};
}

#endif // _Include_DirectX11BaseApi_hpp_
