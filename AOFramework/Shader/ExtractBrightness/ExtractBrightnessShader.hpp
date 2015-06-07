/*************************************************************
*	@file   	ExtractBrightnessShader.hpp
*	@brief  	ExtractBrightnessShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/09/08
*************************************************************/

#ifndef _Include_ExtractBrightnessShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_ExtractBrightnessShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<vector>
#include<mutex>
#include"../RenderBaseShader.hpp"
#include"../../Api/DirectX11RenderTarget.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/ConstBuffer.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  ExtractBrightnessShader
	//	@brief  �P�x���o�V�F�[�_�[
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/08
	------------------------------------------------------------*/
	class ExtractBrightnessShader : public RenderBaseShader
	{
	public:
		/// �R���X�g���N�^
		ExtractBrightnessShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		/// �f�X�g���N�^
		~ExtractBrightnessShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		������
		//  @param[in]  _pGraphicsState		�O���t�B�b�N�X�e�[�g
		//  @param[in]  _pRenderer			�����_���[
		//  @return     S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		���o�p�̃T�[�t�F�C�X�쐬
		//	@param[in]	_nWidth		����
		//	@param[in]	_nHeight	�c��
		//	@param[in]	_format	�t�H�[�}�b�g
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Create(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST DXGI_FORMAT _format = DXGI_FORMAT_R16G16B16A16_FLOAT);

		/*!-----------------------------------------------------------
		//	@brief		���o����
		//	@note		�V�F�[�_�[���\�[�X�r���[�w��
		//	@param[in]	_pTexture	���o���̃e�N�X�`��
		//	@param[in]	_r			���o����Ԃ̓x����[�ȗ���]
		//	@param[in]	_g			���o����΂̓x����[�ȗ���]
		//	@param[in]	_b			���o����̓x����[�ȗ���]
		//	@param[in]	_a			���o���铧���̓x����[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseTexture* _pTexture,
			const float _r,
			const float _g,
			const float _b,
			const float _a);

		/*!-----------------------------------------------------------
		//	@brief		���o����
		//	@param[in]	_r			���o����Ԃ̓x����[�ȗ���]
		//	@param[in]	_g			���o����΂̓x����[�ȗ���]
		//	@param[in]	_b			���o����̓x����[�ȗ���]
		//	@param[in]	_a			���o���铧���̓x����[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	Render(
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		�`��R�}���h���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		void	OnDraw();

		/*!-----------------------------------------------------------
		//	@brief		���o���������_�[�^�[�Q�b�g�̃N���A����
		//	@param[in]	_r	�ԐF
		//	@param[in]	_g	�ΐF
		//	@param[in]	_b	�F
		//	@param[in]	_a	���ߗ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		void	Clear(
			const float _r = 0,
			const float _g = 0,
			const float _b = 0,
			const float _a = 0);

		/*!-----------------------------------------------------------
		//	@brief		�e�N�X�`���擾
		//	@param[in]	_sampler	�T���v���[
		//  @return     �e�N�X�`��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		BaseTexture* GetBrightnessTexture(SamplerState _sampler=SamplerState::LINEAR)
		{
			return this->pExtRenderTarget->GetRenderTexture(_sampler);
		}

	private:
		struct PsConst
		{
			float sub[4];
		};

		struct COMMAND
		{
			FLOAT sub[4];
			ID3D11ShaderResourceView** ppSrView;
			ID3D11SamplerState** ppSampState;
		};

		std::vector<COMMAND> commandList;
		VertexShader* pVertexShader;
		PixelShader* pPixelShader;
		ConstBuffer<PsConst> psConst;
		SmartComPtr<ID3D11Buffer> pSurfaceBuffer;
		SmartComPtr<ID3D11ShaderResourceView> pBackBufferSrView;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
		DirectX11RenderTarget*	pExtRenderTarget;

	private:
		/*!-----------------------------------------------------------
		//	@brief		�T�[�t�F�C�X�쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		inline	HRESULT CreateSurface();
	};
}

#endif // _Include_ExtractBrightnessShader_hpp_
