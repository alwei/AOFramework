/*************************************************************
*	@file   	RenderUiShader.hpp
*	@brief  	RenderUiShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/08
*************************************************************/

#ifndef _Include_RenderUiShader_hpp_	// インクルードガード
#define _Include_RenderUiShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<vector>
#include<mutex>
#include<DirectXMath.h>
#include"../RenderBaseShader.hpp"
#include"../../Resource/Sprite.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"

using namespace DirectX;

namespace  ao
{
	//射影行列の基点種類
	enum class SurfacePivot : int
	{
		LeftTop = 0,
		RightTop,
		LeftButtom,
		RightButtom,
		Center
	};

	/*!-----------------------------------------------------------
	//	@class  RenderUiShader
	//	@brief  UIレンダリングシェーダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/08
	------------------------------------------------------------*/
	class RenderUiShader : public RenderBaseShader
	{
	public:
		RenderUiShader(SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~RenderUiShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//  @param[in]  _pGraphicsState		グラフィックステート
		//  @param[in]  _pRenderer			レンダラー
		//	return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_pSprite		スプライトポインタ
		//	@param[in]	_x				X座標
		//	@param[in]	_y				Y座標
		//	@param[in]	_xScale			Xスケール
		//	@param[in]	_yScale			Yスケール
		//	@param[in]	_radian			ラジアン[省略可]
		//	@param[in]	_r				赤色[省略可]
		//	@param[in]	_g				緑色[省略可]
		//	@param[in]	_b				青色[省略可]
		//	@param[in]	_a				透過率[省略可]
		//  @param[in]  _blendState		ブレンドステート[省略可]
		//  @param[in]  _fillState		描画モード[省略可]
		//	@param[in]	_isCache		キャッシュフラグ[省略可]
		//  @param[in]  _spritePivot	スプライトピボット[省略可]
		//	@param[in]	_surfacePivot	画面の原点種類[省略可]
		//	@param[in]	_priority		レンダリング優先度[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		HRESULT	Render(
			Sprite* const _pSprite,
			const int _x,
			const int _y,
			const float _xScale,
			const float _yScale,
			const float _rotation = 0,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const BlendState _blendState = BlendState::Alpha,
			const FillState _fillState = FillState::Solid,
			const SpritePivot _spritePivot = SpritePivot::Center,
			const SurfacePivot _surfacePivot = SurfacePivot::Center,
			const BOOL _isCache = TRUE,
			const UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_pTexture		テクスチャポインタ
		//	@param[in]	_x				X座標
		//	@param[in]	_y				Y座標
		//	@param[in]	_xScale			Xスケール
		//	@param[in]	_yScale			Yスケール
		//	@param[in]	_radian			ラジアン[省略可]
		//	@param[in]	_r				赤色[省略可]
		//	@param[in]	_g				緑色[省略可]
		//	@param[in]	_b				青色[省略可]
		//	@param[in]	_a				透過率[省略可]
		//  @param[in]  _blendState		ブレンドステート[省略可]
		//	@param[in]	_isCache		キャッシュフラグ[省略可]
		//  @param[in]  _spritePivot	スプライトピボット[省略可]
		//	@param[in]	_surfacePivot	画面の原点種類[省略可]
		//	@param[in]	_priority		レンダリング優先度[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseTexture* const _pTexture,
			const int _x,
			const int _y,
			const float _xScale,
			const float _yScale,
			const float _radian = 0,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const BlendState _blendState = BlendState::Alpha,
			const FillState _fillState = FillState::Solid,
			const SpritePivot _spritePivot = SpritePivot::Center,
			const SurfacePivot _surfacePivot = SurfacePivot::Center,
			const BOOL _isCache = TRUE,
			const UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		テクスチャスロット解放
		//	@param[in]	_priority	優先度[省略可]
		//	@return		S_OK：解放成功　それ以外：解放失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/04
		------------------------------------------------------------*/
		HRESULT	ReleaseTextureSlot(UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		描画コマンド実行
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		void	OnDraw();

	private:
		struct COMMAND
		{
			int x, y;
			float sx, sy;
			float radian;
			float color[4];
			UINT priority;
			BOOL isCache;
			BOOL isCenter;
			SurfacePivot surfacePivot;
			BlendState blendState;
			FillState fillState;
			Sprite* pSprite;
			BaseTexture* pTexture;
		};

		struct VsConst
		{
			XMFLOAT4X4 matWO;
		};

		struct PsConst
		{
			FLOAT color[4];
		};

		BOOL isNeedSort;
		std::vector<COMMAND>	commandList;
		VertexShader*			pVertexShader;
		PixelShader*			pPixelShader;
		ConstBuffer<VsConst>	vsConst;
		ConstBuffer<PsConst>	psConst;
		XMFLOAT4X4				leftTopOrthoMatrix;
		XMFLOAT4X4				centerOrthoMatrix;
		Sprite					defaultLeftTopSprite;
		Sprite					defaultCenterSprite;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
		SmartComPtr<ID3D11SamplerState> pointSampler;

	private:
		/*!-----------------------------------------------------------
		//	@brief		定数バッファマッピング
		//	@param[in]	_command	コマンド
		//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		inline	HRESULT	MapConstBuffer(const COMMAND& _command);

		/*!-----------------------------------------------------------
		//	@brief		デフォルトスプライト用の作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		inline HRESULT	CreateDefaultSprite();

		/*!-----------------------------------------------------------
		//	@brief		ポイントサンプラー作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		inline HRESULT	CreatePointSampler();
	};
}

#endif // _Include_RenderUiShader_hpp_