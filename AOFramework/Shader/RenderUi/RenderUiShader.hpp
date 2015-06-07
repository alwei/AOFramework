/*************************************************************
*	@file   	RenderUiShader.hpp
*	@brief  	RenderUiShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/08
*************************************************************/

#ifndef _Include_RenderUiShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_RenderUiShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<vector>
#include<mutex>
#include<DirectXMath.h>
#include"../RenderBaseShader.hpp"
#include"../../Resource/Sprite.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"

using namespace DirectX;

namespace  ao
{
	//�ˉe�s��̊�_���
	enum class SurfacePivot : int
	{
		LeftTop = 0,
		RightTop,
		LeftButtom,
		RightButtom,
		Center
	};

	/*!-----------------------------------------------------------
	//	@class  RenderUiShader
	//	@brief  UI�����_�����O�V�F�[�_�[
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/08
	------------------------------------------------------------*/
	class RenderUiShader : public RenderBaseShader
	{
	public:
		RenderUiShader(SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~RenderUiShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		������
		//  @param[in]  _pGraphicsState		�O���t�B�b�N�X�e�[�g
		//  @param[in]  _pRenderer			�����_���[
		//	return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_pSprite		�X�v���C�g�|�C���^
		//	@param[in]	_x				X���W
		//	@param[in]	_y				Y���W
		//	@param[in]	_xScale			X�X�P�[��
		//	@param[in]	_yScale			Y�X�P�[��
		//	@param[in]	_radian			���W�A��[�ȗ���]
		//	@param[in]	_r				�ԐF[�ȗ���]
		//	@param[in]	_g				�ΐF[�ȗ���]
		//	@param[in]	_b				�F[�ȗ���]
		//	@param[in]	_a				���ߗ�[�ȗ���]
		//  @param[in]  _blendState		�u�����h�X�e�[�g[�ȗ���]
		//  @param[in]  _fillState		�`�惂�[�h[�ȗ���]
		//	@param[in]	_isCache		�L���b�V���t���O[�ȗ���]
		//  @param[in]  _spritePivot	�X�v���C�g�s�{�b�g[�ȗ���]
		//	@param[in]	_surfacePivot	��ʂ̌��_���[�ȗ���]
		//	@param[in]	_priority		�����_�����O�D��x[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		HRESULT	Render(
			Sprite* const _pSprite,
			const int _x,
			const int _y,
			const float _xScale,
			const float _yScale,
			const float _rotation = 0,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const BlendState _blendState = BlendState::Alpha,
			const FillState _fillState = FillState::Solid,
			const SpritePivot _spritePivot = SpritePivot::Center,
			const SurfacePivot _surfacePivot = SurfacePivot::Center,
			const BOOL _isCache = TRUE,
			const UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_pTexture		�e�N�X�`���|�C���^
		//	@param[in]	_x				X���W
		//	@param[in]	_y				Y���W
		//	@param[in]	_xScale			X�X�P�[��
		//	@param[in]	_yScale			Y�X�P�[��
		//	@param[in]	_radian			���W�A��[�ȗ���]
		//	@param[in]	_r				�ԐF[�ȗ���]
		//	@param[in]	_g				�ΐF[�ȗ���]
		//	@param[in]	_b				�F[�ȗ���]
		//	@param[in]	_a				���ߗ�[�ȗ���]
		//  @param[in]  _blendState		�u�����h�X�e�[�g[�ȗ���]
		//	@param[in]	_isCache		�L���b�V���t���O[�ȗ���]
		//  @param[in]  _spritePivot	�X�v���C�g�s�{�b�g[�ȗ���]
		//	@param[in]	_surfacePivot	��ʂ̌��_���[�ȗ���]
		//	@param[in]	_priority		�����_�����O�D��x[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseTexture* const _pTexture,
			const int _x,
			const int _y,
			const float _xScale,
			const float _yScale,
			const float _radian = 0,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const BlendState _blendState = BlendState::Alpha,
			const FillState _fillState = FillState::Solid,
			const SpritePivot _spritePivot = SpritePivot::Center,
			const SurfacePivot _surfacePivot = SurfacePivot::Center,
			const BOOL _isCache = TRUE,
			const UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		�e�N�X�`���X���b�g���
		//	@param[in]	_priority	�D��x[�ȗ���]
		//	@return		S_OK�F��������@����ȊO�F������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/04
		------------------------------------------------------------*/
		HRESULT	ReleaseTextureSlot(UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		�`��R�}���h���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		void	OnDraw();

	private:
		struct COMMAND
		{
			int x, y;
			float sx, sy;
			float radian;
			float color[4];
			UINT priority;
			BOOL isCache;
			BOOL isCenter;
			SurfacePivot surfacePivot;
			BlendState blendState;
			FillState fillState;
			Sprite* pSprite;
			BaseTexture* pTexture;
		};

		struct VsConst
		{
			XMFLOAT4X4 matWO;
		};

		struct PsConst
		{
			FLOAT color[4];
		};

		BOOL isNeedSort;
		std::vector<COMMAND>	commandList;
		VertexShader*			pVertexShader;
		PixelShader*			pPixelShader;
		ConstBuffer<VsConst>	vsConst;
		ConstBuffer<PsConst>	psConst;
		XMFLOAT4X4				leftTopOrthoMatrix;
		XMFLOAT4X4				centerOrthoMatrix;
		Sprite					defaultLeftTopSprite;
		Sprite					defaultCenterSprite;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
		SmartComPtr<ID3D11SamplerState> pointSampler;

	private:
		/*!-----------------------------------------------------------
		//	@brief		�萔�o�b�t�@�}�b�s���O
		//	@param[in]	_command	�R�}���h
		//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		inline	HRESULT	MapConstBuffer(const COMMAND& _command);

		/*!-----------------------------------------------------------
		//	@brief		�f�t�H���g�X�v���C�g�p�̍쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		inline HRESULT	CreateDefaultSprite();

		/*!-----------------------------------------------------------
		//	@brief		�|�C���g�T���v���[�쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		inline HRESULT	CreatePointSampler();
	};
}

#endif // _Include_RenderUiShader_hpp_