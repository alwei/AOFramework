/*************************************************************
*	@file   	DirectX11Viewport.hpp
*	@brief  	DirectX11Viewportクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/07/16
*************************************************************/

#ifndef _Include_DirectX11Viewport_hpp_	// インクルードガード
#define _Include_DirectX11Viewport_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"DirectX11Header.hpp"
#include"DirectX11BaseApi.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11Viewport
	//	@brief  ビューポートクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/16
	------------------------------------------------------------*/
	class DirectX11Viewport : public DirectX11BaseApi
	{
	public:
		DirectX11Viewport(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11Viewport()override final;

		/*!-----------------------------------------------------------
		//	@brief		作成
		//	@param[in]	_nWidth		横幅
		//	@param[in]  _nHeight	縦幅
		//	@param[in]	_topLeftX	X軸の原点[省略可]
		//	@param[in]	_topLeftY	Y軸の原点[省略可]
		//	@param[in]	_minDepth	最小深度[省略可]
		//	@param[in]	_maxDeppth	最大深度[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		void	Create(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST INT _topLeftX = 0,
			CONST INT _topLeftY = 0,
			CONST FLOAT _minDpeth = 0.0f,
			CONST FLOAT _maxDpeth = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		設定
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		void	SetViewport()
		{
			this->pd3dDeviceContext->RSSetViewports(1, &this->viewport);
		}

	public:
		D3D11_VIEWPORT	viewport;
	};
}

#endif // _Include_DirectX11Viewport_hpp_
