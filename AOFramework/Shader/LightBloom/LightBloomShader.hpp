/*************************************************************
*	@file   	LightBloomShader.hpp
*	@brief  	LightBloomShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/09/09
*************************************************************/

#ifndef _Include_LightBloomShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_LightBloomShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<array>
#include"../RenderBaseShader.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../GaussianFilter/GaussianFilterShader.hpp"

namespace ao
{
	enum class BloomBuffer : int
	{
		Low = 2,
		Normal = 4,
		Hight = 6,
		VeryHigh = 8
	};

	/*!-----------------------------------------------------------
	//	@class  LightBloomShader
	//	@brief  ���C�g�u���[���V�F�[�_�[
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/09
	------------------------------------------------------------*/
	class LightBloomShader : public RenderBaseShader
	{
	public:
		/// �R���X�g���N�^
		LightBloomShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		/// �f�X�g���N�^
		~LightBloomShader();

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	_pGraphicsState		�O���t�B�b�N�X�e�[�g
		//	@param[in]	_pRenderer			�����_���[
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		�����_�[�^�[�Q�b�g�̍쐬
		//	@param[in]	_nWidth			�u���[����
		//	@param[in]	_nHeight		�u���[�c��
		//  @param[in]  _numBuffer		�o�b�t�@��[�ȗ���]
		//	@param[in]	_format			�u���[�t�H�[�}�b�g[�ȗ���]
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		HRESULT	Create(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST BloomBuffer _numBuffer = BloomBuffer::Normal,
			CONST DXGI_FORMAT _format = DXGI_FORMAT_B8G8R8A8_UNORM);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_pTexture		HDR�e�N�X�`��
		//  @param[in]  _dispersion		���U��[�ȗ���]
		//  @param[in]  _mulPower		��Z��[�ȗ���]
		//	@return		S_OK�F�萔�o�b�t�@MAP�����@����ȊO�F�萔�o�b�t�@MAP���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseTexture* _pTexture,
			FLOAT _dispersion=0.1f,
			FLOAT _mulPower = 0.5f);

		/*!-----------------------------------------------------------
		//	@brief		�v�b�V���o�b�N�J�n���̏���
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		void	OnPushbackEnter();

		/*!-----------------------------------------------------------
		//	@brief		�`����s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		void	OnDraw()override final;

		/*!-----------------------------------------------------------
		//	@brief		�����_�[�^�[�Q�b�g�̃N���A
		//  @param[in]  _r	�����ԗv�f[�ȗ���]
		//  @param[in]  _g	�����Ηv�f[�ȗ���]
		//  @param[in]  _b	�����v�f[�ȗ���]
		//  @param[in]  _a	�������ߗv�f[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		void	Clear(float _r = 0, float _g = 0, float _b = 0, float _a = 0);

	private:
		struct PsConst
		{
			int numBuffer;
			float mulPower;
			float uvOffset[2];
		};

		struct COMMAND
		{
			BOOL isActive;
			FLOAT dispersion;
			FLOAT mulPower;
			BaseTexture* upSampleTexture;
			std::array<BaseTexture*,(int)BloomBuffer::VeryHigh> pTextureArray;
		};

		BloomBuffer numBuffer;
		COMMAND renderCommand;
		VertexShader* pUpSampleVertexShader;
		PixelShader* pUpSamplePixelShader;
		VertexShader* pSurfaceVertexShader;
		PixelShader* pSurfacePixelShader;
		ConstBuffer<PsConst> psConst;
		SmartComPtr<ID3D11Buffer> pSurfaceBuffer;
		DirectX11GraphicsState* pGraphicsState;
		DirectX11GraphicsRenderer* pRenderer;
		DirectX11RenderTarget* pUpSampleRenderTarget;
		std::array<BaseTexture*, (int)BloomBuffer::VeryHigh> nullTextures;
		std::array<GaussianFilterShader*, (int)BloomBuffer::VeryHigh> pReductionBuffer;

	private:
		/*!-----------------------------------------------------------
		//	@brief		�T�[�t�F�C�X�쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		inline	HRESULT CreateSurface();
	};
}

#endif // _Include_LightBloomShader_hpp_