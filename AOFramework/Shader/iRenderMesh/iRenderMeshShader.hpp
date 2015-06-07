/*************************************************************
*	@file   	iRenderMeshShader.hpp
*	@brief  	iRenderMeshShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/09/02
*************************************************************/

#ifndef _Include_iRenderMeshShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_iRenderMeshShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
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
	//	@brief  ���b�V���C���X�^���X�����_�����O�V�F�[�_�[
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
		//	@brief		������
		//  @param[in]  _pGraphicsState		�O���t�B�b�N�X�e�[�g
		//  @param[in]  _pRenderer			�����_���[
		//	return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* const _pGraphicsState,
			class DirectX11GraphicsRenderer* const _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�o�[�X�g�o�b�t�@�쐬
		//	@param[in]	_numInstance	�C���X�^���X��
		//  @param[in]  _pMesh			���b�V���|�C���^
		//	@return		�C���X�^���X�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		RenderBurstBuffer* CreateRenderBurstBuffer(
			const u32 _numInstance,
			BaseMesh* const _pMesh);

		/*!-----------------------------------------------------------
		//	@brief		�C���X�^���X�o�b�t�@�̃��T�C�Y
		//	@param[in]	_numInstance	���T�C�Y����C���X�^���X��
		//	@return		_pInstance		���T�C�Y����C���X�^���X
		//	@return		S_OK�F���T�C�Y�����@����ȊO�F���T�C�Y���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/17
		------------------------------------------------------------*/
		HRESULT ResizeRenderBurstBuffer(
			const u32 _numInstance,
			RenderBurstBuffer* const _pInstanceBuffer);

		/*!-----------------------------------------------------------
		//	@brief		���C�g�p�̎ˉe�s��X�V
		//	@param[in]	_lightProjMtx	���C�g�p�̎ˉe�s��
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/14
		------------------------------------------------------------*/
		void UpdateLightProjMatrix(const XMFLOAT4X4& _lightProjMtx);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�s��X�V
		//	@param[in]	_pViewMtx	�r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		void UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		���C�g�r���[�s��X�V
		//	@param[in]	_pLightViewMtx	���C�g�r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/18
		------------------------------------------------------------*/
		void UpdateLightViewMatrix(const XMFLOAT4X4* _pLightViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		���C�g�����X�V
		//	@param[in]	_x				X����
		//	@param[in]	_y				Y����
		//	@param[in]	_z				Z����
		//	@param[in]	_ambientPower	�����̋���[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		void UpdateLightDirection(float _x, float _y, float _z, float _ambientPower = 0.5f);

		/*!-----------------------------------------------------------
		//	@brief		�������W�X�V
		//	@param[in]	_x				X���W
		//  @param[in]  _y				Y���W
		//  @param[in]  _z				Z���W
		//  @param[in]  _specularPower	�X�y�L�������x[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		void UpdateEyePosition(float _x, float _y, float _z,float _specularPower=1.0f);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�R�}���h����
		//	@param[in]	_pRenderBurstBuffer	�����_�����O�o�[�X�g�o�b�t�@
		//	@param[in]	_transform			�p��
		//	@param[in]	_r					�ԐF[�ȗ���]
		//	@param[in]	_g					�ΐF[�ȗ���]
		//	@param[in]	_b					�F[�ȗ���]
		//	@param[in]	_a					���ߗ�[�ȗ���]
		//	@return		S_OK�F�R�}���h�o�^�����@����ȊO�F�R�}���h�o�^���s
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
		//	@brief		�����_�����O�R�}���h���s
		//	@note		�R�}���h�o�b�t�@��
		//	@param[in]	_pInstance			�C���X�^���X
		//	@param[in]	_renderFlags		�����_�����O�^�C�v[�ȗ���]
		//	@param[in]	_blendState			�u�����h�X�e�[�g[�ȗ���]
		//	@param[in]	_renderFlags		�����_�����O�t���O[�ȗ���]
		//  @param[in]  _blendState			�u�����h�X�e�[�g[�ȗ���]
		//  @param[in]  _fillState			�`�惂�[�h[�ȗ���]
		//  @param[in]  _priority			�v���C�I���e�B[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
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
		//	@brief		�`��R�}���h���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		void	OnDraw() override final;

		/*!-----------------------------------------------------------
		//	@brief		�N���A
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
		//	@brief		�萔�o�b�t�@�̈�m��
		//	@param[in]	_numInstance		�C���X�^���X��
		//	@param[in]	_pGpuBuffer			GPU�o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/10
		------------------------------------------------------------*/
		template <class T>
		inline HRESULT CreateConstantBuffer(
			const u32 _numInstance,
			RenderBurstBuffer::GpuBuffer<T>* const _pGpuBuffer);


		/*!-----------------------------------------------------------
		//	@brief		�萔�o�b�t�@���}�b�v
		//	@param[in]	_pGpuBuffer	GPU�o�b�t�@
		//	@param[in]	_pMapBuffer	�}�b�v����萔�o�b�t�@
		//	@param[in]	_dataSize	�f�[�^�T�C�Y
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/12
		------------------------------------------------------------*/
		template<class T>
		inline HRESULT MapConstantBuffers(RenderBurstBuffer::GpuBuffer<T>* const _pGpuBuffer);

		/*!-----------------------------------------------------------
		//	@brief		�萔�o�b�t�@�}�b�s���O
		//	@param[in]	_numSet				�}�b�v���鐔
		//	@param[in]	_pInstanceBuffer	�C���X�^���X�o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/11
		------------------------------------------------------------*/
		inline HRESULT SetConstantBuffers(const u32 _numSet, RenderBurstBuffer* const _pRenderBurstBuffer);

		/*!-----------------------------------------------------------
		//	@brief		�}�e���A���萔�o�b�t�@�}�b�s���O
		//	@param[in]	_material	�}�e���A��
		//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		inline	HRESULT	MapMaterialConstBuffer(const Material& _material);
	};
}

#endif // _Include_iRenderMeshShader_hpp_