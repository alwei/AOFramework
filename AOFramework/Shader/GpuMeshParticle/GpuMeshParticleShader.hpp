/*************************************************************
*	@file   	GpuMeshParticleShader.hpp
*	@brief  	GpuMeshParticleShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/01/19
*************************************************************/

#ifndef _Include_GpuMeshParticleShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_GpuMeshParticleShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
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
		//	@brief		������
		//	@param[in]	_pGraphicsState		�O���t�B�b�N�X�e�[�g
		//  @param[in]  _pRenderer			�����_���[
		//	return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		���b�V���p�[�e�B�N���C���X�^���X�쐬
		//	@param[in]	_numBuffer		�o�b�t�@��
		//	@param[in]	_pMesh			���������郁�b�V��
		//	@param[in]	_texPath		�e�N�X�`���p�X
		//	@param[in]	_minPos			�ŏ����W
		//	@param[in]	_maxPos			�ő���W
		//	@param[in]	_minSize		�ŏ��T�C�Y
		//	@param[in]	_maxSize		�ő�T�C�Y
		//	@param[in]	_minColor		�ŏ��F
		//	@param[in]	_maxColor		�ő�F
		//	@param[in]	_minSpeed		�ŏ����x
		//	@param[in]	_maxSpeed		�ő呬�x
		//	@param[in]	_minLifeTime	�ŏ���������
		//	@param[in]	_maxLifeTime	�ő吶������
		//	@return		�������ꂽ�C���X�^���X
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
		//	@brief		�r���[�s��X�V
		//	@param[in]	_pViewMtx	�r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		void UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_pBuffer			�C���X�^���X�o�b�t�@
		//	@param[in]	_offset				�I�t�Z�b�g
		//	@param[in]	_color				�F
		//  @param[in]  _blendState			�u�����h�X�e�[�g[�ȗ���]
		//  @param[in]  _fillState			�`��X�e�[�g[�ȗ���]
		//	@param[in]	_priority			�����_�����O�D��x[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
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
		//	@brief		�`��R�}���h���s
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