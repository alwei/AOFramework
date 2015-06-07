/*************************************************************
*	@file   	DirectX11RenderTarget.hpp
*	@brief  	DirectX11RenderTargetクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/06/27
*************************************************************/

#ifndef _Include_DirectX11RenderTarget_hpp_	// インクルードガード
#define _Include_DirectX11RenderTarget_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include"DirectX11Header.hpp"
#include"DirectX11BaseApi.hpp"
#include"../Utility/SmartComPtr.hpp"
#include"../Resource/BaseTexture.hpp"
#include"../Define/RenderingDefine.hpp"
#include"Define/IODefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11RenderTarget
	//	@brief  レンダーターゲット
	//	@author	Tatsunori Aoyama
	//	@date	2014/06/27
	------------------------------------------------------------*/
	class DirectX11RenderTarget : public DirectX11BaseApi
	{
	public:
		DirectX11RenderTarget(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11RenderTarget()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化

		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		HRESULT	Initialize();

		/*!-----------------------------------------------------------
		//	@brief		作成
		//	@param[in]	_nWidth					横解像度
		//	@param[in]  _nHeight				縦解像度
		//	@param[in]  _renderFormat			フォーマット[省略可]
		//	@param[in]  _miscFlag				リソース使用に関するオプション[省略可]
		//	@param[in]  _mipLevel				ミップレベル[省略可]
		//	@param[in]  _isCreateDepthStencil	深度ステンシルの作成有無[省略可]
		//	@param[in]  _depthFormat			深度フォーマット[省略可]
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		HRESULT	Create(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST DXGI_FORMAT _format = DXGI_FORMAT_UNKNOWN,
			CONST UINT _miscFlag = FALSE,
			CONST UINT _mipLevel = 1,
			CONST BOOL _isCreateDepthStencil = FALSE,
			CONST DXGI_FORMAT _depthFormat = DXGI_FORMAT_R32_TYPELESS);

		/*!-----------------------------------------------------------
		//	@brief		レンダーターゲットの設定
		//	@param[in]  _isLogRestore		リストア情報を保持するかどうか
		//	@param[in]  _isSetDepthStencil	深度ステンシルの設定有無
		//	@param[in]	_pd3dDeviceContext	使用するデバイスコンテキスト[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/23
		------------------------------------------------------------*/
		void	SetRenderTarget(
			BOOL _isLogRestore = FALSE,
			BOOL _isSetDepthStencil = TRUE,
			ID3D11DeviceContext* _pd3dDeviceContext=nullptr);

		/*!-----------------------------------------------------------
		//	@brief		レンダーターゲットのリストア
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/23
		------------------------------------------------------------*/
		void	RestoreRenderTarget();

		/*!-----------------------------------------------------------
		//	@brief		クリア
		//	@param[in]	_r	クリア赤色[省略可]
		//	@param[in]	_g	クリア緑色[省略可]
		//	@param[in]	_b	クリア青色[省略可]
		//	@param[in]	_a	クリア透過率[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/11
		------------------------------------------------------------*/
		void	Clear(float _r = 0, float _g = 0, float _b = 0, float _a = 0);

		/*!-----------------------------------------------------------
		//	@brief		画像ファイルとして外部に出力
		//	@param[in]	_filePath	ファイルパス
		//	@param[in]	_format		出力フォーマット[省略可]
		//	@return		S_OK：出力成功　それ以外：出力失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/24
		------------------------------------------------------------*/
		HRESULT OutputRenderTarget(AOLPCSTR _filePath,const ImageFormat _format=ImageFormat::PNG);

		/*!-----------------------------------------------------------
		//	@brief		ビューポート取得
		//	@param[out]	_pOut	出力されるビューポート
		//  @return     出力されるビューポート
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		const D3D11_VIEWPORT* GetViewport(D3D11_VIEWPORT* const _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		レンダリングテクスチャの取得
		//	@param[in]	_sampler	指定するサンプラー
		//  @return     レンダリングテクスチャ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		BaseTexture* GetRenderTexture(SamplerState _sampler=SamplerState::LINEAR)
		{
			this->rtTexture.pSampState = this->renderSamplerState[(int)_sampler];
			return &this->rtTexture;
		}

		/*!-----------------------------------------------------------
		//	@brief		深度テクスチャの取得
		//	@param[in]	_sampler	指定するサンプラー
		//  @return     深度テクスチャ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		BaseTexture* GetDepthTexture(SamplerState _sampler=SamplerState::LINEAR)
		{
			this->dsTexture.pSampState = this->renderSamplerState[(int)_sampler];
			return &this->dsTexture;
		}

	public:
		SmartComPtr<ID3D11Texture2D>			pRtTex;
		SmartComPtr<ID3D11RenderTargetView>		pRtView;
		SmartComPtr<ID3D11ShaderResourceView>	pRtSrView;
		SmartComPtr<ID3D11Texture2D>			pDsTex;
		SmartComPtr<ID3D11DepthStencilView>		pDsView;
		SmartComPtr<ID3D11ShaderResourceView>	pDsSrView;
		SmartComPtr<ID3D11RenderTargetView>		pLogRtView;
		SmartComPtr<ID3D11DepthStencilView>		pLogDsView;
		ID3D11DeviceContext* pLogDeviceContext;
		D3D11_VIEWPORT viewport;
		D3D11_VIEWPORT logViewport;
		BaseTexture rtTexture;
		BaseTexture dsTexture;
		SmartComPtr<ID3D11SamplerState> renderSamplerState[(int)SamplerState::MAX];

	private:
		/*!-----------------------------------------------------------
		//	@brief		レンダーターゲットビュー作成
		//	@param[in]	_nWidth		レンダーターゲットの幅
		//	@param[in]	_nHeight	レンダーターゲットの高さ
		//	@param[in]	_format		レンダーターゲットのフォーマット[省略可]
		//	@param[in]	_miscFlag	リソースに使用するオプション[省略可]
		//	@param[in]	_mipLevel	ミップレベル[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		inline	HRESULT	CreateRenderTargetView(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
			CONST UINT miscFlag = FALSE,
			CONST UINT mipLevel = 1);

		/*!-----------------------------------------------------------
		//	@brief		深度ステンシルビュー作成
		//	@param[in]	_nWidth		深度ステンシルの幅
		//	@param[in]	_nHeight	深度ステンシルの高さ
		//	@param[in]	_format		フォーマット[省略可]
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		inline	HRESULT	CreateDepthStencilView(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST DXGI_FORMAT _format = DXGI_FORMAT_R8G8B8A8_UNORM);

		/*!-----------------------------------------------------------
		//	@brief		サンプラーステート作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		inline	HRESULT	CreateDefaultSamplerState();
	};
}

#endif // _Include_DirectX11RenderTarget_hpp_
