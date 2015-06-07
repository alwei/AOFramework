/*************************************************************
*	@file   	RenderMeshAnimationShader.hpp
*	@brief  	RenderMeshAnimationShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/10/31
*************************************************************/

#ifndef _Include_RenderMeshAnimationShader_hpp_	// インクルードガード
#define _Include_RenderMeshAnimationShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<vector>
#include<mutex>
#include<DirectXMath.h>
#include"../../Math/Math.hpp"
#include"../RenderBaseShader.hpp"
#include"../../Resource/Mesh.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Math/GameArithmetic.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Resource/ConstVectorBuffer.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"

using namespace DirectX;

namespace ao
{
	struct AnimationInstance
	{
		ConstVectorBuffer<XMFLOAT4X4> mtxBuffer;
		ao::VertexShader* pVertexShader;
		ao::VertexShader* pVSMVertexShader;
		UINT bufferSize;
		StaMesh* mesh;

		size_t GetBufferSize()
		{
			return this->mtxBuffer.GetBufferSize();
		}
	};

	/*!-----------------------------------------------------------
	//	@class  RenderMeshAnimationShader
	//	@brief  メッシュレンダリングシェーダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/31
	------------------------------------------------------------*/
	class RenderMeshAnimationShader : public RenderBaseShader
	{
	public:
		RenderMeshAnimationShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~RenderMeshAnimationShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pGraphicsState		グラフィックステート
		//  @param[in]  _pRenderer			レンダラー
		//	return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		静的メッシュデータの読み込み
		//	@param[in]	_pMesh		メッシュ
		//  @param[in]  _numMesh	内部メッシュの数
		//  @return     生成された描画用のインスタンス
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		AnimationInstance* CreateAnimationInstance(BaseMesh* _pMesh,UINT _numMesh);

		/*!-----------------------------------------------------------
		//	@brief		ビュー行列更新
		//	@param[in]	_pViewMtx	ビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
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
		void UpdateLightDirection(float _x, float _y, float _z, float _ambientPower = 0.5f);

		/*!-----------------------------------------------------------
		//	@brief		視点座標更新
		//	@param[in]	_x				X座標
		//  @param[in]  _y				Y座標
		//  @param[in]  _z				Z座標
		//  @param[in]  _specularPower	スペキュラ強度
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		void UpdateEyePosition(float _x, float _y, float _z, float _specularPower = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//  @param[in]  _pInstance			インスタンス
		//	@param[in]	_transform			トランスフォーム
		//	@param[in]	_r					赤色[省略可]
		//	@param[in]	_g					緑色[省略可]
		//	@param[in]	_b					青色[省略可]
		//	@param[in]	_a					透過率[省略可]
		//  @param[in]  disableLighting		ライティングの有無[省略可]
		//	@param[in]	_isReceiveShadow	影の描画フラグ[省略可]
		//  @param[in]  _blendState			ブレンドステート[省略可]
		//  @param[in]  _fillState			描画ステート[省略可]
		//	@param[in]	_priority			レンダリング優先度[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		HRESULT	Render(
			AnimationInstance* _pInstance,
			Transform& _transform,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const BOOL _disableLighting = FALSE,
			const BOOL _isReceiveShadow = FALSE,
			const BlendState _blendState = BlendState::Alpha,
			const FillState _fillState = FillState::Solid,
			const UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		描画コマンド実行
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		void	OnDraw();

	private:
		struct COMMAND
		{
			XMFLOAT4X4 matrix;
			BlendState blendState;
			FillState fillState;
			BOOL disableLighting;
			BOOL shadowFlag;
			UINT priority;
			UINT stride;
			FLOAT color[4];
			ao::VertexShader* pUseVertexShader;
			AnimationInstance* pInstance;
		};

		struct VsConst
		{
			XMFLOAT4X4 matWVP;
			XMFLOAT4X4 matLWVP;
		};

		struct PsConst0
		{
			FLOAT diffuse[4];
			FLOAT specular[4];
			FLOAT emissive[4];
		};

		struct PsConst1
		{
			float lightDir[4];
			float eyePos[4];
			float color[4];
			int	disableLighting;
			int dummy[3];
		};

		BOOL isNeedSort;
		std::mutex mutex;
		std::vector<COMMAND>	commandList;
		PixelShader*			pPixelShader;
		PixelShader*			pVSMPixelShader;
		ConstBuffer<VsConst>	vsConst;
		ConstBuffer<PsConst0>	psConst0;
		ConstBuffer<PsConst1>	psConst1;
		std::list<AnimationInstance*> instanceList;
		XMFLOAT3 lightDir;
		XMFLOAT3 eyePos;
		XMFLOAT4X4 projMatrix;
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
		//  @param[in]  _pInstance			アニメーションインスタンス
		//	@param[in]	_isReceiveShadow	影フラグ
		//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		inline	HRESULT	MapMatrixConstBuffer(
			const XMFLOAT4X4& _mtx,
			AnimationInstance* _pInstance,
			BOOL _isReceiveShadow);

		/*!-----------------------------------------------------------
		//	@brief		マテリアル定数バッファマッピング
		//	@param[in]	_material	マテリアル
		//	@param[in]	_wRotation	ワールド回転行列
		//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		inline	HRESULT	MapMaterialConstBuffer(
			const Material& _material,
			const XMFLOAT4X4& _wRotation);
	};
}

#endif // _Include_RenderMeshAnimationShader_hpp_