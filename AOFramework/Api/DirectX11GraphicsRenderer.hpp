/*************************************************************
*	@file   	DirectX11GraphicsRenderer.hpp
*	@brief  	DirectX11GraphicsRendererクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/10
*************************************************************/

#ifndef _Include_DirectX11GraphicsRenderer_hpp_	// インクルードガード
#define _Include_DirectX11GraphicsRenderer_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<list>
#include<array>
#include<vector>
#include<mutex>
#include<DirectXMath.h>
#include"DirectX11BaseApi.hpp"
#include"../Math/GameArithmetic.hpp"
#include"../Resource/BaseTexture.hpp"
#include"../Shader/RenderBaseShader.hpp"
#include"../Define/RenderingDefine.hpp"
#include"../Resource/ConstBuffer.hpp"
#include"DirectX11RenderTarget.hpp"

using namespace  DirectX;

namespace ao
{
	class DirectX11GraphicsState;

	struct AxisData
	{
		union
		{
			struct
			{
				float sx, sy, sz;
				float sr, sg, sb, sa;
			};
			struct{ float fs[3]; };
		};
		union
		{
			struct
			{
				float ex, ey, ez;
				float er, eg, eb, ea;
			};
			struct{ float fe[3]; };
		};
		AxisData() :sr(1), sg(1), sb(1), sa(1), er(1), eg(1), eb(1), ea(1){}
	};

	/*!-----------------------------------------------------------
	//	@class  DirectX11GraphicsRenderer
	//	@brief  グラフィックレンダラー
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/10
	------------------------------------------------------------*/
	class DirectX11GraphicsRenderer : public DirectX11BaseApi
	{
	public:
		DirectX11GraphicsRenderer(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11GraphicsRenderer()override;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pSwapChain			スワップチェイン
		//	@param[in]	_pBackBuffer		バックバッファポインタ
		//  @param[in]  _pGraphicsState		グラフィックステート
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		HRESULT	Initialize(
			SmartComPtr<IDXGISwapChain> _pSwapChain,
			DirectX11RenderTarget* _pBackBuffer,
			DirectX11GraphicsState* _pGraphicsState);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング開始
		//	@param[in]	_r	クリア赤色[省略可]
		//	@param[in]	_g	クリア緑色[省略可]
		//	@param[in]	_b	クリア青色[省略可]
		//	@param[in]	_a	クリア透過率[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/11
		------------------------------------------------------------*/
		void	RenderBegin(float _r = 0, float _g = 0, float _b = 0, float _a = 0);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング終了
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/11
		------------------------------------------------------------*/
		void	RenderEnd();

		/*!-----------------------------------------------------------
		//	@brief		テクスチャ設定
		//	@param[in]	_srSlot			設定するスロット
		//	@param[in]	_sampSlots		設定するサンプラースロット
		//	@param[in]	_pTexture		設定するテクスチャ
		//	@param[in]	_isCache		キャッシュフラグ[省略可]
		//	@param[in]	_isForceBind	強制的にBindするかどうか[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		void	SetTexture(
			int _srSlot,
			int _sampSlot,
			BaseTexture* _pTexture,
			BOOL _isCache = TRUE,
			BOOL _isForceBind = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		テクスチャ設定
		//	@param[in]	_srSlot			設定するスロット
		//	@param[in]	_sampSlots		設定するサンプラースロット
		//  @param[in]  _ppSrView		設定するシェーダーリソースビュー[省略可]
		//  @param[in]  _ppSampler		設定するサンプラーステート[省略可]
		//	@param[in]	_isCache		キャッシュフラグ[省略可]
		//	@param[in]	_isForceBind	強制的にBindするかどうか[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		void	SetTexture(
			int _srSlot,
			int _sampSlot,
			ID3D11ShaderResourceView** _ppSrView = nullptr,
			ID3D11SamplerState** _ppSampler = nullptr,
			BOOL _isCache = TRUE,
			BOOL _isForceBind = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		テクスチャ設定
		//	@param[in]	_num			設定する数
		//	@param[in]	_srSlotStart	設定する開始スロット
		//	@param[in]	_sampSlotStart	設定する開始サンプラースロット
		//	@param[in]	_pTextures		設定するテクスチャ配列
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		void	SetTexture(
			int num,
			int _srSlotStart,
			int _sampSlotStart,
			BaseTexture** _pTextures);

		/*!-----------------------------------------------------------
		//	@brief		頂点テクスチャ設定
		//	@param[in]	_srSlot			設定するスロット
		//	@param[in]	_sampSlots		設定するサンプラースロット
		//	@param[in]	_pTexture		設定するテクスチャ
		//	@param[in]	_isCache		キャッシュフラグ[省略可]
		//	@param[in]	_isForceBind	強制的にBindするかどうか[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/06
		------------------------------------------------------------*/
		void	SetVertexTexture(
			int _srSlot,
			int _sampSlot,
			BaseTexture* _pTexture,
			BOOL _isCache = TRUE,
			BOOL _isForceBind = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		頂点テクスチャ設定
		//	@param[in]	_srSlot			設定するスロット
		//	@param[in]	_sampSlots		設定するサンプラースロット
		//  @param[in]  _ppSrView		設定するシェーダーリソースビュー[省略可]
		//  @param[in]  _ppSampler		設定するサンプラーステート[省略可]
		//	@param[in]	_isCache		キャッシュフラグ[省略可]
		//	@param[in]	_isForceBind	強制的にBindするかどうか[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/06
		------------------------------------------------------------*/
		void	SetVertexTexture(
			int _srSlot,
			int _sampSlot,
			ID3D11ShaderResourceView** _ppSrView = nullptr,
			ID3D11SamplerState** _ppSampler = nullptr,
			BOOL _isCache = TRUE,
			BOOL _isForceBind = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		頂点テクスチャ設定
		//	@param[in]	_num			設定する数
		//	@param[in]	_srSlotStart	設定する開始スロット
		//	@param[in]	_sampSlotStart	設定する開始サンプラースロット
		//	@param[in]	_pTextures		設定するテクスチャ配列
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/06
		------------------------------------------------------------*/
		void	SetVertexTexture(
			int num,
			int _srSlotStart,
			int _sampSlotStart,
			BaseTexture** _pTextures);

		/*!-----------------------------------------------------------
		//	@brief		プリレンダリング登録
		//	@param[in]	_pNode		登録レンダラーノード
		//  @param[in]  _priority	レンダリング優先度[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		void	PreRenderPushback(RenderBaseShader* _pNode, UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング登録
		//	@param[in]	_pNode		登録レンダラーノード
		//	@param[in]	_priority	レンダリング優先度[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		void	RenderPushback(RenderBaseShader* _pNode, UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		ポストレンダリング登録
		//	@param[in]	_pNode		登録レンダラーノード
		//	@param[in]	_priority	レンダリング優先度[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		void	PostRenderPushback(RenderBaseShader* _pNode, UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		プリバーストレンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		HRESULT PreRenedrBurst();

		/*!-----------------------------------------------------------
		//	@brief		バーストレンダリング
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		HRESULT	RenderBurst();

		/*!-----------------------------------------------------------
		//	@brief		ポストバーストレンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	PostRenderBurst();

		/*!-----------------------------------------------------------
		//	@brief		レンダリングを全て破棄
		//	@author		Tatsunori Aoyama
		//	@date		2014/０９/15
		------------------------------------------------------------*/
		void	ClearRender();

		/*!-----------------------------------------------------------
		//	@brief		軸描画
		//	@param[in]	_sx			開始X座標
		//	@param[in]	_sy			開始Y座標
		//	@param[in]	_sz			開始Z座標
		//	@param[in]	_ex			終端X座標
		//	@param[in]	_ey			終端Y座標
		//	@param[in]	_ez			終端Z座標
		//	@param[in]	_r			赤色[省略可]
		//	@param[in]	_g			緑色[省略可]
		//	@param[in]	_b			青色[省略可]
		//	@param[in]	_a			透過率[省略可]
		//	@param[in]	_pOrigin	ワールド変換する行列[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/15
		------------------------------------------------------------*/
		HRESULT	RenderAxis(
			const float _sx,
			const float _sy,
			const float _sz,
			const float _ex,
			const float _ey,
			const float _ez,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const XMFLOAT4X4* _pOrigin = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		軸描画のバースト
		//	@note		恐らく激重
		//	@param[in]	_axisData	軸データ
		//	@param[in]	_num		軸の本数
		//	@param[in]	_r			赤色[省略可]
		//	@param[in]	_g			緑色[省略可]
		//	@param[in]	_b			青色[省略可]
		//	@param[in]	_a			透過率[省略可]
		//	@param[in]	_pOrigin	ワールド変換する行列[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/07
		------------------------------------------------------------*/
		HRESULT	RenderAxis(
			const AxisData* _axisData,
			const int _num,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const XMFLOAT4X4* _pOrigin = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		軸描画
		//	@note		頂点バッファをマニュアル指定
		//	@param[in]	_pVB		マップする頂点バッファ
		//	@param[in]	_numRender	レンダリングする軸本数
		//	@param[in]	_r			全体の赤色[省略可]
		//	@param[in]	_g			全体の緑色[省略可]
		//	@param[in]	_b			全体の青色[省略可]
		//	@param[in]	_a			全体の透過率[省略可]
		//	@param[in]	_pOrigin	ワールド変換する行列[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/15
		------------------------------------------------------------*/
		HRESULT	RenderAxis(
			ID3D11Buffer* _pVB,
			const int _numRender,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const XMFLOAT4X4* _pOrigin = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		姿勢軸描画
		//	@param[in]	_pose	姿勢
		//  @param[in]  _length	長さ[省略可]
		//	@param[in]	_r		赤色[省略可]
		//	@param[in]	_g		緑色[省略可]
		//	@param[in]	_b		青色[省略可]
		//	@param[in]	_a		透過率[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/08
		------------------------------------------------------------*/
		HRESULT	RenderPose(
			Transform& _pose,
			const float _length = 2.0f,
			float _r = 1.0f,
			float _g = 1.0f,
			float _b = 1.0f,
			float _a = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		グリッド描画
		//	@param[in]	_x		X座標[省略可]
		//	@param[in]	_y		Y座標[省略可]
		//	@param[in]	_z		Z座標[省略可]
		//	@param[in]	_num	グリッド数[省略可]
		//	@param[in]	_space	間隔[省略可]
		//	@param[in]	_r		赤色[省略可]
		//	@param[in]	_g		緑色[省略可]
		//	@param[in]	_b		青色[省略可]
		//	@param[in]	_a		透過率[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/14
		------------------------------------------------------------*/
		HRESULT RenderGrid(
			const float _x = 0.0f,
			const float _y = 0.0f,
			const float _z = 0.0f,
			const int _num = 10,
			const float _space = 5.0f,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f);


		/*!-----------------------------------------------------------
		//	@brief		画面上へスプライトの簡易レンダリング
		//	@param[in]	_x			X座標
		//	@param[in]	_y			Y座標
		//	@param[in]	_width		横幅
		//	@param[in]	_height		縦幅
		//	@param[in]	_color		色[省略可]
		//	@param[in]	_pTexture	テクスチャ[省略可]
		//	@param[in]	_sampType	サンプラータイプ[省略可]
		//	@param[in]	_orthoType	正射影行列のタイプ[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/20
		------------------------------------------------------------*/
		HRESULT RenderSprite(
			const float _x,
			const float _y,
			const float _width,
			const float _height,
			const color _color = color::white,
			BaseTexture* const _pTexture = nullptr,
			const SamplerState _sampType = SamplerState::POINT,
			const OrthoType _orthoType = OrthoType::LeftTop);

		/*!-----------------------------------------------------------
		//	@brief		ビュー行列の更新
		//	@param[in]	_viewMtx	ビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/15
		------------------------------------------------------------*/
		void	UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		正射影行列取得(左上原点)
		//	@param[out]	_pOut	出力する正射影行列
		//	@return		出力する射影行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		const XMFLOAT4X4*	GetLeftTopOrthoMatrix(XMFLOAT4X4* _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		正射影行列取得(中心原点)
		//	@param[out]	_pOut	出力する正射影行列
		//	@return		出力する射影行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		const XMFLOAT4X4*	GetCenterOrthoMatrix(XMFLOAT4X4* _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		射影行列取得
		//	@param[out]	_pOut	出力する射影行列
		//	@return		出力する射影行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		const XMFLOAT4X4*	GetProjMatrix(XMFLOAT4X4* _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		ビュー行列取得
		//	@param[out]	_pOut	出力するビュー行列
		//	@return		出力するビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		const XMFLOAT4X4*	GetViewMatrix(XMFLOAT4X4* _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		ビュー行列取得
		//	@param[out]	_pOut	出力するビュー射影行列
		//	@return		出力するビュー射影行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/17
		------------------------------------------------------------*/
		const XMFLOAT4X4*	GetViewProjMatrix(XMFLOAT4X4* _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		バックバッファビューポート取得
		//	@param[out]	_pOut	出力するビューポート
		//  @return     出力するビューポート
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		const D3D11_VIEWPORT* GetViewport(D3D11_VIEWPORT* _pOut)
		{
			return this->pRenderTarget->GetViewport(_pOut);
		}

		/*!-----------------------------------------------------------
		//	@brief		メインのレンダーターゲットを取得
		//	@return		メインのレンダーターゲットテクスチャ
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/22
		------------------------------------------------------------*/
		BaseTexture* GetMainTexture()
		{
			return this->pRenderTarget->GetRenderTexture();
		}

	private:
		static const u32 MAX_GRID = 3000;
		static const u32 MAX_GRID_BUFFER = (MAX_GRID * 2 + 1) * 2;
		static const u32 MAX_TEXTURE = 128;

		struct GridBuffer
		{
			struct Vertex
			{
				float x, y, z;
				float r, g, b, a;
				Vertex() :r(1), g(1), b(1), a(1){}
			};
			std::array<Vertex, MAX_GRID_BUFFER > vertex;
			SmartComPtr<ID3D11Buffer> buffer;
		};

		template<u32 NUM>
		struct AxisBuffer
		{
			struct Vertex
			{
				float x, y, z;
				float r, g, b, a;
				Vertex() :r(1), g(1), b(1), a(1){}
			};
			std::array<Vertex, NUM> vertex;
			SmartComPtr<ID3D11Buffer> buffer;
		};

		struct ShaderPKG
		{
			SmartComPtr<ID3D11VertexShader> vertex;
			SmartComPtr<ID3D11InputLayout> inputLayout;
			SmartComPtr<ID3D11PixelShader> pixel;
		};

		struct GridConstBuffer
		{
			struct VsConst
			{
				DirectX::XMFLOAT4X4	matWVP;
			};
			struct PsConst
			{
				float color[4];
			};
			VsConst rawVsBuffer;
			PsConst rawPsBuffer;
			SmartComPtr<ID3D11Buffer> vsBuffer;
			SmartComPtr<ID3D11Buffer> psBuffer;
		};


		struct SpriteConstBuffer
		{
			matrix matWP;
			color color;
		};

		struct RenderNode
		{
			RenderBaseShader* shader;
			UINT priority;
		};

		struct Projection
		{
			float fovAngleY;
			float aspect;
			float nearZ;
			float farZ;
		};

	private:
		BOOL isNeedSortRenderList;
		BOOL isNeedSortPreRenderList;
		BOOL isNeedSortPostRenderList;
		SmartComPtr<IDXGISwapChain> pSwapChain;
		DirectX11RenderTarget* pRenderTarget;
		DirectX11GraphicsState* pGraphicsState;
		std::vector<RenderNode> renderList;
		std::vector<RenderNode> preRenderList;
		std::vector<RenderNode> postRenderList;
		AxisBuffer<2> axisBuffer;
		AxisBuffer<6> poseBuffer;
		GridBuffer xGridBuffer;
		GridBuffer zGridBuffer;
		ShaderPKG gridShader;
		ShaderPKG spriteShader;
		ConstBuffer<SpriteConstBuffer> spriteConstBuffer;
		SmartComPtr<ID3D11Buffer> pSpriteVertexBuffer;
		GridConstBuffer gridConstBuffer;
		Projection projection;
		XMFLOAT4X4 projMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 viewProjMatrix;
		XMFLOAT4X4 leftTopOrthoMatrix;
		XMFLOAT4X4 centerOrthoMatrix;
		std::array<ao_intptr, MAX_TEXTURE>	logSrViewBuffer;
		std::array<ao_intptr, MAX_TEXTURE>	logSamplerBuffer;
		std::array<ao_intptr, MAX_TEXTURE>	logVertexSrViewBuffer;
		std::array<ao_intptr, MAX_TEXTURE>	logVertexSamplerBuffer;
		std::array<BaseTexture, MAX_TEXTURE> nullTexture;

	private:
		/*!-----------------------------------------------------------
		//	@brief		nullテクスチャ作成
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		inline HRESULT	CreateNullTexture();

		/*!-----------------------------------------------------------
		//	@brief		null用のサンプラーステート作成
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		inline HRESULT CreateNullSamplerState();

		/*!-----------------------------------------------------------
		//	@brief		軸定数バッファ
		//	@param[in]	_worldMtx	ワールド行列
		//	@param[in]	_color		色配列
		//	@return		S_OK：マップ成功　それ以外：マップ失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/15
		------------------------------------------------------------*/
		inline HRESULT	MapAxisConstBuffer(const XMFLOAT4X4& _worldMtx, const float* const _color);

		/*!-----------------------------------------------------------
		//	@brief		シェーダーファイルの読み込み
		//	@param[in]	_vsFilePath	頂点シェーダーファイルパス
		//	@param[in]	_psFilePath	ピクセルシェーダーファイルパス
		//	@param[out]	_pOutShader	出力するシェーダーポインタ	
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/20
		------------------------------------------------------------*/
		HRESULT LoadShaderFile(LPCWSTR _vsFilePath, LPCWSTR _psFilePath, ShaderPKG* const _pOutShader);
	};
}

#endif // _Include_DirectX11GraphicsRenderer_hpp_