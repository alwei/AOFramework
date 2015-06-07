/*************************************************************
*	@file   	ExtractBrightnessShader.hpp
*	@brief  	ExtractBrightnessShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/09/08
*************************************************************/

#ifndef _Include_ExtractBrightnessShader_hpp_	// インクルードガード
#define _Include_ExtractBrightnessShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<vector>
#include<mutex>
#include"../RenderBaseShader.hpp"
#include"../../Api/DirectX11RenderTarget.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/ConstBuffer.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  ExtractBrightnessShader
	//	@brief  輝度抽出シェーダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/08
	------------------------------------------------------------*/
	class ExtractBrightnessShader : public RenderBaseShader
	{
	public:
		/// コンストラクタ
		ExtractBrightnessShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		/// デストラクタ
		~ExtractBrightnessShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//  @param[in]  _pGraphicsState		グラフィックステート
		//  @param[in]  _pRenderer			レンダラー
		//  @return     S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		抽出用のサーフェイス作成
		//	@param[in]	_nWidth		横幅
		//	@param[in]	_nHeight	縦幅
		//	@param[in]	_format	フォーマット
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Create(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST DXGI_FORMAT _format = DXGI_FORMAT_R16G16B16A16_FLOAT);

		/*!-----------------------------------------------------------
		//	@brief		抽出処理
		//	@note		シェーダーリソースビュー指定
		//	@param[in]	_pTexture	抽出元のテクスチャ
		//	@param[in]	_r			抽出する赤の度合い[省略可]
		//	@param[in]	_g			抽出する緑の度合い[省略可]
		//	@param[in]	_b			抽出する青の度合い[省略可]
		//	@param[in]	_a			抽出する透明の度合い[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseTexture* _pTexture,
			const float _r,
			const float _g,
			const float _b,
			const float _a);

		/*!-----------------------------------------------------------
		//	@brief		抽出処理
		//	@param[in]	_r			抽出する赤の度合い[省略可]
		//	@param[in]	_g			抽出する緑の度合い[省略可]
		//	@param[in]	_b			抽出する青の度合い[省略可]
		//	@param[in]	_a			抽出する透明の度合い[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Render(
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		描画コマンド実行
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		void	OnDraw();

		/*!-----------------------------------------------------------
		//	@brief		抽出したレンダーターゲットのクリア処理
		//	@param[in]	_r	赤色
		//	@param[in]	_g	緑色
		//	@param[in]	_b	青色
		//	@param[in]	_a	透過率
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		void	Clear(
			const float _r = 0,
			const float _g = 0,
			const float _b = 0,
			const float _a = 0);

		/*!-----------------------------------------------------------
		//	@brief		テクスチャ取得
		//	@param[in]	_sampler	サンプラー
		//  @return     テクスチャ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		BaseTexture* GetBrightnessTexture(SamplerState _sampler=SamplerState::LINEAR)
		{
			return this->pExtRenderTarget->GetRenderTexture(_sampler);
		}

	private:
		struct PsConst
		{
			float sub[4];
		};

		struct COMMAND
		{
			FLOAT sub[4];
			ID3D11ShaderResourceView** ppSrView;
			ID3D11SamplerState** ppSampState;
		};

		std::vector<COMMAND> commandList;
		VertexShader* pVertexShader;
		PixelShader* pPixelShader;
		ConstBuffer<PsConst> psConst;
		SmartComPtr<ID3D11Buffer> pSurfaceBuffer;
		SmartComPtr<ID3D11ShaderResourceView> pBackBufferSrView;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
		DirectX11RenderTarget*	pExtRenderTarget;

	private:
		/*!-----------------------------------------------------------
		//	@brief		サーフェイス作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		inline	HRESULT CreateSurface();
	};
}

#endif // _Include_ExtractBrightnessShader_hpp_
