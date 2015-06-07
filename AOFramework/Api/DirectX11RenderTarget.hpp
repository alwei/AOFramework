/*************************************************************
*	@file   	DirectX11RenderTarget.hpp
*	@brief  	DirectX11RenderTarget�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/06/27
*************************************************************/

#ifndef _Include_DirectX11RenderTarget_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11RenderTarget_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include"DirectX11Header.hpp"
#include"DirectX11BaseApi.hpp"
#include"../Utility/SmartComPtr.hpp"
#include"../Resource/BaseTexture.hpp"
#include"../Define/RenderingDefine.hpp"
#include"Define/IODefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11RenderTarget
	//	@brief  �����_�[�^�[�Q�b�g
	//	@author	Tatsunori Aoyama
	//	@date	2014/06/27
	------------------------------------------------------------*/
	class DirectX11RenderTarget : public DirectX11BaseApi
	{
	public:
		DirectX11RenderTarget(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11RenderTarget()override final;

		/*!-----------------------------------------------------------
		//	@brief		������

		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		HRESULT	Initialize();

		/*!-----------------------------------------------------------
		//	@brief		�쐬
		//	@param[in]	_nWidth					���𑜓x
		//	@param[in]  _nHeight				�c�𑜓x
		//	@param[in]  _renderFormat			�t�H�[�}�b�g[�ȗ���]
		//	@param[in]  _miscFlag				���\�[�X�g�p�Ɋւ���I�v�V����[�ȗ���]
		//	@param[in]  _mipLevel				�~�b�v���x��[�ȗ���]
		//	@param[in]  _isCreateDepthStencil	�[�x�X�e���V���̍쐬�L��[�ȗ���]
		//	@param[in]  _depthFormat			�[�x�t�H�[�}�b�g[�ȗ���]
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		HRESULT	Create(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST DXGI_FORMAT _format = DXGI_FORMAT_UNKNOWN,
			CONST UINT _miscFlag = FALSE,
			CONST UINT _mipLevel = 1,
			CONST BOOL _isCreateDepthStencil = FALSE,
			CONST DXGI_FORMAT _depthFormat = DXGI_FORMAT_R32_TYPELESS);

		/*!-----------------------------------------------------------
		//	@brief		�����_�[�^�[�Q�b�g�̐ݒ�
		//	@param[in]  _isLogRestore		���X�g�A����ێ����邩�ǂ���
		//	@param[in]  _isSetDepthStencil	�[�x�X�e���V���̐ݒ�L��
		//	@param[in]	_pd3dDeviceContext	�g�p����f�o�C�X�R���e�L�X�g[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/23
		------------------------------------------------------------*/
		void	SetRenderTarget(
			BOOL _isLogRestore = FALSE,
			BOOL _isSetDepthStencil = TRUE,
			ID3D11DeviceContext* _pd3dDeviceContext=nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�����_�[�^�[�Q�b�g�̃��X�g�A
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/23
		------------------------------------------------------------*/
		void	RestoreRenderTarget();

		/*!-----------------------------------------------------------
		//	@brief		�N���A
		//	@param[in]	_r	�N���A�ԐF[�ȗ���]
		//	@param[in]	_g	�N���A�ΐF[�ȗ���]
		//	@param[in]	_b	�N���A�F[�ȗ���]
		//	@param[in]	_a	�N���A���ߗ�[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/11
		------------------------------------------------------------*/
		void	Clear(float _r = 0, float _g = 0, float _b = 0, float _a = 0);

		/*!-----------------------------------------------------------
		//	@brief		�摜�t�@�C���Ƃ��ĊO���ɏo��
		//	@param[in]	_filePath	�t�@�C���p�X
		//	@param[in]	_format		�o�̓t�H�[�}�b�g[�ȗ���]
		//	@return		S_OK�F�o�͐����@����ȊO�F�o�͎��s
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/24
		------------------------------------------------------------*/
		HRESULT OutputRenderTarget(AOLPCSTR _filePath,const ImageFormat _format=ImageFormat::PNG);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�|�[�g�擾
		//	@param[out]	_pOut	�o�͂����r���[�|�[�g
		//  @return     �o�͂����r���[�|�[�g
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		const D3D11_VIEWPORT* GetViewport(D3D11_VIEWPORT* const _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�e�N�X�`���̎擾
		//	@param[in]	_sampler	�w�肷��T���v���[
		//  @return     �����_�����O�e�N�X�`��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		BaseTexture* GetRenderTexture(SamplerState _sampler=SamplerState::LINEAR)
		{
			this->rtTexture.pSampState = this->renderSamplerState[(int)_sampler];
			return &this->rtTexture;
		}

		/*!-----------------------------------------------------------
		//	@brief		�[�x�e�N�X�`���̎擾
		//	@param[in]	_sampler	�w�肷��T���v���[
		//  @return     �[�x�e�N�X�`��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		BaseTexture* GetDepthTexture(SamplerState _sampler=SamplerState::LINEAR)
		{
			this->dsTexture.pSampState = this->renderSamplerState[(int)_sampler];
			return &this->dsTexture;
		}

	public:
		SmartComPtr<ID3D11Texture2D>			pRtTex;
		SmartComPtr<ID3D11RenderTargetView>		pRtView;
		SmartComPtr<ID3D11ShaderResourceView>	pRtSrView;
		SmartComPtr<ID3D11Texture2D>			pDsTex;
		SmartComPtr<ID3D11DepthStencilView>		pDsView;
		SmartComPtr<ID3D11ShaderResourceView>	pDsSrView;
		SmartComPtr<ID3D11RenderTargetView>		pLogRtView;
		SmartComPtr<ID3D11DepthStencilView>		pLogDsView;
		ID3D11DeviceContext* pLogDeviceContext;
		D3D11_VIEWPORT viewport;
		D3D11_VIEWPORT logViewport;
		BaseTexture rtTexture;
		BaseTexture dsTexture;
		SmartComPtr<ID3D11SamplerState> renderSamplerState[(int)SamplerState::MAX];

	private:
		/*!-----------------------------------------------------------
		//	@brief		�����_�[�^�[�Q�b�g�r���[�쐬
		//	@param[in]	_nWidth		�����_�[�^�[�Q�b�g�̕�
		//	@param[in]	_nHeight	�����_�[�^�[�Q�b�g�̍���
		//	@param[in]	_format		�����_�[�^�[�Q�b�g�̃t�H�[�}�b�g[�ȗ���]
		//	@param[in]	_miscFlag	���\�[�X�Ɏg�p����I�v�V����[�ȗ���]
		//	@param[in]	_mipLevel	�~�b�v���x��[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		inline	HRESULT	CreateRenderTargetView(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
			CONST UINT miscFlag = FALSE,
			CONST UINT mipLevel = 1);

		/*!-----------------------------------------------------------
		//	@brief		�[�x�X�e���V���r���[�쐬
		//	@param[in]	_nWidth		�[�x�X�e���V���̕�
		//	@param[in]	_nHeight	�[�x�X�e���V���̍���
		//	@param[in]	_format		�t�H�[�}�b�g[�ȗ���]
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		inline	HRESULT	CreateDepthStencilView(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST DXGI_FORMAT _format = DXGI_FORMAT_R8G8B8A8_UNORM);

		/*!-----------------------------------------------------------
		//	@brief		�T���v���[�X�e�[�g�쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		inline	HRESULT	CreateDefaultSamplerState();
	};
}

#endif // _Include_DirectX11RenderTarget_hpp_
