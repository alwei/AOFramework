/*************************************************************
*	@file   	DirectX11GraphicsState.hpp
*	@brief  	DirectX11GraphicsState�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/17
*************************************************************/

#ifndef _Include_DirectX11GraphicsState_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11GraphicsState_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<map>
#include"DirectX11BaseApi.hpp"

namespace ao
{
	enum class BlendState : int
	{
		Invalid = -1,
		Default = 0,
		Alpha,
		Add,
		Sub,
		Mul,
		Max
	};

	enum class FillState : int
	{
		Invalid = -1,
		Solid = D3D11_FILL_SOLID,
		WireFrame = D3D11_FILL_WIREFRAME
	};

	/*!-----------------------------------------------------------
	//	@class  DirectX11GraphicsState
	//	@brief  �O���t�B�b�N�X�e�[�g�Ǘ��N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/17
	------------------------------------------------------------*/
	class DirectX11GraphicsState : public DirectX11BaseApi
	{
	public:
		DirectX11GraphicsState(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11GraphicsState()override;

		/*!-----------------------------------------------------------
		//	@brief		�u�����h�X�e�[�g�ݒ�
		//	@param[in]	_blendState				�u�����h����ݒ�z����
		//	@param[in]	_numBlendState			�ݒ肷�郌���_�[�^�[�Q�b�g�̐�[�ȗ���]
		//	@param[in]	_alphaToCoverageEnable	�A���t�@�J���@���b�W�̗L����[�ȗ���]
		//	@param[in]	_isForceBind			�����I�Ȑݒ�L��[�ȗ���]
		//	@return		S_OK�F�ݒ萬���@����ȊO�F�ݒ莸�s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/17
		------------------------------------------------------------*/
		HRESULT	SetBlendState(
			const BlendState _blendState,
			const UINT _numBlendState = 1,
			const BOOL _alphaToCoverageEnable = FALSE,
			const BOOL _isForceBind = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		�[�x�X�e���V���X�e�[�g��ݒ肷��
		//	@param[in]	_isDepthEnable		�[�x�̗L������
		//	@param[in]	_depthWriteMask		�[�x�̏������ݐݒ�[�ȗ���]
		//	@param[in]	_depthFunc			�[�x�f�[�^�̔�r�ݒ�[�ȗ���]
		//	@param[in]	_isStencilEnable	�X���e���V���̗L������[�ȗ���]
		//	@param[in]	_isForceBind		�����I�Ȑݒ�L��[�ȗ���]
		//	@return		S_OK�F�ݒ萬���@����ȊO�F�ݒ莸�s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/17
		------------------------------------------------------------*/
		HRESULT	SetDepthStecilState(
			const BOOL _isEnableDepth,
			const D3D11_DEPTH_WRITE_MASK _depthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
			const D3D11_COMPARISON_FUNC _depthFunc = D3D11_COMPARISON_LESS_EQUAL,
			const BOOL _isEnableStencil = FALSE,
			const BOOL _isForceBind = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		���X�^���C�Y�X�e�[�g��ݒ肷��
		//	@param[in]	_fillMode				�`�惂�[�h[�ȗ���]
		//	@param[in]	_cullMode				�J�����O���[�h[�ȗ���]
		//	@param[in]	_isForceBind			�����o�C���h�t���O[�ȗ���]
		//	@param[in]	_frontCCW				�J�����O����[�ȗ���]
		//	@param[in]	_depthBias				�w��̃s�N�Z���ɉ��Z����[�x�l[�ȗ���]
		//	@param[in]	_depthBiasClamp			�s�N�Z���̍ő�[�x�o�C�A�X[�ȗ���]
		//	@param[in]	_slopeScaleDepthBias	�w��̃s�N�Z���̌X���ɑ΂���X�J���l[�ȗ���]
		//	@param[in]	_depthClipEnable		�����Ɋ�Â��ăN���b�s���O��L��[�ȗ���]
		//	@param[in]	_scissorEnable			�V�U�[��`�J�����O�̗L��[�ȗ���]
		//	@param[in]	_multisampleEnable		�}���`�T���v�����O�̃G�C���A�V���O�̗L��[�ȗ���]
		//	@param[in]	_antialiasedLineEnable	���̃A���`�G�C���A�V���O�̗L��[�ȗ���]
		//	@return		S_OK�F�ݒ萬���@����ȊO�F�ݒ莸�s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/03
		------------------------------------------------------------*/
		HRESULT SetRasterizerState(
			D3D11_FILL_MODE _fillMode = D3D11_FILL_SOLID,
			D3D11_CULL_MODE _cullMode = D3D11_CULL_BACK,
			BOOL _isForceBind = FALSE,
			BOOL _frontCCW = FALSE,
			INT _depthBias = 0,
			FLOAT _depthBiasClamp = 0,
			FLOAT _slopeScaleDepthBias = 0,
			BOOL _depthClipEnable = TRUE,
			BOOL _scissorEnable = FALSE,
			BOOL _multisampleEnable = FALSE,
			BOOL _antialiasedLineEnable = FALSE);

	private:
		struct NowBlendState{
			BlendState blend;
			BOOL enableAlphaToCoverage;
		};
		struct NowDepthStencilState{
			BOOL depthEnable;
			BOOL stencilEnable;
			D3D11_DEPTH_WRITE_MASK mask;
			D3D11_COMPARISON_FUNC func;
		};
		struct NowRasterizerState{
			D3D11_FILL_MODE fillMode;
			D3D11_CULL_MODE cullMode;
			BOOL frontCounterClockwise;
			INT depthBias;
			INT depthBiasClamp;
			INT slopeScaledDepthBias;
			BOOL depthClipEnable;
			BOOL scissorEnable;
			BOOL multisampleEnable;
			BOOL antialiasedLineEnable;
		};

		NowBlendState nowBlendState;
		NowDepthStencilState nowDepthStencilState;
		NowRasterizerState nowRasterizerState;

		typedef std::map<USHORT, ID3D11BlendState*> blend_map;
		typedef std::map<USHORT, ID3D11DepthStencilState*> depthStencil_map;
		typedef std::map<DWORD, ID3D11RasterizerState*> rasterizer_map;
		blend_map	pBlendStateMap;
		depthStencil_map pDepthStencilMap;
		rasterizer_map pRasterizerMap;

	private:
		/*!-----------------------------------------------------------
		//	@brief		�u�����h�X�e�[�g�̍쐬
		//	@param[in]	_blendState				�쐬����u�����h�X�e�[�g
		//	@param[in]	_numBlendState			�쐬����ۂɁA�ݒ肷��u�����h�X�e�[�g�̐�[�ȗ���]
		//	@param[in]	_alphaToCoverageEnable	���J�o���b�W�̗L������[�ȗ���]
		//	@return		�u�����h�X�e�[�g�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/17
		------------------------------------------------------------*/
		inline	ID3D11BlendState*	CreateBlendState(
			const BlendState _blendState,
			const UINT _numBlendState = 1,
			const BOOL _alphaToCocerageEnable = FALSE);


		/*!-----------------------------------------------------------
		//	@brief		�[�x�X�e���V���̍쐬
		//	@param[in]	_isDepthEnable		�[�x�̗L������
		//	@param[in]	_depthWriteMask		�[�x�̏������ݐݒ�[�ȗ���]
		//	@param[in]	_depthFunc			�[�x�f�[�^�̔�r�ݒ�[�ȗ���]
		//	@param[in]	_isStencilEnable	�X���e���V���̗L������[�ȗ���]
		//	@return		�[�x�X�e���V���X�e�[�g�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/17
		------------------------------------------------------------*/
		inline	ID3D11DepthStencilState* CreateDepthStencilState(
			const BOOL _isEnableDepth,
			const D3D11_DEPTH_WRITE_MASK _depthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
			const D3D11_COMPARISON_FUNC _depthFunc = D3D11_COMPARISON_LESS_EQUAL,
			const BOOL _isEnableStencil = FALSE);


		/*!-----------------------------------------------------------
		//	@brief		���X�^���C�Y�X�e�[�g�̍쐬
		//	@param[in]	_fillMode				�`�惂�[�h
		//	@param[in]	_cullMode				�J�����O���[�h
		//	@param[in]	_frontCCW				�J�����O����
		//	@param[in]	_depthBias				�w��̃s�N�Z���ɉ��Z����[�x�l
		//	@param[in]	_depthBiasClamp			�s�N�Z���̍ő�[�x�o�C�A�X
		//	@param[in]	_slopeScaleDepthBias	�w��̃s�N�Z���̌X���ɑ΂���X�J���l
		//	@param[in]	_depthClipEnable		�����Ɋ�Â��ăN���b�s���O��L��
		//	@param[in]	_scissorEnable			�V�U�[��`�J�����O�̗L��
		//	@param[in]	_multisampleEnable		�}���`�T���v�����O�̃G�C���A�V���O�̗L��
		//	@param[in]	_antialiasedLineEnable	���̃A���`�G�C���A�V���O�̗L��
		//	@return		���X�^���C�Y�X�e�[�g�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/03
		------------------------------------------------------------*/
		inline	ID3D11RasterizerState*	CreateRasterizerState(
			D3D11_FILL_MODE _fillMode,
			D3D11_CULL_MODE _cullMode,
			BOOL _frontCCW,
			INT _depthBias,
			FLOAT _depthBiasClamp,
			FLOAT _slopeScaleDepthBias,
			BOOL _depthClipEnable,
			BOOL _scissorEnable,
			BOOL _multisampleEnable,
			BOOL _antialiasedLineEnable);
	};
}

#endif // _Include_DirectX11GraphicsState_hpp_