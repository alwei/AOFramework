/*************************************************************
*	@file   	RenderMeshShader.hpp
*	@brief  	RenderMeshShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/20
*************************************************************/

#ifndef _Include_RenderMeshShader_hpp_	// インクルードガード
#define _Include_RenderMeshShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<vector>
#include<mutex>
#include<DirectXMath.h>
#include"../RenderBaseShader.hpp"
#include"../../Define/SystemDefine.hpp"
#include"../../Math/Math.hpp"
#include"../../Math/GameArithmetic.hpp"
#include"../../Resource/Mesh.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"

using namespace DirectX;

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  RenderMeshShader
	//	@brief  メッシュレンダリングシェーダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/20
	------------------------------------------------------------*/
	class RenderMeshShader : public RenderBaseShader
	{
	public:
		RenderMeshShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~RenderMeshShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pGraphicsState		グラフィックステート
		//  @param[in]  _pRenderer			レンダラー
		//	return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		ライト用の射影行列更新
		//	@param[in]	_lightProjMtx	ライト用の射影行列
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		void UpdateLightProjMatrix(const XMFLOAT4X4& _lightProjMtx);

		/*!-----------------------------------------------------------
		//	@brief		ビュー行列更新
		//	@param[in]	_pViewMtx	ビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		void UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		ライトビュー行列更新
		//	@param[in]	_pLightViewMtx	ライトビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/19
		------------------------------------------------------------*/
		void UpdateLightViewMatrix(const XMFLOAT4X4* _pLightViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		ライト方向更新
		//	@param[in]	_x				X方向
		//	@param[in]	_y				Y方向
		//	@param[in]	_z				Z方向
		//	@param[in]	_ambientPower	環境光の強さ[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/21
		------------------------------------------------------------*/
		void UpdateLightDirection(
			float _x, float _y, float _z,
			float _ambientPower=0.5f);

		/*!-----------------------------------------------------------
		//	@brief		視点座標更新
		//	@param[in]	_x				X座標
		//  @param[in]  _y				Y座標
		//  @param[in]  _z				Z座標
		//  @param[in]  _specularPower	スペキュラ強度
		//	@param[in]	_specularRange	スペキュラ範囲
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		void UpdateEyePosition(
			float _x, float _y, float _z,
			float _specularPower = 1.0f,int _specularRange=10);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_pMesh				メッシュポインタ
		//	@param[in]	_transform			トランスフォーム
		//  @param[in]  disableLighting		ライティングの有無[省略可]
		//	@param[in]	_isReceiveShadow	影の描画フラグ[省略可]
		//  @param[in]  _blendState			ブレンドステート[省略可]
		//  @param[in]  _fillState			描画ステート[省略可]
		//	@param[in]	_dColor				ディフューズ色
		//	@param[in]	_sColor				スペキュラ色
		//	@param[in]	_eColor				エミッシブ色
		//	@param[in]	_priority			レンダリング優先度[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseMesh* const _pMesh,
			Transform& _transform,
			const BOOL _disableLighting = FALSE,
			const BOOL _isReceiveShadow = FALSE,
			const BlendState _blendState=BlendState::Alpha,
			const FillState _fillState = FillState::Solid,
			const ao::float4 _dColor = ao::float4(1,1,1,1),
			const ao::float4 _sColor = ao::float4(1,1,1,0),
			const ao::float4 _eColor = ao::float4(0,0,0,0),
			const UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		描画コマンド実行
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		void	OnDraw();

	private:
		struct COMMAND
		{
			XMFLOAT4X4 matrix;
			BaseMesh::Subsets* pSubsets;
			BlendState blendState;
			FillState fillState;
			BOOL disableLighting;
			BOOL shadowFlag;
			u32 priority;
			u32 stride;
			ao::float4 dColor;
			ao::float4 sColor;
			ao::float4 eColor;
			ID3D11Buffer** ppVertexBuffer;
			ID3D11Buffer* pIndexBuffer;
		};

		struct VsConst
		{
			XMFLOAT4X4 matWVP;
			ao::float4 eyePos;
			XMFLOAT4X4 matLWVP;
		};

		struct PsConst0
		{
			ao::float4 diffuse;
			ao::float4 specular;
			ao::float4 emissive;
		};
		struct PsConst1
		{
			ao::float4 lightDir;
			ao::float4 dColor;
			ao::float4 sColor;
			ao::float4 eColor;
			int	disableLighting;
			int specularRange;
			int dummy[2];
		};

		BOOL isNeedSort;
		std::vector<COMMAND>	commandList;
		VertexShader*			pVertexShader;
		PixelShader*			pPixelShader;
		VertexShader*			pVSMVertexShader;
		PixelShader*			pVSMPixelShader;
		ConstBuffer<VsConst>	vsConst;
		ConstBuffer<PsConst0>	psConst0;
		ConstBuffer<PsConst1>	psConst1;
		XMFLOAT3 lightDir;
		XMFLOAT3 eyePos;
		XMFLOAT4X4 projMatrix;
		XMFLOAT4X4 lightProjMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 lightViewMatrix;
		XMFLOAT4X4 viewProjMatrix;
		XMFLOAT4X4 lightViewProjMatrix;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;

	private:
		/*!-----------------------------------------------------------
		//	@brief		行列定数バッファマッピング
		//	@param[in]	_mtx				行列
		//	@param[in]	_material			マテリアル
		//	@param[in]	_isReceiveShadow	影フラグ
		//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		HRESULT	MapConstBuffer(
			const XMFLOAT4X4& _mtx,
			BOOL _isReceiveShadow);

		/*!-----------------------------------------------------------
		//	@brief		マテリアルマッピング
		//	@param[in]	_material	マテリアル
		//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		HRESULT MapMaterialBuffer(const Material& _material);
	};
}

#endif // _Include_RenderMeshShader_hpp_