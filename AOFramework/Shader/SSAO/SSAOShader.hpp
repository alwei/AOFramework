/*************************************************************
*	@file   	SSAOShader.hpp
*	@brief  	SSAOShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/01/19
*************************************************************/

#ifndef _Include_SSAOShader_hpp_	// インクルードガード
#define _Include_SSAOShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../RenderBaseShader.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Resource/ShaderResource.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  SSAOShader
	//	@brief  SSAO(不完全）
	//	@author	Tatsunori Aoyama
	//	@date	2015/01/19
	------------------------------------------------------------*/
	class SSAOShader : public RenderBaseShader
	{
	public:
		SSAOShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~SSAOShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pGraphicsState		グラフィックステート
		//	@param[in]	_pRenderer			レンダラー
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
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
		//	@date		2015/01/19
		------------------------------------------------------------*/
		HRESULT	Create(DirectX11RenderTarget* const _pRenderTarget);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_pTexture		テクスチャ
		//	@return		S_OK：登録成功　それ以外：登録失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		HRESULT	Render(BaseTexture* _pTexture);

		/*!-----------------------------------------------------------
		//	@brief		描画実行
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
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
		//	@date		2015/01/19
		------------------------------------------------------------*/
		HRESULT CreateSurface();
	};
}

#endif // _Include_SSAOShader_hpp_