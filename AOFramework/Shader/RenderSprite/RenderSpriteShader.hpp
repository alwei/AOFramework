/*************************************************************
*	@file   	RenderSpriteShader.hpp
*	@brief  	RenderSpriteShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/10/02
*************************************************************/

#ifndef _Include_RenderSpriteShader_hpp_	// インクルードガード
#define _Include_RenderSpriteShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<vector>
#include<mutex>
#include<DirectXMath.h>
#include"../RenderBaseShader.hpp"
#include"../../Math/Math.hpp"
#include"../../Math/GameArithmetic.hpp"
#include"../../Resource/Sprite.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"

using namespace DirectX;

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  RenderSpriteShader
	//	@brief  メッシュレンダリングシェーダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/02
	------------------------------------------------------------*/
	class RenderSpriteShader : public RenderBaseShader
	{
	public:
		RenderSpriteShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~RenderSpriteShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pGraphicsState		グラフィックステート
		//  @param[in]  _pRenderer			レンダラー
		//	return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/02
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		ビュー行列更新
		//	@param[in]	_pViewMtx	ビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/02
		------------------------------------------------------------*/
		void UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_pSprite			スプライトポインタ
		//	@param[in]	_transform			トランスフォーム
		//	@param[in]	_r					赤色[省略可]
		//	@param[in]	_g					緑色[省略可]
		//	@param[in]	_b					青色[省略可]
		//	@param[in]	_a					透過率[省略可]
		//  @param[in]  _blendState			ブレンドステート[省略可]
		//	@param[in]	_isCulling			カリングフラグ[省略可]
		//	@param[in]	_fillMode			描画モード[省略可]
		//	@param[in]	_priority			レンダリング優先度[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/02
		------------------------------------------------------------*/
		HRESULT	Render(
			Sprite* const _pSprite,
			Transform& _transform,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const BlendState _blendState = BlendState::Alpha,
			const BOOL _isCullling = TRUE,
			const FillState _fillMode = FillState::Solid,
			const UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_pTexture			テクスチャポインタ
		//	@param[in]	_transform			トランスフォーム
		//	@param[in]	_r					赤色[省略可]
		//	@param[in]	_g					緑色[省略可]
		//	@param[in]	_b					青色[省略可]
		//	@param[in]	_a					透過率[省略可]
		//  @param[in]  _blendState			ブレンドステート[省略可]
		//	@param[in]	_isCulling			カリングフラグ[省略可]
		//	@param[in]	_fillMode			描画モード[省略可]
		//	@param[in]	_priority			レンダリング優先度[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/03
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseTexture* const _pTexture,
			Transform& _transform,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const BlendState _blendState = BlendState::Alpha,
			const BOOL _isCullling = TRUE,
			const FillState _fillMode = FillState::Solid,
			const UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		描画コマンド実行
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/02
		------------------------------------------------------------*/
		void	OnDraw()override final;

	private:
		struct COMMAND
		{
			XMFLOAT4X4 matrix;
			BlendState blendState;
			D3D11_FILL_MODE fillMode;
			D3D11_CULL_MODE cullMode;
			UINT priority;
			FLOAT color[4];
			Sprite* pSprite;
		};

		struct VsConst
		{
			XMFLOAT4X4 matWVP;
		};

		struct PsConst0
		{
			float color[4];
		};

		BOOL isNeedSort;
		Sprite defaultLeftTopSprite;
		Sprite defaultCenterSprite;
		std::vector<COMMAND>	commandList;
		VertexShader*			pVertexShader;
		PixelShader*			pPixelShader;
		ConstBuffer<VsConst>	vsConst;
		ConstBuffer<PsConst0>	psConst0;
		XMFLOAT4X4 projMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 viewProjMatrix;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;

	private:
		/*!-----------------------------------------------------------
		//	@brief		行列定数バッファマッピング
		//	@param[in]	_mtx				行列
		//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/02
		------------------------------------------------------------*/
		inline	HRESULT	MapMatrixConstBuffer(const XMFLOAT4X4& _mtx);
	};
}

#endif // _Include_RenderSpriteShader_hpp_