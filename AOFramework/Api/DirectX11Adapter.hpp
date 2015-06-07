/*************************************************************
*	@file   	DirectX11Adapter.hpp
*	@brief  	DirectX11Adapterクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/07/15
*************************************************************/

#ifndef _Include_DirectX11Adapter_hpp_	// インクルードガード
#define _Include_DirectX11Adapter_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"DirectX11Header.hpp"
#include"../Utility/SmartComPtr.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11Adapter
	//	@brief  アダプタークラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/15
	------------------------------------------------------------*/
	class DirectX11Adapter
	{
	public:
		DirectX11Adapter();
		~DirectX11Adapter();

		/*!-----------------------------------------------------------
		//	@brief		アダプター作成
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/15
		------------------------------------------------------------*/
		HRESULT	Create();

	public:
		SmartComPtr<IDXGIAdapter>	inst;
		static	SmartComPtr<IDXGIFactory>	factory;
	};
}

#endif // _Include_DirectX11Adapter_hpp_
