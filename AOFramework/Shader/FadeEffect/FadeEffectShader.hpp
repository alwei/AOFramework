/*************************************************************
*	@file   	FadeEffectShader.hpp
*	@brief  	FadeEffectShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/12/03
*************************************************************/

#ifndef _Include_FadeEffectShader_hpp_	// インクルードガード
#define _Include_FadeEffectShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../../../AOFramework/Api/DirectX11GraphicsRenderer.hpp"
#include"../../../AOFramework/Api/DirectX11GraphicsState.hpp"
#include"../../../AOFramework/Resource/ShaderResource.hpp"
#include"../../../AOFramework/Resource/ConstBuffer.hpp"
#include"../../../AOFramework/Shader/RenderBaseShader.hpp"


namespace ao
{

	enum class FadeType : int
	{
		BlockIn = 0,
		BlackOut,
		WhiteIn,
		WhiteOut
	};

	/*!-----------------------------------------------------------
	//	@class  FadeEffectShader
	//	@brief  フェードエフェクトシェーダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/12/03
	------------------------------------------------------------*/
	class FadeEffectShader : public RenderBaseShader
	{
	public:
		/// コンストラクタ
		FadeEffectShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		/// デストラクタ
		~FadeEffectShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pGraphicsState		グラフィックステート
		//  @param[in]  _pRenderer			レンダラー
		//	return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/01
		------------------------------------------------------------*/
		HRESULT	Initialize(
			ao::DirectX11GraphicsState* _pGraphicsState,
			ao::DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		フェードパラメーター設定
		//	@param[in]	_fadeType		フェードの種類
		//  @param[in]  _sec			フェードする時間（秒）
		//	@param[in]	_init			初期化する時間[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/03
		------------------------------------------------------------*/
		void SetFadeParameter(FadeType _fadeType, float _sec,float _init=0);

		/*!-----------------------------------------------------------
		//	@brief		更新
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/03
		------------------------------------------------------------*/
		void Update();

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/03
		------------------------------------------------------------*/
		void Render();

		/*!-----------------------------------------------------------
		//	@brief		描画メソッド
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		void OnDraw();

		/*!-----------------------------------------------------------
		//	@brief		設定時間を取得
		//	@return		設定の時間
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/09
		------------------------------------------------------------*/
		float GetMaxTime()
		{
			return this->maxFadeTime;
		}

		/*!-----------------------------------------------------------
		//	@brief		現在の時間を取得
		//	@return		現在の時間
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/09
		------------------------------------------------------------*/
		float GetNowTime()
		{
			return this->nowFadeTime;
		}

		/*!-----------------------------------------------------------
		//	@brief		正規化された時間を取得
		//  @return     正規化された時間
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/03
		------------------------------------------------------------*/
		float GetNormalizeTime()
		{
			return this->nowFadeTime / this->maxFadeTime;
		}

		/*!-----------------------------------------------------------
		//	@brief		フェードが終了したかどうか
		//  @return     TRUE：終了　FALSE：終了していない
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/03
		------------------------------------------------------------*/
		BOOL IsFadeEnd()
		{
			return (this->nowFadeTime >= this->maxFadeTime);
		}

	private:
		BOOL isPushRender;
		DirectX11GraphicsState* pGraphicsState;
		DirectX11GraphicsRenderer* pRenderer;
		ConstBuffer<ao::float4> mapColor;
		SmartComPtr<ID3D11Buffer> vertexBuffer;
		VertexShader* pVertexShader;
		PixelShader* pPixelShader;
		float3 fadeColor;
		FadeType fadeType;
		float fadeAlpha;
		float nowFadeTime;
		float maxFadeTime;
	};
}

#endif // _Include_FadeEffectShader_hpp_