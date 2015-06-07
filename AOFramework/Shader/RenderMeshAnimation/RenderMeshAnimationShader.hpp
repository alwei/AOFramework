/*************************************************************
*	@file   	RenderMeshAnimationShader.hpp
*	@brief  	RenderMeshAnimationShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/31
*************************************************************/

#ifndef _Include_RenderMeshAnimationShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_RenderMeshAnimationShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
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
	//	@brief  ���b�V�������_�����O�V�F�[�_�[
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
		//	@brief		������
		//	@param[in]	_pGraphicsState		�O���t�B�b�N�X�e�[�g
		//  @param[in]  _pRenderer			�����_���[
		//	return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		�ÓI���b�V���f�[�^�̓ǂݍ���
		//	@param[in]	_pMesh		���b�V��
		//  @param[in]  _numMesh	�������b�V���̐�
		//  @return     �������ꂽ�`��p�̃C���X�^���X
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		AnimationInstance* CreateAnimationInstance(BaseMesh* _pMesh,UINT _numMesh);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�s��X�V
		//	@param[in]	_pViewMtx	�r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
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
		void UpdateLightDirection(float _x, float _y, float _z, float _ambientPower = 0.5f);

		/*!-----------------------------------------------------------
		//	@brief		���_���W�X�V
		//	@param[in]	_x				X���W
		//  @param[in]  _y				Y���W
		//  @param[in]  _z				Z���W
		//  @param[in]  _specularPower	�X�y�L�������x
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		void UpdateEyePosition(float _x, float _y, float _z, float _specularPower = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//  @param[in]  _pInstance			�C���X�^���X
		//	@param[in]	_transform			�g�����X�t�H�[��
		//	@param[in]	_r					�ԐF[�ȗ���]
		//	@param[in]	_g					�ΐF[�ȗ���]
		//	@param[in]	_b					�F[�ȗ���]
		//	@param[in]	_a					���ߗ�[�ȗ���]
		//  @param[in]  disableLighting		���C�e�B���O�̗L��[�ȗ���]
		//	@param[in]	_isReceiveShadow	�e�̕`��t���O[�ȗ���]
		//  @param[in]  _blendState			�u�����h�X�e�[�g[�ȗ���]
		//  @param[in]  _fillState			�`��X�e�[�g[�ȗ���]
		//	@param[in]	_priority			�����_�����O�D��x[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
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
		//	@brief		�`��R�}���h���s
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
		//	@brief		�s��萔�o�b�t�@�}�b�s���O
		//	@param[in]	_mtx				�s��
		//  @param[in]  _pInstance			�A�j���[�V�����C���X�^���X
		//	@param[in]	_isReceiveShadow	�e�t���O
		//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		inline	HRESULT	MapMatrixConstBuffer(
			const XMFLOAT4X4& _mtx,
			AnimationInstance* _pInstance,
			BOOL _isReceiveShadow);

		/*!-----------------------------------------------------------
		//	@brief		�}�e���A���萔�o�b�t�@�}�b�s���O
		//	@param[in]	_material	�}�e���A��
		//	@param[in]	_wRotation	���[���h��]�s��
		//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		inline	HRESULT	MapMaterialConstBuffer(
			const Material& _material,
			const XMFLOAT4X4& _wRotation);
	};
}

#endif // _Include_RenderMeshAnimationShader_hpp_