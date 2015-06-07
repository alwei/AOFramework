/*************************************************************
*	@file   	DirectX11GraphicsState.hpp
*	@brief  	DirectX11GraphicsStateクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/17
*************************************************************/

#ifndef _Include_DirectX11GraphicsState_hpp_	// インクルードガード
#define _Include_DirectX11GraphicsState_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<map>
#include"DirectX11BaseApi.hpp"

namespace ao
{
	enum class BlendState : int
	{
		Invalid = -1,
		Default = 0,
		Alpha,
		Add,
		Sub,
		Mul,
		Max
	};

	enum class FillState : int
	{
		Invalid = -1,
		Solid = D3D11_FILL_SOLID,
		WireFrame = D3D11_FILL_WIREFRAME
	};

	/*!-----------------------------------------------------------
	//	@class  DirectX11GraphicsState
	//	@brief  グラフィックステート管理クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/17
	------------------------------------------------------------*/
	class DirectX11GraphicsState : public DirectX11BaseApi
	{
	public:
		DirectX11GraphicsState(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11GraphicsState()override;

		/*!-----------------------------------------------------------
		//	@brief		ブレンドステート設定
		//	@param[in]	_blendState				ブレンドする設定配列情報
		//	@param[in]	_numBlendState			設定するレンダーターゲットの数[省略可]
		//	@param[in]	_alphaToCoverageEnable	アルファカヴァレッジの有効化[省略可]
		//	@param[in]	_isForceBind			強制的な設定有無[省略可]
		//	@return		S_OK：設定成功　それ以外：設定失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/17
		------------------------------------------------------------*/
		HRESULT	SetBlendState(
			const BlendState _blendState,
			const UINT _numBlendState = 1,
			const BOOL _alphaToCoverageEnable = FALSE,
			const BOOL _isForceBind = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		深度ステンシルステートを設定する
		//	@param[in]	_isDepthEnable		深度の有効無効
		//	@param[in]	_depthWriteMask		深度の書き込み設定[省略可]
		//	@param[in]	_depthFunc			深度データの比較設定[省略可]
		//	@param[in]	_isStencilEnable	スンテンシルの有効無効[省略可]
		//	@param[in]	_isForceBind		強制的な設定有無[省略可]
		//	@return		S_OK：設定成功　それ以外：設定失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/17
		------------------------------------------------------------*/
		HRESULT	SetDepthStecilState(
			const BOOL _isEnableDepth,
			const D3D11_DEPTH_WRITE_MASK _depthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
			const D3D11_COMPARISON_FUNC _depthFunc = D3D11_COMPARISON_LESS_EQUAL,
			const BOOL _isEnableStencil = FALSE,
			const BOOL _isForceBind = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		ラスタライズステートを設定する
		//	@param[in]	_fillMode				描画モード[省略可]
		//	@param[in]	_cullMode				カリングモード[省略可]
		//	@param[in]	_isForceBind			強制バインドフラグ[省略可]
		//	@param[in]	_frontCCW				カリング方向[省略可]
		//	@param[in]	_depthBias				指定のピクセルに加算する深度値[省略可]
		//	@param[in]	_depthBiasClamp			ピクセルの最大深度バイアス[省略可]
		//	@param[in]	_slopeScaleDepthBias	指定のピクセルの傾きに対するスカラ値[省略可]
		//	@param[in]	_depthClipEnable		距離に基づいてクリッピングを有効[省略可]
		//	@param[in]	_scissorEnable			シザー矩形カリングの有効[省略可]
		//	@param[in]	_multisampleEnable		マルチサンプリングのエイリアシングの有効[省略可]
		//	@param[in]	_antialiasedLineEnable	線のアンチエイリアシングの有効[省略可]
		//	@return		S_OK：設定成功　それ以外：設定失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/03
		------------------------------------------------------------*/
		HRESULT SetRasterizerState(
			D3D11_FILL_MODE _fillMode = D3D11_FILL_SOLID,
			D3D11_CULL_MODE _cullMode = D3D11_CULL_BACK,
			BOOL _isForceBind = FALSE,
			BOOL _frontCCW = FALSE,
			INT _depthBias = 0,
			FLOAT _depthBiasClamp = 0,
			FLOAT _slopeScaleDepthBias = 0,
			BOOL _depthClipEnable = TRUE,
			BOOL _scissorEnable = FALSE,
			BOOL _multisampleEnable = FALSE,
			BOOL _antialiasedLineEnable = FALSE);

	private:
		struct NowBlendState{
			BlendState blend;
			BOOL enableAlphaToCoverage;
		};
		struct NowDepthStencilState{
			BOOL depthEnable;
			BOOL stencilEnable;
			D3D11_DEPTH_WRITE_MASK mask;
			D3D11_COMPARISON_FUNC func;
		};
		struct NowRasterizerState{
			D3D11_FILL_MODE fillMode;
			D3D11_CULL_MODE cullMode;
			BOOL frontCounterClockwise;
			INT depthBias;
			INT depthBiasClamp;
			INT slopeScaledDepthBias;
			BOOL depthClipEnable;
			BOOL scissorEnable;
			BOOL multisampleEnable;
			BOOL antialiasedLineEnable;
		};

		NowBlendState nowBlendState;
		NowDepthStencilState nowDepthStencilState;
		NowRasterizerState nowRasterizerState;

		typedef std::map<USHORT, ID3D11BlendState*> blend_map;
		typedef std::map<USHORT, ID3D11DepthStencilState*> depthStencil_map;
		typedef std::map<DWORD, ID3D11RasterizerState*> rasterizer_map;
		blend_map	pBlendStateMap;
		depthStencil_map pDepthStencilMap;
		rasterizer_map pRasterizerMap;

	private:
		/*!-----------------------------------------------------------
		//	@brief		ブレンドステートの作成
		//	@param[in]	_blendState				作成するブレンドステート
		//	@param[in]	_numBlendState			作成する際に、設定するブレンドステートの数[省略可]
		//	@param[in]	_alphaToCoverageEnable	αカバレッジの有効無効[省略可]
		//	@return		ブレンドステートポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/17
		------------------------------------------------------------*/
		inline	ID3D11BlendState*	CreateBlendState(
			const BlendState _blendState,
			const UINT _numBlendState = 1,
			const BOOL _alphaToCocerageEnable = FALSE);


		/*!-----------------------------------------------------------
		//	@brief		深度ステンシルの作成
		//	@param[in]	_isDepthEnable		深度の有効無効
		//	@param[in]	_depthWriteMask		深度の書き込み設定[省略可]
		//	@param[in]	_depthFunc			深度データの比較設定[省略可]
		//	@param[in]	_isStencilEnable	スンテンシルの有効無効[省略可]
		//	@return		深度ステンシルステートポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/17
		------------------------------------------------------------*/
		inline	ID3D11DepthStencilState* CreateDepthStencilState(
			const BOOL _isEnableDepth,
			const D3D11_DEPTH_WRITE_MASK _depthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
			const D3D11_COMPARISON_FUNC _depthFunc = D3D11_COMPARISON_LESS_EQUAL,
			const BOOL _isEnableStencil = FALSE);


		/*!-----------------------------------------------------------
		//	@brief		ラスタライズステートの作成
		//	@param[in]	_fillMode				描画モード
		//	@param[in]	_cullMode				カリングモード
		//	@param[in]	_frontCCW				カリング方向
		//	@param[in]	_depthBias				指定のピクセルに加算する深度値
		//	@param[in]	_depthBiasClamp			ピクセルの最大深度バイアス
		//	@param[in]	_slopeScaleDepthBias	指定のピクセルの傾きに対するスカラ値
		//	@param[in]	_depthClipEnable		距離に基づいてクリッピングを有効
		//	@param[in]	_scissorEnable			シザー矩形カリングの有効
		//	@param[in]	_multisampleEnable		マルチサンプリングのエイリアシングの有効
		//	@param[in]	_antialiasedLineEnable	線のアンチエイリアシングの有効
		//	@return		ラスタライズステートポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/03
		------------------------------------------------------------*/
		inline	ID3D11RasterizerState*	CreateRasterizerState(
			D3D11_FILL_MODE _fillMode,
			D3D11_CULL_MODE _cullMode,
			BOOL _frontCCW,
			INT _depthBias,
			FLOAT _depthBiasClamp,
			FLOAT _slopeScaleDepthBias,
			BOOL _depthClipEnable,
			BOOL _scissorEnable,
			BOOL _multisampleEnable,
			BOOL _antialiasedLineEnable);
	};
}

#endif // _Include_DirectX11GraphicsState_hpp_