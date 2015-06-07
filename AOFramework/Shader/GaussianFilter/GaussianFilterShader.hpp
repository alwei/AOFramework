/*************************************************************
*	@file   	GaussianFilterShader.hpp
*	@brief  	GaussianFilterShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2013/09/04
*************************************************************/

#ifndef _Include_GaussianFilterShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_GaussianFilterShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"../RenderBaseShader.hpp"
#include"../../Utility/SmartComPtr.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Api/DirectX11Shader.hpp"
#include"../../Api/DirectX11RenderTarget.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  GaussianFilterShader
	//	@brief  �K�E�X�t�B���^�[�V�F�[�_�[�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/08
	------------------------------------------------------------*/
	class GaussianFilterShader : public RenderBaseShader
	{
	public:
		/// �R���X�g���N�^
		GaussianFilterShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		/// �f�X�g���N�^
		~GaussianFilterShader();

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	_pGraphicsState		�O���t�B�b�N�X�e�[�g
		//	@param[in]	_pRenderer			�����_���[
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		�����_�[�^�[�Q�b�g�̍쐬
		//	@param[in]	_nBlurWidth		�u���[����
		//	@param[in]	_nBlurHeight	�u���[�c��
		//	@param[in]	_blurFormat		�u���[�t�H�[�}�b�g[�ȗ���]
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Create(
			CONST UINT _nBlurWidth,
			CONST UINT _nBlurHeight,
			CONST DXGI_FORMAT _blurFormat = DXGI_FORMAT_B8G8R8A8_UNORM);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_pTexture		HDR�e�N�X�`��
		//  @param[in]  _dispersion		���U��[�ȗ���]
		//  @param[in]  _mulPower		��Z��[�ȗ���]
		//	@return		S_OK�F�萔�o�b�t�@MAP�����@����ȊO�F�萔�o�b�t�@MAP���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseTexture* _pTexture,
			FLOAT _dispersion = 0.1f,
			FLOAT _mulPower = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		�`����s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		void	OnDraw();

		/*!-----------------------------------------------------------
		//	@brief		�u���[�ς݃e�N�X�`���擾
		//	@param[in]	_sampler	�T���v���[
		//  @return     �e�N�X�`��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		BaseTexture* GetBlurTexture(SamplerState _sampler = SamplerState::LINEAR)
		{
			return this->pBlurRenderTarget[1]->GetRenderTexture(_sampler);
		}

		/*!-----------------------------------------------------------
		//	@brief		�����_�[�^�[�Q�b�g�̃N���A
		//  @param[in]  _r	�����ԗv�f[�ȗ���]
		//  @param[in]  _g	�����Ηv�f[�ȗ���]
		//  @param[in]  _b	�����v�f[�ȗ���]
		//  @param[in]  _a	�������ߗv�f[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		void	Clear(float _r=0, float _g=0,float _b=0,float _a=0);

	private:
		struct VsConst
		{
			float	width;
			float	height;
			float	power[2];
		};

		struct PsConst
		{
			float	weight[8];
			float	offset[2];
			float	power[2];
		};

		struct COMMAND
		{
			BOOL isActive;
			FLOAT dispersion;
			FLOAT mulPower;
			ID3D11ShaderResourceView** ppSrView;
			ID3D11SamplerState** ppSampState;
		};

	private:
		COMMAND renderCommand;
		ConstBuffer<VsConst> vsConst;
		ConstBuffer<PsConst> psConst;
		VertexShader*	pXBlurVertexShader;
		PixelShader*	pXBlurPixelShader;
		VertexShader*	pYBlurVertexShader;
		PixelShader*	pYBlurPixelShader;
		SmartComPtr<ID3D11Buffer>	pSurfaceBuffer;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
		DirectX11RenderTarget*		pBlurRenderTarget[2];

	private:
		/*!-----------------------------------------------------------
		//	@brief		�T�[�t�F�C�X�쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		inline	HRESULT CreateSurface();

		/*!-----------------------------------------------------------
		//	@brief		�萔�o�b�t�@�̍X�V�}�b�v
		//	@param[in]	_dispersion	���U��
		//	@param[in]	_mulPower	��Z���x[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		inline	void	UpdateMap(float _dispersion, float _mulPower = 1.0f);
	};
}

#endif // _Include_GaussianFilterShader_hpp_
