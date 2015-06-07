/*************************************************************
*	@file   	iRenderUiShader.hpp
*	@brief  	iRenderUiShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/10
*************************************************************/

#ifndef _Include_iRenderUiShader_hpp_	// インクルードガード
#define _Include_iRenderUiShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<list>
#include"../RenderBaseShader.hpp"
#include"../../Math/Math.hpp"
#include"../../Math/GameArithmetic.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Resource/ConstVectorBuffer.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/InstanceBuffer.hpp"
#include"../../Resource/Sprite.hpp"

namespace ao
{
	//4Byteアライメント調整のため
	//頂点定数バッファに角度値を4つ用意している
	struct iUiVsConst0
	{
		float degree[4];
	};

	//4Byteアライメント調整のため
	//頂点定数にUV値を用意している
	struct iUiVsConst1
	{
		float2 uv[4];
	};

	//4Byteアライメント調整のため
	//頂点定数に他パラメーターを用意している
	struct iUiVsConst2
	{
		float2 pos;
		float2 size;
		float4 color;
	};

	//ユーザー側が取得するインスタンス
	struct UiInstance
	{
		UINT numPushRender;
		UINT numCommitRender;
		UINT numAllocatedBuffer;
		BaseTexture* pTexture;
		BlendState blendState;
		FillState fillState;
		VertexShader* pVertexShader;
		ConstVectorBuffer<iUiVsConst0> vsConst0;
		ConstVectorBuffer<iUiVsConst1> vsConst1;
		ConstVectorBuffer<iUiVsConst2> vsConst2;
		SmartComPtr<ID3D11Buffer> vertexBuffer;
		UINT priority;
	};

	/*!-----------------------------------------------------------
	//	@class  iRenderUiShader
	//	@brief  UIインスタンスレンダラー
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/10
	------------------------------------------------------------*/
	class iRenderUiShader : public RenderBaseShader
	{
	public:
		iRenderUiShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~iRenderUiShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//  @param[in]  _pGraphicsState		グラフィックステート
		//  @param[in]  _pRenderer			レンダラー
		//  @return     S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/10
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		バッファ作成
		//	@param[in]	_pTexture		使用するテクスチャ
		//	@param[in]	_numAllocate	確保するバッファ数
		//	@return		生成されたインスタンス
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/11
		------------------------------------------------------------*/
		UiInstance* CreateInstance(
			BaseTexture* _pTexture,
			UINT _numAllocate = 100);

		/*!-----------------------------------------------------------
		//	@brief		レンダリングコマンド生成
		//	@param[in]	_pInstance		インスタンスポインタ
		//	@param[in]	_x				X座標
		//	@param[in]	_y				Y座標
		//	@param[in]	_w				横幅
		//	@param[in]	_h				縦幅
		//	@param[in]	_lt				左上UV
		//	@param[in]	_rt				右上UV
		//	@param[in]	_lb				左下UV
		//	@param[in]	_rb				右下UV
		//	@param[in]	_degree			回転角度[省略可]
		//	@param[in]	_r				赤色[省略可]
		//	@param[in]	_g				緑色[省略可]
		//	@param[in]	_b				青色[省略可]
		//	@param[in]	_a				透過率[省略可]
		//  @param[in]  _priority		プライオリティ[省略可]
		//	@return		S_OK：コマンド登録成功　それ以外：コマンド登録失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/10
		------------------------------------------------------------*/
		HRESULT	PushRenderCommand(
			UiInstance* _pInstance,
			const int _x,
			const int _y,
			const float _w,
			const float _h,
			const float2& _lt,
			const float2& _rt,
			const float2& _lb,
			const float2& _rb,
			const float _degree = 0,
			const FLOAT _r = 1.0f,
			const FLOAT _g = 1.0f,
			const FLOAT _b = 1.0f,
			const FLOAT _a = 1.0f,
			UINT priority = 0xCCCCCCC);


		/*!-----------------------------------------------------------
		//	@brief		レンダリングコマンド実行
		//	@param[in]	_pInstance	インスタンスポインタ
		//	@param[in]	_blendState	ブレンドステート[省略可]
		//	@param[in]	_fillState	描画モード[省略可]
		//	@return		S_OK：実行成功　それ以外：実行失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/12
		------------------------------------------------------------*/
		HRESULT ExecuteCommandList(
			UiInstance* _pInstance,
			const BlendState _blendState = BlendState::Alpha,
			const FillState _fillState = FillState::Solid);

		/*!-----------------------------------------------------------
		//	@brief		描画コマンド実行
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/10
		------------------------------------------------------------*/
		void	OnDraw() override final;

		/*!-----------------------------------------------------------
		//	@brief		クリア
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/10
		------------------------------------------------------------*/
		void	Clear();

		/*!-----------------------------------------------------------
		//	@brief		破棄処理
		//	@param[in]	_pInstance	インスタンス
		//  @return     S_OK：破棄成功　それ以外：破棄失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/02/12
		------------------------------------------------------------*/
		HRESULT Destory(UiInstance* _pInstance);

	private:
		BOOL isNeedSort;
		std::vector<UiInstance*> commandList;
		std::list<UiInstance*> pUiInstanceList;
		PixelShader*			pPixelShader;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
		ConstBuffer<XMFLOAT4X4> orthoMatrix;
	};
}

#endif // _Include_iRenderUiShader_hpp_
