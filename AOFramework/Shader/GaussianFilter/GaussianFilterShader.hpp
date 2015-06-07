/*************************************************************
*	@file   	GaussianFilterShader.hpp
*	@brief  	GaussianFilterShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2013/09/04
*************************************************************/

#ifndef _Include_GaussianFilterShader_hpp_	// インクルードガード
#define _Include_GaussianFilterShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../RenderBaseShader.hpp"
#include"../../Utility/SmartComPtr.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Api/DirectX11Shader.hpp"
#include"../../Api/DirectX11RenderTarget.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  GaussianFilterShader
	//	@brief  ガウスフィルターシェーダークラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/08
	------------------------------------------------------------*/
	class GaussianFilterShader : public RenderBaseShader
	{
	public:
		/// コンストラクタ
		GaussianFilterShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		/// デストラクタ
		~GaussianFilterShader();

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pGraphicsState		グラフィックステート
		//	@param[in]	_pRenderer			レンダラー
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		レンダーターゲットの作成
		//	@param[in]	_nBlurWidth		ブラー横幅
		//	@param[in]	_nBlurHeight	ブラー縦幅
		//	@param[in]	_blurFormat		ブラーフォーマット[省略可]
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Create(
			CONST UINT _nBlurWidth,
			CONST UINT _nBlurHeight,
			CONST DXGI_FORMAT _blurFormat = DXGI_FORMAT_B8G8R8A8_UNORM);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_pTexture		HDRテクスチャ
		//  @param[in]  _dispersion		分散率[省略可]
		//  @param[in]  _mulPower		乗算率[省略可]
		//	@return		S_OK：定数バッファMAP成功　それ以外：定数バッファMAP失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseTexture* _pTexture,
			FLOAT _dispersion = 0.1f,
			FLOAT _mulPower = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		描画実行
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		void	OnDraw();

		/*!-----------------------------------------------------------
		//	@brief		ブラー済みテクスチャ取得
		//	@param[in]	_sampler	サンプラー
		//  @return     テクスチャ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		BaseTexture* GetBlurTexture(SamplerState _sampler = SamplerState::LINEAR)
		{
			return this->pBlurRenderTarget[1]->GetRenderTexture(_sampler);
		}

		/*!-----------------------------------------------------------
		//	@brief		レンダーターゲットのクリア
		//  @param[in]  _r	消去赤要素[省略可]
		//  @param[in]  _g	消去緑要素[省略可]
		//  @param[in]  _b	消去青要素[省略可]
		//  @param[in]  _a	消去透過要素[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		void	Clear(float _r=0, float _g=0,float _b=0,float _a=0);

	private:
		struct VsConst
		{
			float	width;
			float	height;
			float	power[2];
		};

		struct PsConst
		{
			float	weight[8];
			float	offset[2];
			float	power[2];
		};

		struct COMMAND
		{
			BOOL isActive;
			FLOAT dispersion;
			FLOAT mulPower;
			ID3D11ShaderResourceView** ppSrView;
			ID3D11SamplerState** ppSampState;
		};

	private:
		COMMAND renderCommand;
		ConstBuffer<VsConst> vsConst;
		ConstBuffer<PsConst> psConst;
		VertexShader*	pXBlurVertexShader;
		PixelShader*	pXBlurPixelShader;
		VertexShader*	pYBlurVertexShader;
		PixelShader*	pYBlurPixelShader;
		SmartComPtr<ID3D11Buffer>	pSurfaceBuffer;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
		DirectX11RenderTarget*		pBlurRenderTarget[2];

	private:
		/*!-----------------------------------------------------------
		//	@brief		サーフェイス作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		inline	HRESULT CreateSurface();

		/*!-----------------------------------------------------------
		//	@brief		定数バッファの更新マップ
		//	@param[in]	_dispersion	分散率
		//	@param[in]	_mulPower	乗算強度[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		inline	void	UpdateMap(float _dispersion, float _mulPower = 1.0f);
	};
}

#endif // _Include_GaussianFilterShader_hpp_
