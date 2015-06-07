/*************************************************************
*	@file   	DirectX11BaseCore.hpp
*	@brief  	DirectX11BaseCoreクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/04/27
*************************************************************/

#ifndef _Include_DirectX11BaseCore_hpp_	// インクルードガード
#define _Include_DirectX11BaseCore_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"DirectX11Header.hpp"
#include"../Define/SystemDefine.hpp"
#include"../Utility/SmartComPtr.hpp"
#include"../Debug/DebugSystem.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11BaseCore
	//	@brief  ID3D11Deviceを持つコアクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/27
	------------------------------------------------------------*/
	class DirectX11BaseCore
	{
	public:
		/// コンストラクタ
		DirectX11BaseCore(SmartComPtr<ID3D11Device> _pd3dDevice) : pd3dDevice(_pd3dDevice){};

		/// デストラクタ
		virtual ~DirectX11BaseCore(){};

		/*!-----------------------------------------------------------
		//	@brief		デバイス取得
		//	@return		デバイスポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		SmartComPtr<ID3D11Device> GetDevice()
		{
			return this->pd3dDevice;
		}

	protected:
		SmartComPtr<ID3D11Device> pd3dDevice;
	};
};

#endif // _Include_DirectX11BaseCore_hpp_