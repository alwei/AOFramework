/*************************************************************
*	@file   	RenderMeshShader.hpp
*	@brief  	RenderMeshShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/20
*************************************************************/

#ifndef _Include_RenderMeshShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_RenderMeshShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
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
	//	@brief  ���b�V�������_�����O�V�F�[�_�[
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
		//	@brief		������
		//	@param[in]	_pGraphicsState		�O���t�B�b�N�X�e�[�g
		//  @param[in]  _pRenderer			�����_���[
		//	return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		���C�g�p�̎ˉe�s��X�V
		//	@param[in]	_lightProjMtx	���C�g�p�̎ˉe�s��
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		void UpdateLightProjMatrix(const XMFLOAT4X4& _lightProjMtx);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�s��X�V
		//	@param[in]	_pViewMtx	�r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		void UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		���C�g�r���[�s��X�V
		//	@param[in]	_pLightViewMtx	���C�g�r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/19
		------------------------------------------------------------*/
		void UpdateLightViewMatrix(const XMFLOAT4X4* _pLightViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		���C�g�����X�V
		//	@param[in]	_x				X����
		//	@param[in]	_y				Y����
		//	@param[in]	_z				Z����
		//	@param[in]	_ambientPower	�����̋���[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/21
		------------------------------------------------------------*/
		void UpdateLightDirection(
			float _x, float _y, float _z,
			float _ambientPower=0.5f);

		/*!-----------------------------------------------------------
		//	@brief		���_���W�X�V
		//	@param[in]	_x				X���W
		//  @param[in]  _y				Y���W
		//  @param[in]  _z				Z���W
		//  @param[in]  _specularPower	�X�y�L�������x
		//	@param[in]	_specularRange	�X�y�L�����͈�
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		void UpdateEyePosition(
			float _x, float _y, float _z,
			float _specularPower = 1.0f,int _specularRange=10);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_pMesh				���b�V���|�C���^
		//	@param[in]	_transform			�g�����X�t�H�[��
		//  @param[in]  disableLighting		���C�e�B���O�̗L��[�ȗ���]
		//	@param[in]	_isReceiveShadow	�e�̕`��t���O[�ȗ���]
		//  @param[in]  _blendState			�u�����h�X�e�[�g[�ȗ���]
		//  @param[in]  _fillState			�`��X�e�[�g[�ȗ���]
		//	@param[in]	_dColor				�f�B�t���[�Y�F
		//	@param[in]	_sColor				�X�y�L�����F
		//	@param[in]	_eColor				�G�~�b�V�u�F
		//	@param[in]	_priority			�����_�����O�D��x[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
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
		//	@brief		�`��R�}���h���s
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
		//	@brief		�s��萔�o�b�t�@�}�b�s���O
		//	@param[in]	_mtx				�s��
		//	@param[in]	_material			�}�e���A��
		//	@param[in]	_isReceiveShadow	�e�t���O
		//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		HRESULT	MapConstBuffer(
			const XMFLOAT4X4& _mtx,
			BOOL _isReceiveShadow);

		/*!-----------------------------------------------------------
		//	@brief		�}�e���A���}�b�s���O
		//	@param[in]	_material	�}�e���A��
		//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		HRESULT MapMaterialBuffer(const Material& _material);
	};
}

#endif // _Include_RenderMeshShader_hpp_