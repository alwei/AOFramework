/*************************************************************
*	@file   	DirectX11Application.hpp
*	@brief  	DirectX11Applicationクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/06/27
*************************************************************/

#ifndef _Include_DirectX11Application_hpp_	// インクルードガード
#define _Include_DirectX11Application_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"DirectX11Header.hpp"
#include"DirectX11BaseApi.hpp"
#include"../Utility/SmartComPtr.hpp"

namespace ao
{
	class DirectX11Adapter;
	class DirectX11EnumDisplay;
	class DirectX11RenderTarget;
	class DirectX11Viewport;

	/*!-----------------------------------------------------------
	//	@class  DirectX11Application
	//	@brief  11専用アプリケーションクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/06/26
	------------------------------------------------------------*/
	class DirectX11Application
	{
	public:
		DirectX11Application();

		~DirectX11Application();

		/*!-----------------------------------------------------------
		//	@brief		ディスプレイの初期化
		//	@param[in]  _backBufferFormat	バックバッファフォーマット[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		HRESULT	InitDisplay( CONST DXGI_FORMAT _backBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT );

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_hWnd				ウィンドウハンドル
		//	@param[in]  _nWidth				解像度の幅
		//	@param[in]  _nHeight			解像度の高さ
		//	@param[in]  _renderFormat		レンダリングフォーマット
		//	@param[in]  _isWindowMode		ウィンドウモード[省略可]
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		HRESULT	InitDirect3D(
			CONST HWND _hWnd,
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST DXGI_FORMAT _renderFormat = DXGI_FORMAT_R16G16B16A16_FLOAT,
			CONST BOOL _isWindowMode = TRUE);

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウモードの変更
		//	@param[in]	_isResize	バックバッファの解像度変更フラグ[省略可]
		//	@return		S_OK：変更成功　それ以外：変更失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/23
		------------------------------------------------------------*/
		HRESULT	ChangeWindowMode(CONST BOOL _isResize = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		バックバッファ取得
		//	@return		バックバッファポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/11
		------------------------------------------------------------*/
		DirectX11RenderTarget* GetBackBuffer()
		{
			return this->pBackBuffer;
		}

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

		/*!-----------------------------------------------------------
		//	@brief		スワップチェイン取得
		//	@return		スワップチェイン
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/11
		------------------------------------------------------------*/
		SmartComPtr<IDXGISwapChain> GetSwapChain()
		{
			return this->swapChain;
		}

	private:
		bool	bInitDisplay;		///< ディスプレイの初期化フラグ
		bool	isWindowMode;		///< ウィンドウモード
		HWND	hWnd;				///< ウィンドウハンドル
		DXGI_FORMAT							useFormat;			///< 使用するフォーマット
		DirectX11Adapter*					pAdapter;			///< アダプター
		DirectX11EnumDisplay*				pEnumDisplay;		///< ディスプレイ列挙
		DirectX11RenderTarget*				pBackBuffer;		///< バックバッファ
		DirectX11Viewport*					pViewport;			///< ビューポート
		SmartComPtr<IDXGISwapChain>			swapChain;			///< スワップチェーン
		SmartComPtr<ID3D11Device>			pd3dDevice;			///< デバイスポインタ
		SmartComPtr<ID3D11DeviceContext>	pd3dDeviceContext;	///< デバイスコンテキストポインタ

	private:
		/*!-----------------------------------------------------------
		//	@brief		デバイス作成
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	CreateDevice();

		/*!-----------------------------------------------------------
		//	@brief		スワップチェイン作成
		//	@param[in]  _pDisplayMode			ディスプレイ情報
		//	@param[in]  _isMultiSampleEnable	マルチサンプルの有効有無
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	CreateSwapChain(
			CONST DXGI_MODE_DESC* _pDisplayMode,
			const bool _isMultiSampleEnable = false);

		/*!-----------------------------------------------------------
		//	@brief		レンダーターゲット作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	CreateRenderTargetView();

		/*!-----------------------------------------------------------
		//	@brief		深度ステンシル作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	CreateDepthStencilView();

		/*!-----------------------------------------------------------
		//	@brief		ビューポート作成
		//  @param[in]  _nWidth		横幅
		//  @param[in]  _nHeight	高さ
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	CreateViewport(UINT _nWidth,UINT _nHeight);

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウの状態を設定
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	SetWindowAssociation();

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウモード変更の最適化
		//	param[in]	_nWidth		解像度の横幅[省略可]
		//	param[in]	_nHeight	解像度の縦幅[省略可]
		//	@return		S_OK：最適化成功　それ以外：最適化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/23
		------------------------------------------------------------*/
		inline	HRESULT	ChangeWindowModeOptimization(CONST UINT _nWidth, CONST UINT _nHeight);

		/*!-----------------------------------------------------------
		//	@brief		デフォルトのラスタライザを設定
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	SetDefaultRasterize();

		/*!-----------------------------------------------------------
		//	@brief		デフォルトの深度ステンシルステートを設定
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	SetDefaultDepthStencilState();
	};
}

#endif	// _Include_DirectX11Application_hpp_