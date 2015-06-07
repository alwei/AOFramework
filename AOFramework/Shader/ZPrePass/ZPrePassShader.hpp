/*************************************************************
*	@file   	ZPrePassShader.hpp
*	@brief  	ZPrePassShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/09/03
*************************************************************/

#ifndef _Include_ZPrePassShader_hpp_	// インクルードガード
#define _Include_ZPrePassShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<vector>
#include<mutex>
#include<DirectXMath.h>
#include"../GaussianFilter/GaussianFilterShader.hpp"
#include"../RenderBaseShader.hpp"
#include"../../Math/Math.hpp"
#include"../../Math/GameArithmetic.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Resource/Mesh.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Api/DirectX11RenderTarget.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Resource/InstanceBuffer.hpp"

using namespace DirectX;

namespace ao
{
	class RenderBurstBuffer;

	/*!-----------------------------------------------------------
	//	@class  ZPrePassShader
	//	@brief  Z値レンダリングシェーダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/03
	------------------------------------------------------------*/
	class ZPrePassShader : public RenderBaseShader
	{
	public:
		ZPrePassShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~ZPrePassShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pd3dDevice			デバイスポインタ
		//	@param[in]	_pd3dDeviceContext	デバイスコンテキストポインタ
		//  @param[in]  _pGraphicsState		グラフィックステート
		//  @param[in]  _pRenderer			レンダラー
		//	return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		深度テクスチャ作成
		//	@param[in]	_nWidth		横幅解像度
		//  @param[in]  _nHeight	縦幅解像度
		//  @param[in]  _format		フォーマット[省略可]
		//	@param[in]	_degree		画角[省略可]
		//	@param[in]	_nZ			ニアZ値[省略可]
		//	@param[in]	_fZ			ファーZ値[省略可]
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/18
		------------------------------------------------------------*/
		HRESULT	Create(
			const UINT _nWidth,
			const UINT _nHeight,
			const DXGI_FORMAT _format = DXGI_FORMAT_R8G8_UNORM,
			const FLOAT _degree = 60.0f,
			const FLOAT _nZ = 40.0f,
			const FLOAT _fZ = 500.0f);

		/*!-----------------------------------------------------------
		//	@brief		ビュー行列更新
		//	@param[in]	_pViewMtx	ビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		void UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_pMesh			メッシュポインタ
		//	@param[in]	_transform		トランスフォーム
		//	@param[in]	_priority		レンダリング優先度[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseMesh* const _pMesh,
			const Transform& _transform,
			UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		レンダリングコマンド実行
		//	@note		コマンドバッファ版
		//	@param[in]	_pBurstBuffer	バーストバッファ
		//  @param[in]  _priority		プライオリティ[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		HRESULT	ExecuteCommandList(
			RenderBurstBuffer* const _pBurstBuffer,
			UINT priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		プッシュバック終了時の描画
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/19
		------------------------------------------------------------*/
		void	OnPushbackQuit();

		/*!-----------------------------------------------------------
		//	@brief		描画コマンド実行
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		void	OnDraw();

		/*!-----------------------------------------------------------
		//	@brief		リザルト処理
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/19
		------------------------------------------------------------*/
		void	OnResult();

		/*!-----------------------------------------------------------
		//	@brief		射影行列取得
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		const XMFLOAT4X4& GetProjMatrix() const;

		/*!-----------------------------------------------------------
		//	@brief		クリア処理
		//	@param[in]	_depth	クリアする深度値
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/04
		------------------------------------------------------------*/
		void Clear(float _depth = 1)
		{
			this->pDepthRt->Clear(_depth, _depth, 1, 1);
			this->pZBlurRt->Clear(0, 0, 0, 1);
		}

		/*!-----------------------------------------------------------
		//	@brief		ブラー度合いを変更
		//	@param[in]	_blurPower	ブラーパワー[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/06
		------------------------------------------------------------*/
		void SetBlurPower(float _blurPower = 0.5f)
		{
			this->blurPower = _blurPower;
		}

		/*!-----------------------------------------------------------
		//	@brief		レンダリングテクスチャ取得
		//  @param[in]  _sampler	サンプラーステート[省略可]
		//  @return     レンダリングテクスチャ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		BaseTexture* GetRenderTexture(SamplerState _sampler = SamplerState::LINEAR)
		{
			return this->pZBlurRt->GetBlurTexture(_sampler);
		}

	private:
		struct COMMAND
		{
			XMFLOAT4X4 matrix;
			UINT priority;
			BaseMesh* pMesh;
		};

		struct ICOMMAND
		{
			UINT numRender;
			ID3D11ShaderResourceView* pGpuBuffer;
			BaseMesh* pMesh;
			UINT priority;
		};

		struct VsConst
		{
			XMFLOAT4X4 matWVP;
		};

		BOOL isNeedSort;
		FLOAT blurPower;
		std::vector<COMMAND>	commandList;
		std::vector<ICOMMAND>	iCommandList;
		VertexShader*			pVertexShader;
		PixelShader*			pPixelShader;
		VertexShader*			piVertexShader;
		ConstBuffer<VsConst>	vsConst;
		DirectX11RenderTarget*	pDepthRt;
		GaussianFilterShader*	pZBlurRt;
		XMFLOAT4X4 projMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 viewProjMatrix;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;

	private:
		/*!-----------------------------------------------------------
		//	@brief		行列定数バッファマッピング
		//	@param[in]	_mtx		行列
		//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		inline	HRESULT	MapMatrixConstBuffer(const XMFLOAT4X4& _mtx);

		/*!-----------------------------------------------------------
		//	@brief		単一レンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		inline	void	OnDrawSingle();

		/*!-----------------------------------------------------------
		//	@brief		インスタンスレンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		inline	void	OnDrawBurst();
	};
}

#endif // _Include_ZPrePassShader_hpp_