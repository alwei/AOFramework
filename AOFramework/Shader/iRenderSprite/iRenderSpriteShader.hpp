/*************************************************************
*	@file   	iRenderSpriteShader.hpp
*	@brief  	iRenderSpriteShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/09/29
*************************************************************/

#ifndef _Include_iRenderSpriteShader_hpp_	// インクルードガード
#define _Include_iRenderSpriteShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../RenderBaseShader.hpp"
#include"../../Math/Math.hpp"
#include"../../Math/GameArithmetic.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/InstanceBuffer.hpp"
#include"../../Resource/Sprite.hpp"

namespace ao
{
	class SpriteInstance
	{
	public:
		struct Instance
		{
			XMFLOAT4X4 matrix;	//行列
			XMFLOAT4 color;		//色
		};
		int numPushRender;
		int maxPushRender;
		Sprite* pSprite;
		Instance* pInstance;
		SmartComPtr<ID3D11Buffer> buffer;
	};

	/*!-----------------------------------------------------------
	//	@class  iRenderSpriteShader
	//	@brief  スプライトレンダラー
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/29
	------------------------------------------------------------*/
	class iRenderSpriteShader : public RenderBaseShader
	{
	public:
		iRenderSpriteShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~iRenderSpriteShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//  @param[in]  _pGraphicsState		グラフィックステート
		//  @param[in]  _pRenderer			レンダラー
		//  @return     S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		インスタンス作成
		//	@param[in]	_numInstance	インスタンス数
		//  @param[in]  _pSprite		生成するスプライト
		//  @param[in]  _pivot			スプライト基点[省略可]
		//  @return     インスタンス
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		SpriteInstance* CreateInstance(UINT _numInstance, Sprite* _pSprite, SpritePivot _pivot = SpritePivot::Center);

		/*!-----------------------------------------------------------
		//	@brief		ビュー行列更新
		//	@param[in]	_pViewMtx	ビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		レンダリングコマンド生成
		//	@param[in]	_pInstance		インスタンス
		//	@param[in]	_transform		姿勢
		//	@param[in]	_r				赤色[省略可]
		//	@param[in]	_g				緑色[省略可]
		//	@param[in]	_b				青色[省略可]
		//	@param[in]	_a				透過率[省略可]
		//	@return		S_OK：コマンド登録成功　それ以外：コマンド登録失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		HRESULT	PushRenderCommand(
			SpriteInstance* const _pInstance,
			Transform& _transform,
			const FLOAT _r = 1.0f,
			const FLOAT _g = 1.0f,
			const FLOAT _b = 1.0f,
			const FLOAT _a = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		レンダリングコマンド実行
		//	@note		コマンドバッファ版
		//	@param[in]	_pInstance			インスタンス
		//  @param[in]  _isBillboard		ビルボードフラグ
		//	@param[in]	_blendState			ブレンドステート[省略可]
		//  @param[in]  _fillState			描画モード[省略可]
		//  @param[in]  _priority			プライオリティ[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		HRESULT	ExecuteCommandList(
			SpriteInstance* const _pInstance,
			BlendState _blendState = BlendState::Alpha,
			const FillState _fillState = FillState::Solid,
			UINT priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		描画コマンド実行
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void	OnDraw()override final;

		/*!-----------------------------------------------------------
		//	@brief		クリア
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void	Clear();

	private:
		struct COMMAND
		{
			UINT priority;
			UINT numRender;
			BOOL isBillboard;
			BlendState blendState;
			FillState fillState;
			SpriteInstance* pInstance;
		};

		struct PsConst
		{
			FLOAT color[4];
		};

	private:
		BOOL isNeedSort;
		std::mutex mutex;
		std::vector<COMMAND> commandList;
		std::list<SpriteInstance*>	pInstanceDataList;
		VertexShader*			pVertexShader;
		PixelShader*			pPixelShader;
		ConstBuffer<PsConst>	psConst;
		XMFLOAT4X4 projMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 viewProjMatrix;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
	};
}

#endif // _Include_iRenderSpriteShader_hpp_
