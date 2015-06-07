/*************************************************************
*	@file   	LightBloomShader.hpp
*	@brief  	LightBloomShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/09/09
*************************************************************/

#ifndef _Include_LightBloomShader_hpp_	// インクルードガード
#define _Include_LightBloomShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<array>
#include"../RenderBaseShader.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../GaussianFilter/GaussianFilterShader.hpp"

namespace ao
{
	enum class BloomBuffer : int
	{
		Low = 2,
		Normal = 4,
		Hight = 6,
		VeryHigh = 8
	};

	/*!-----------------------------------------------------------
	//	@class  LightBloomShader
	//	@brief  ライトブルームシェーダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/09
	------------------------------------------------------------*/
	class LightBloomShader : public RenderBaseShader
	{
	public:
		/// コンストラクタ
		LightBloomShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		/// デストラクタ
		~LightBloomShader();

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pGraphicsState		グラフィックステート
		//	@param[in]	_pRenderer			レンダラー
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		レンダーターゲットの作成
		//	@param[in]	_nWidth			ブラー横幅
		//	@param[in]	_nHeight		ブラー縦幅
		//  @param[in]  _numBuffer		バッファ数[省略可]
		//	@param[in]	_format			ブラーフォーマット[省略可]
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		HRESULT	Create(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST BloomBuffer _numBuffer = BloomBuffer::Normal,
			CONST DXGI_FORMAT _format = DXGI_FORMAT_B8G8R8A8_UNORM);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_pTexture		HDRテクスチャ
		//  @param[in]  _dispersion		分散率[省略可]
		//  @param[in]  _mulPower		乗算率[省略可]
		//	@return		S_OK：定数バッファMAP成功　それ以外：定数バッファMAP失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseTexture* _pTexture,
			FLOAT _dispersion=0.1f,
			FLOAT _mulPower = 0.5f);

		/*!-----------------------------------------------------------
		//	@brief		プッシュバック開始時の処理
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		void	OnPushbackEnter();

		/*!-----------------------------------------------------------
		//	@brief		描画実行
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		void	OnDraw()override final;

		/*!-----------------------------------------------------------
		//	@brief		レンダーターゲットのクリア
		//  @param[in]  _r	消去赤要素[省略可]
		//  @param[in]  _g	消去緑要素[省略可]
		//  @param[in]  _b	消去青要素[省略可]
		//  @param[in]  _a	消去透過要素[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		void	Clear(float _r = 0, float _g = 0, float _b = 0, float _a = 0);

	private:
		struct PsConst
		{
			int numBuffer;
			float mulPower;
			float uvOffset[2];
		};

		struct COMMAND
		{
			BOOL isActive;
			FLOAT dispersion;
			FLOAT mulPower;
			BaseTexture* upSampleTexture;
			std::array<BaseTexture*,(int)BloomBuffer::VeryHigh> pTextureArray;
		};

		BloomBuffer numBuffer;
		COMMAND renderCommand;
		VertexShader* pUpSampleVertexShader;
		PixelShader* pUpSamplePixelShader;
		VertexShader* pSurfaceVertexShader;
		PixelShader* pSurfacePixelShader;
		ConstBuffer<PsConst> psConst;
		SmartComPtr<ID3D11Buffer> pSurfaceBuffer;
		DirectX11GraphicsState* pGraphicsState;
		DirectX11GraphicsRenderer* pRenderer;
		DirectX11RenderTarget* pUpSampleRenderTarget;
		std::array<BaseTexture*, (int)BloomBuffer::VeryHigh> nullTextures;
		std::array<GaussianFilterShader*, (int)BloomBuffer::VeryHigh> pReductionBuffer;

	private:
		/*!-----------------------------------------------------------
		//	@brief		サーフェイス作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		inline	HRESULT CreateSurface();
	};
}

#endif // _Include_LightBloomShader_hpp_