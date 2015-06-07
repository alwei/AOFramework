/*************************************************************
*	@file   	SSAOShader.hpp
*	@brief  	SSAOShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/01/19
*************************************************************/

#ifndef _Include_SSAOShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_SSAOShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"../RenderBaseShader.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Resource/ShaderResource.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  SSAOShader
	//	@brief  SSAO(�s���S�j
	//	@author	Tatsunori Aoyama
	//	@date	2015/01/19
	------------------------------------------------------------*/
	class SSAOShader : public RenderBaseShader
	{
	public:
		SSAOShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~SSAOShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	_pGraphicsState		�O���t�B�b�N�X�e�[�g
		//	@param[in]	_pRenderer			�����_���[
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		�����_�[�^�[�Q�b�g�̍쐬
		//	@param[in]	_pRenderTarget	AA�����_�����O��
		//	@param[in]	_format			�u���[�t�H�[�}�b�g[�ȗ���]
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		HRESULT	Create(DirectX11RenderTarget* const _pRenderTarget);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_pTexture		�e�N�X�`��
		//	@return		S_OK�F�o�^�����@����ȊO�F�o�^���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		HRESULT	Render(BaseTexture* _pTexture);

		/*!-----------------------------------------------------------
		//	@brief		�`����s
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		void	OnDraw()override final;

	private:
		struct COMMAND
		{
			BaseTexture* renderTexture;
			DirectX11RenderTarget* pRenderTarget;
		};

	private:
		COMMAND renderCommand;
		SmartComPtr<ID3D11Buffer> pSurfaceBuffer;
		DirectX11GraphicsState* pGraphicsState;
		DirectX11GraphicsRenderer* pRenderer;
		VertexShader* pVertexShader;
		PixelShader* pPixelShader;

	private:
		/*!-----------------------------------------------------------
		//	@brief		�T�[�t�F�C�X�쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/19
		------------------------------------------------------------*/
		HRESULT CreateSurface();
	};
}

#endif // _Include_SSAOShader_hpp_