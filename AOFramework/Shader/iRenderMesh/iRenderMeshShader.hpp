/*************************************************************
*	@file   	iRenderMeshShader.hpp
*	@brief  	iRenderMeshShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/09/02
*************************************************************/

#ifndef _Include_iRenderMeshShader_hpp_	// インクルードガード
#define _Include_iRenderMeshShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<list>
#include<vector>
#include<mutex>
#include<map>
#include"../RenderBaseShader.hpp"
#include"../../Math/Math.hpp"
#include"../../Math/GameArithmetic.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Resource/Material.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/RenderBurstBuffer.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Define/RenderingDefine.hpp"

using namespace DirectX;


namespace ao
{
	class BaseMesh;

	/*!-----------------------------------------------------------
	//	@class  iRenderMeshShader
	//	@brief  メッシュインスタンスレンダリングシェーダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/02
	------------------------------------------------------------*/
	class iRenderMeshShader : public RenderBaseShader
	{
	public:
		iRenderMeshShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~iRenderMeshShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//  @param[in]  _pGraphicsState		グラフィックステート
		//  @param[in]  _pRenderer			レンダラー
		//	return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* const _pGraphicsState,
			class DirectX11GraphicsRenderer* const _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		レンダリングバーストバッファ作成
		//	@param[in]	_numInstance	インスタンス数
		//  @param[in]  _pMesh			メッシュポインタ
		//	@return		インスタンスポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		RenderBurstBuffer* CreateRenderBurstBuffer(
			const u32 _numInstance,
			BaseMesh* const _pMesh);

		/*!-----------------------------------------------------------
		//	@brief		インスタンスバッファのリサイズ
		//	@param[in]	_numInstance	リサイズするインスタンス数
		//	@return		_pInstance		リサイズするインスタンス
		//	@return		S_OK：リサイズ成功　それ以外：リサイズ失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/17
		------------------------------------------------------------*/
		HRESULT ResizeRenderBurstBuffer(
			const u32 _numInstance,
			RenderBurstBuffer* const _pInstanceBuffer);

		/*!-----------------------------------------------------------
		//	@brief		ライト用の射影行列更新
		//	@param[in]	_lightProjMtx	ライト用の射影行列
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/14
		------------------------------------------------------------*/
		void UpdateLightProjMatrix(const XMFLOAT4X4& _lightProjMtx);

		/*!-----------------------------------------------------------
		//	@brief		ビュー行列更新
		//	@param[in]	_pViewMtx	ビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		void UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		ライトビュー行列更新
		//	@param[in]	_pLightViewMtx	ライトビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/18
		------------------------------------------------------------*/
		void UpdateLightViewMatrix(const XMFLOAT4X4* _pLightViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		ライト方向更新
		//	@param[in]	_x				X方向
		//	@param[in]	_y				Y方向
		//	@param[in]	_z				Z方向
		//	@param[in]	_ambientPower	環境光の強さ[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		void UpdateLightDirection(float _x, float _y, float _z, float _ambientPower = 0.5f);

		/*!-----------------------------------------------------------
		//	@brief		視線座標更新
		//	@param[in]	_x				X座標
		//  @param[in]  _y				Y座標
		//  @param[in]  _z				Z座標
		//  @param[in]  _specularPower	スペキュラ強度[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		void UpdateEyePosition(float _x, float _y, float _z,float _specularPower=1.0f);

		/*!-----------------------------------------------------------
		//	@brief		レンダリングコマンド生成
		//	@param[in]	_pRenderBurstBuffer	レンダリングバーストバッファ
		//	@param[in]	_transform			姿勢
		//	@param[in]	_r					赤色[省略可]
		//	@param[in]	_g					緑色[省略可]
		//	@param[in]	_b					青色[省略可]
		//	@param[in]	_a					透過率[省略可]
		//	@return		S_OK：コマンド登録成功　それ以外：コマンド登録失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		HRESULT	PushRenderCommand(
			RenderBurstBuffer* const _pRenderBurstBuffer,
			Transform& _transform,
			const FLOAT _r = 1.0f,
			const FLOAT _g = 1.0f,
			const FLOAT _b = 1.0f,
			const FLOAT _a = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		レンダリングコマンド実行
		//	@note		コマンドバッファ版
		//	@param[in]	_pInstance			インスタンス
		//	@param[in]	_renderFlags		レンダリングタイプ[省略可]
		//	@param[in]	_blendState			ブレンドステート[省略可]
		//	@param[in]	_renderFlags		レンダリングフラグ[省略可]
		//  @param[in]  _blendState			ブレンドステート[省略可]
		//  @param[in]  _fillState			描画モード[省略可]
		//  @param[in]  _priority			プライオリティ[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/02
		------------------------------------------------------------*/
		HRESULT	ExecuteCommandList(
			RenderBurstBuffer* const _pInstanceBuffer,
			const u32 _renderFlags = RenderDef::LIGHTING,
			const BlendState _blendState = BlendState::Alpha,
			const FillState _fillState = FillState::Solid,
			const u32 priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		描画コマンド実行
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		void	OnDraw() override final;

		/*!-----------------------------------------------------------
		//	@brief		クリア
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		void	Clear();

	private:
		struct COMMAND
		{
			u32 priority;
			u32 numRendering;
			u32 renderFlags;
			BlendState blendState;
			FillState fillState;
			RenderBurstBuffer* pRenderBurstBuffer;
		};

		struct PsConst0
		{
			FLOAT diffuse[4];
			FLOAT specular[4];
			FLOAT emissive[4];
		};

		BOOL isNeedSort;
		std::vector<COMMAND>	commandList;
		VertexShader*			pVertexShader;
		PixelShader*			pPixelShader;
		VertexShader*			pShadowVertexShader;
		PixelShader*			pShadowPixelShader;
		ConstBuffer<PsConst0>	psConst;
		float4 lightDirection;
		float4 eyePosition;
		XMFLOAT4X4 projMatrix;
		XMFLOAT4X4 lightProjMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 lightViewMatrix;
		XMFLOAT4X4 viewProjMatrix;
		XMFLOAT4X4 lightViewProjMatrix;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
		std::map<ao_intptr, RenderBurstBuffer*> pRenderBurstBufferMap;

	private:

		/*!-----------------------------------------------------------
		//	@brief		定数バッファ領域確保
		//	@param[in]	_numInstance		インスタンス数
		//	@param[in]	_pGpuBuffer			GPUバッファ
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/10
		------------------------------------------------------------*/
		template <class T>
		inline HRESULT CreateConstantBuffer(
			const u32 _numInstance,
			RenderBurstBuffer::GpuBuffer<T>* const _pGpuBuffer);


		/*!-----------------------------------------------------------
		//	@brief		定数バッファをマップ
		//	@param[in]	_pGpuBuffer	GPUバッファ
		//	@param[in]	_pMapBuffer	マップする定数バッファ
		//	@param[in]	_dataSize	データサイズ
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/12
		------------------------------------------------------------*/
		template<class T>
		inline HRESULT MapConstantBuffers(RenderBurstBuffer::GpuBuffer<T>* const _pGpuBuffer);

		/*!-----------------------------------------------------------
		//	@brief		定数バッファマッピング
		//	@param[in]	_numSet				マップする数
		//	@param[in]	_pInstanceBuffer	インスタンスバッファ
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/11
		------------------------------------------------------------*/
		inline HRESULT SetConstantBuffers(const u32 _numSet, RenderBurstBuffer* const _pRenderBurstBuffer);

		/*!-----------------------------------------------------------
		//	@brief		マテリアル定数バッファマッピング
		//	@param[in]	_material	マテリアル
		//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		inline	HRESULT	MapMaterialConstBuffer(const Material& _material);
	};
}

#endif // _Include_iRenderMeshShader_hpp_