/*************************************************************
*	@file   	ZPrePassShader.hpp
*	@brief  	ZPrePassShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/09/03
*************************************************************/

#ifndef _Include_ZPrePassShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_ZPrePassShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
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
	//	@brief  Z�l�����_�����O�V�F�[�_�[
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
		//	@brief		������
		//	@param[in]	_pd3dDevice			�f�o�C�X�|�C���^
		//	@param[in]	_pd3dDeviceContext	�f�o�C�X�R���e�L�X�g�|�C���^
		//  @param[in]  _pGraphicsState		�O���t�B�b�N�X�e�[�g
		//  @param[in]  _pRenderer			�����_���[
		//	return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		�[�x�e�N�X�`���쐬
		//	@param[in]	_nWidth		�����𑜓x
		//  @param[in]  _nHeight	�c���𑜓x
		//  @param[in]  _format		�t�H�[�}�b�g[�ȗ���]
		//	@param[in]	_degree		��p[�ȗ���]
		//	@param[in]	_nZ			�j�AZ�l[�ȗ���]
		//	@param[in]	_fZ			�t�@�[Z�l[�ȗ���]
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
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
		//	@brief		�r���[�s��X�V
		//	@param[in]	_pViewMtx	�r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		void UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_pMesh			���b�V���|�C���^
		//	@param[in]	_transform		�g�����X�t�H�[��
		//	@param[in]	_priority		�����_�����O�D��x[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseMesh* const _pMesh,
			const Transform& _transform,
			UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�R�}���h���s
		//	@note		�R�}���h�o�b�t�@��
		//	@param[in]	_pBurstBuffer	�o�[�X�g�o�b�t�@
		//  @param[in]  _priority		�v���C�I���e�B[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		HRESULT	ExecuteCommandList(
			RenderBurstBuffer* const _pBurstBuffer,
			UINT priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		�v�b�V���o�b�N�I�����̕`��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/19
		------------------------------------------------------------*/
		void	OnPushbackQuit();

		/*!-----------------------------------------------------------
		//	@brief		�`��R�}���h���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		void	OnDraw();

		/*!-----------------------------------------------------------
		//	@brief		���U���g����
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/19
		------------------------------------------------------------*/
		void	OnResult();

		/*!-----------------------------------------------------------
		//	@brief		�ˉe�s��擾
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		const XMFLOAT4X4& GetProjMatrix() const;

		/*!-----------------------------------------------------------
		//	@brief		�N���A����
		//	@param[in]	_depth	�N���A����[�x�l
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/04
		------------------------------------------------------------*/
		void Clear(float _depth = 1)
		{
			this->pDepthRt->Clear(_depth, _depth, 1, 1);
			this->pZBlurRt->Clear(0, 0, 0, 1);
		}

		/*!-----------------------------------------------------------
		//	@brief		�u���[�x������ύX
		//	@param[in]	_blurPower	�u���[�p���[[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/06
		------------------------------------------------------------*/
		void SetBlurPower(float _blurPower = 0.5f)
		{
			this->blurPower = _blurPower;
		}

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�e�N�X�`���擾
		//  @param[in]  _sampler	�T���v���[�X�e�[�g[�ȗ���]
		//  @return     �����_�����O�e�N�X�`��
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
		//	@brief		�s��萔�o�b�t�@�}�b�s���O
		//	@param[in]	_mtx		�s��
		//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		inline	HRESULT	MapMatrixConstBuffer(const XMFLOAT4X4& _mtx);

		/*!-----------------------------------------------------------
		//	@brief		�P�ꃌ���_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		inline	void	OnDrawSingle();

		/*!-----------------------------------------------------------
		//	@brief		�C���X�^���X�����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		inline	void	OnDrawBurst();
	};
}

#endif // _Include_ZPrePassShader_hpp_