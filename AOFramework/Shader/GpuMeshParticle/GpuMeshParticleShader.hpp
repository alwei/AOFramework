/*************************************************************
*	@file   	GpuMeshParticleShader.hpp
*	@brief  	GpuMeshParticleShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/01/19
*************************************************************/

#ifndef _Include_GpuMeshParticleShader_hpp_	// インクルードガード
#define _Include_GpuMeshParticleShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../RenderBaseShader.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/Mesh.hpp"
#include"../../Math/MathDefine.hpp"

namespace ao
{
	namespace GpuMeshParticle
	{
		struct InitLayout
		{
			ao::float3 pos;	// w:size
			ao::float4 color;
			ao::float3 speed;
			float maxLifeTime;
		};

		struct UpdateLayout
		{
			ao::float3 pos;
			ao::float4 color;
			ao::float3 speed;
			FLOAT nowLifeTime;
			FLOAT size;
		};

		struct Instance
		{
			u32 numBuffer;
			UavBuffer initBuffer;
			UavBuffer updateBuffer;
			UavBuffer vectorField;
			ComputeShader* pComputeShader;
			BaseTexture* pTexture;
		};
	};

	/*!-----------------------------------------------------------
	//	@class  GpuMeshParticleShader
	//	@brief  GpuMeshParticle
	//	@author	Tatsunori Aoyama
	//	@date	2015/01/19
	------------------------------------------------------------*/
	class GpuMeshParticleShader : public ao::RenderBaseShader
	{
	public:
		GpuMeshParticleShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~GpuMeshParticleShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pGraphicsState		グラフィックステート
		//  @param[in]  _pRenderer			レンダラー
		//	return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		メッシュパーティクルインスタンス作成
		//	@param[in]	_numBuffer		バッファ数
		//	@param[in]	_pMesh			発生させるメッシュ
		//	@param[in]	_texPath		テクスチャパス
		//	@param[in]	_minPos			最小座標
		//	@param[in]	_maxPos			最大座標
		//	@param[in]	_minSize		最小サイズ
		//	@param[in]	_maxSize		最大サイズ
		//	@param[in]	_minColor		最小色
		//	@param[in]	_maxColor		最大色
		//	@param[in]	_minSpeed		最小速度
		//	@param[in]	_maxSpeed		最大速度
		//	@param[in]	_minLifeTime	最小生存時間
		//	@param[in]	_maxLifeTime	最大生存時間
		//	@return		生成されたインスタンス
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		GpuMeshParticle::Instance* Create(
			const UINT _numBuffer,
			const ao::StMesh* _pMesh,
			const ao::string _texPath,
			const ao::float3& _minPos = ao::float3(-0.1f, -0.1f, -0.1f),
			const ao::float3& _maxPos = ao::float3(+0.1f, +0.1f, +0.1f),
			const float& _minSize = 0.1f, const float& _maxSize = 1.0f,
			const ao::float4& _minColor = ao::float4(1.0f, 1.0f, 1.0f, 1.0f),
			const ao::float4& _maxColor = ao::float4(1.0f, 1.0f, 1.0f, 1.0f),
			const ao::float3& _minSpeed = ao::float3(-0.2f, -0.2f, -0.2f),
			const ao::float3& _maxSpeed = ao::float3(+0.2f, +0.2f, +0.2f),
			const float& _minLifeTime = 0.5f, const float _maxLifeTime = 1.0);

		/*!-----------------------------------------------------------
		//	@brief		ビュー行列更新
		//	@param[in]	_pViewMtx	ビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		void UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_pBuffer			インスタンスバッファ
		//	@param[in]	_offset				オフセット
		//	@param[in]	_color				色
		//  @param[in]  _blendState			ブレンドステート[省略可]
		//  @param[in]  _fillState			描画ステート[省略可]
		//	@param[in]	_priority			レンダリング優先度[省略可]
		//	@return		S_OK：レンダリング成功　それ以外：レンダリング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		HRESULT	Render(
			GpuMeshParticle::Instance* _pInstance,
			const ao::float3& _offset = ao::float3(0,0,0),
			const ao::float4& _color = ao::float4(1, 1, 1, 1),
			const BlendState _blendState = BlendState::Add,
			const FillState _fillState = FillState::Solid,
			const UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		描画コマンド実行
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		void	OnDraw()override final;

	private:
		struct COMMAND
		{
			GpuMeshParticle::Instance* pInstance;
			BlendState blendState;
			FillState fillState;
			UINT priority;
			ao::float4 color;
		};

		struct CsConst
		{
			XMFLOAT4 offsetPos;
			float deltaTime;
			float dummy[3];
		};

		struct GsConst
		{
			XMFLOAT4X4 projMatrix;
			XMFLOAT4X4 viewProjMatrix;
		};

		struct PsConst
		{
			ao::float4 color;
		};

		BOOL isNeedSort;
		std::list<GpuMeshParticle::Instance*> instanceList;
		std::vector<COMMAND>	commandList;
		VertexShader*			pVertexShader;
		GeometryShader*			pGeometryShader;
		PixelShader*			pPixelShader;
		ConstBuffer<CsConst>	csConst;
		ConstBuffer<GsConst>	gsConst;
		ConstBuffer<PsConst>	psConst;
		XMFLOAT4X4 projMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 viewProjMatrix;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
	};
}

#endif // _Include_GpuMeshParticleShader_hpp_