/*************************************************************
*	@file   	CrossFilterShader.hpp
*	@brief  	CrossFilterShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/09/15
*************************************************************/

#ifndef _Include_CrossFilterShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_CrossFilterShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<array>
#include"../RenderBaseShader.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Math/MathDefine.hpp"

namespace ao
{
	enum class CrossType : int
	{
		Dual = 2,
		Quad = 4,
		Hex = 6,
		Oct = 8
	};

	enum class CrossQuality : int
	{
		Low = 2,
		Normal = 4,
		Hight = 6,
		VeryHigh = 8
	};

	/*!-----------------------------------------------------------
	//	@class  CrossFilterShader
	//	@brief  �N���X�t�B���^�[�V�F�[�_�[
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/15
	------------------------------------------------------------*/
	class CrossFilterShader : public RenderBaseShader
	{
	public:
		CrossFilterShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~CrossFilterShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	_pd3dDevice			�f�o�C�X�|�C���^
		//	@param[in]	_pd3dDeviceContext	�f�o�C�X�R���e�L�X�g�|�C���^
		//	@param[in]	_pGraphicsState		�O���t�B�b�N�X�e�[�g
		//	@param[in]	_pRenderer			�����_���[
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		�����_�[�^�[�Q�b�g�̍쐬
		//	@param[in]	_nWidth			�u���[����
		//	@param[in]	_nHeight		�u���[�c��
		//  @param[in]  _numStar		��䊂̎��[�ȗ���]
		//	@param[in]	_numStretch		�����_�����O��[�ȗ���]
		//	@param[in]	_format			�u���[�t�H�[�}�b�g[�ȗ���]
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		HRESULT	Create(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST CrossType _crossType = CrossType::Quad,
			CONST CrossQuality _numStretch = CrossQuality::Normal,
			CONST DXGI_FORMAT _format = DXGI_FORMAT_B8G8R8A8_UNORM);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_pTexture	HDR�e�N�X�`��
		//	@param[in]	_degree		��]�p�x[�ȗ���]
		//	@param[in]	_mulPower	��Z���x[�ȗ���]
		//	@param[in]	_length		����[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseTexture* _pTexture,
			int _degree = 0,
			float _mulPower=0.5f,
			float _length=1.0f);

		/*!-----------------------------------------------------------
		//	@brief		�`�惁�\�b�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		void	OnDraw();

		/*!-----------------------------------------------------------
		//	@brief		�����_�[�^�[�Q�b�g�̃N���A
		//  @param[in]  _r	�����ԗv�f[�ȗ���]
		//  @param[in]  _g	�����Ηv�f[�ȗ���]
		//  @param[in]  _b	�����v�f[�ȗ���]
		//  @param[in]  _a	�������ߗv�f[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		void	Clear(float _r = 0, float _g = 0, float _b = 0, float _a = 0);

	private:
		static const int NUM_SAMPLING = 8;

		struct VsConst
		{
			float2 uvTexel[NUM_SAMPLING];
		};

		struct PsConst
		{
			float4 colorWeight[NUM_SAMPLING];
		};

		struct PsUpSampleConst
		{
			int numStar;
			float mulPower;
			float uvOffset[2];
		};

		struct GradationColor
		{
			float4 c[NUM_SAMPLING];
		};

		struct COMMAND
		{
			BOOL isActive;
			FLOAT mulPower;
			float4 colorWeight[(int)CrossQuality::VeryHigh][NUM_SAMPLING];
			float2 uvTexel[(int)CrossType::Oct][(int)CrossQuality::VeryHigh][NUM_SAMPLING];
			std::array<BaseTexture*, (int)CrossQuality::VeryHigh+1> pTextureArray;
			std::array<BaseTexture*, (int)CrossType::Oct> pGlareTextureArray;
		};

		float2 sourceSize;
		float2 downSize;
		std::array<float2,(int)CrossQuality::VeryHigh> starAngle;

		CrossType crossType;
		CrossQuality numStretch;
		COMMAND renderCommand;

		VertexShader* pGlareVertexShader;
		PixelShader* pGlarePixelShader;
		VertexShader* pUpSampleVertexShader;
		PixelShader* pUpSamplePixelShader;
		VertexShader* pSurfaceVertexShader;
		PixelShader* pSurfacePixelShader;

		ConstBuffer<VsConst> vsConst;
		ConstBuffer<PsConst> psConst;
		ConstBuffer<PsUpSampleConst> psUpSampleConst;

		SmartComPtr<ID3D11Buffer> pSurfaceBuffer;
		DirectX11GraphicsState* pGraphicsState;
		DirectX11GraphicsRenderer* pRenderer;
		DirectX11RenderTarget* pUpSampleRenderTarget;

		std::array<BaseTexture*, (int)CrossType::Oct> nullTextures;
		std::array<GradationColor, (int)CrossQuality::VeryHigh> gradationColor;
		std::array<DirectX11RenderTarget*, (int)CrossQuality::VeryHigh-1> pStretchBuffer;
		std::array<DirectX11RenderTarget*, (int)CrossType::Oct> pReductionBuffer;

	private:
		/*!-----------------------------------------------------------
		//	@brief		��䊂̐F�K����������
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		inline	void CreateGradationColor();

		/*!-----------------------------------------------------------
		//	@brief		�F�̏d�ݒ萔�쐬
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/15
		------------------------------------------------------------*/
		inline	void CreateColorWeight();

		/*!-----------------------------------------------------------
		//	@brief		�T�[�t�F�C�X�쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		inline	HRESULT CreateSurface();
	};
}

#endif // _Include_CrossFilterShader_hpp_