/*************************************************************
*	@file   	FXAAShader.hpp
*	@brief  	FXAAShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/11/30
*************************************************************/

#ifndef _Include_FXAAShader_hpp_	// インクルードガード
#define _Include_FXAAShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../RenderBaseShader.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Resource/ShaderResource.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  FXAAShader
	//	@brief  FXAA
	//	@author	Tatsunori Aoyama
	//	@date	2014/11/30
	------------------------------------------------------------*/
	class FXAAShader : public ao::RenderBaseShader
	{
	public:
		FXAAShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~FXAAShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pGraphicsState		グラフィックステート
		//	@param[in]	_pRenderer			レンダラー
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/30
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		レンダーターゲットの作成
		//	@param[in]	_pRenderTarget	AAレンダリング先
		//	@param[in]	_format			ブラーフォーマット[省略可]
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/30
		------------------------------------------------------------*/
		HRESULT	Create(DirectX11RenderTarget* const _pRenderTarget);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_pTexture		テクスチャ
		//	@return		S_OK：登録成功　それ以外：登録失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/30
		------------------------------------------------------------*/
		HRESULT	Render(BaseTexture* _pTexture);

		/*!-----------------------------------------------------------
		//	@brief		プッシュバック開始時の処理
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/30
		------------------------------------------------------------*/
		void	OnPushbackEnter();

		/*!-----------------------------------------------------------
		//	@brief		描画実行
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/30
		------------------------------------------------------------*/
		void	OnDraw()override final;

	private:
		struct COMMAND
		{
			BaseTexture* renderTexture;
			DirectX11RenderTarget* pRenderTarget;
		};

	private:
		COMMAND renderCommand;
		SmartComPtr<ID3D11Buffer> pSurfaceBuffer;
		DirectX11GraphicsState* pGraphicsState;
		DirectX11GraphicsRenderer* pRenderer;
		VertexShader* pVertexShader;
		PixelShader* pPixelShader;

	private:
		/*!-----------------------------------------------------------
		//	@brief		サーフェイス作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/30
		------------------------------------------------------------*/
		HRESULT CreateSurface();
	};
}

#endif // _Include_FXAAShader_hpp_