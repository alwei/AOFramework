/*************************************************************
*	@file   	CrossFilterShader.hpp
*	@brief  	CrossFilterShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/09/15
*************************************************************/

#ifndef _Include_CrossFilterShader_hpp_	// インクルードガード
#define _Include_CrossFilterShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<array>
#include"../RenderBaseShader.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Math/MathDefine.hpp"

namespace ao
{
	enum class CrossType : int
	{
		Dual = 2,
		Quad = 4,
		Hex = 6,
		Oct = 8
	};

	enum class CrossQuality : int
	{
		Low = 2,
		Normal = 4,
		Hight = 6,
		VeryHigh = 8
	};

	/*!-----------------------------------------------------------
	//	@class  CrossFilterShader
	//	@brief  クロスフィルターシェーダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/15
	------------------------------------------------------------*/
	class CrossFilterShader : public RenderBaseShader
	{
	public:
		CrossFilterShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~CrossFilterShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pd3dDevice			デバイスポインタ
		//	@param[in]	_pd3dDeviceContext	デバイスコンテキストポインタ
		//	@param[in]	_pGraphicsState		グラフィックステート
		//	@param[in]	_pRenderer			レンダラー
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		レンダーターゲットの作成
		//	@param[in]	_nWidth			ブラー横幅
		//	@param[in]	_nHeight		ブラー縦幅
		//  @param[in]  _numStar		光芒の種類[省略可]
		//	@param[in]	_numStretch		レンダリング数[省略可]
		//	@param[in]	_format			ブラーフォーマット[省略可]
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		HRESULT	Create(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST CrossType _crossType = CrossType::Quad,
			CONST CrossQuality _numStretch = CrossQuality::Normal,
			CONST DXGI_FORMAT _format = DXGI_FORMAT_B8G8R8A8_UNORM);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_pTexture	HDRテクスチャ
		//	@param[in]	_degree		回転角度[省略可]
		//	@param[in]	_mulPower	乗算強度[省略可]
		//	@param[in]	_length		長さ[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseTexture* _pTexture,
			int _degree = 0,
			float _mulPower=0.5f,
			float _length=1.0f);

		/*!-----------------------------------------------------------
		//	@brief		描画メソッド
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		void	OnDraw();

		/*!-----------------------------------------------------------
		//	@brief		レンダーターゲットのクリア
		//  @param[in]  _r	消去赤要素[省略可]
		//  @param[in]  _g	消去緑要素[省略可]
		//  @param[in]  _b	消去青要素[省略可]
		//  @param[in]  _a	消去透過要素[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		void	Clear(float _r = 0, float _g = 0, float _b = 0, float _a = 0);

	private:
		static const int NUM_SAMPLING = 8;

		struct VsConst
		{
			float2 uvTexel[NUM_SAMPLING];
		};

		struct PsConst
		{
			float4 colorWeight[NUM_SAMPLING];
		};

		struct PsUpSampleConst
		{
			int numStar;
			float mulPower;
			float uvOffset[2];
		};

		struct GradationColor
		{
			float4 c[NUM_SAMPLING];
		};

		struct COMMAND
		{
			BOOL isActive;
			FLOAT mulPower;
			float4 colorWeight[(int)CrossQuality::VeryHigh][NUM_SAMPLING];
			float2 uvTexel[(int)CrossType::Oct][(int)CrossQuality::VeryHigh][NUM_SAMPLING];
			std::array<BaseTexture*, (int)CrossQuality::VeryHigh+1> pTextureArray;
			std::array<BaseTexture*, (int)CrossType::Oct> pGlareTextureArray;
		};

		float2 sourceSize;
		float2 downSize;
		std::array<float2,(int)CrossQuality::VeryHigh> starAngle;

		CrossType crossType;
		CrossQuality numStretch;
		COMMAND renderCommand;

		VertexShader* pGlareVertexShader;
		PixelShader* pGlarePixelShader;
		VertexShader* pUpSampleVertexShader;
		PixelShader* pUpSamplePixelShader;
		VertexShader* pSurfaceVertexShader;
		PixelShader* pSurfacePixelShader;

		ConstBuffer<VsConst> vsConst;
		ConstBuffer<PsConst> psConst;
		ConstBuffer<PsUpSampleConst> psUpSampleConst;

		SmartComPtr<ID3D11Buffer> pSurfaceBuffer;
		DirectX11GraphicsState* pGraphicsState;
		DirectX11GraphicsRenderer* pRenderer;
		DirectX11RenderTarget* pUpSampleRenderTarget;

		std::array<BaseTexture*, (int)CrossType::Oct> nullTextures;
		std::array<GradationColor, (int)CrossQuality::VeryHigh> gradationColor;
		std::array<DirectX11RenderTarget*, (int)CrossQuality::VeryHigh-1> pStretchBuffer;
		std::array<DirectX11RenderTarget*, (int)CrossType::Oct> pReductionBuffer;

	private:
		/*!-----------------------------------------------------------
		//	@brief		光芒の色階調を初期化
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		inline	void CreateGradationColor();

		/*!-----------------------------------------------------------
		//	@brief		色の重み定数作成
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		inline	void CreateColorWeight();

		/*!-----------------------------------------------------------
		//	@brief		サーフェイス作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		inline	HRESULT CreateSurface();
	};
}

#endif // _Include_CrossFilterShader_hpp_