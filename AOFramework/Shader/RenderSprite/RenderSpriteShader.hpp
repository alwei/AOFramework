/*************************************************************
*	@file   	RenderSpriteShader.hpp
*	@brief  	RenderSpriteShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/02
*************************************************************/

#ifndef _Include_RenderSpriteShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_RenderSpriteShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<vector>
#include<mutex>
#include<DirectXMath.h>
#include"../RenderBaseShader.hpp"
#include"../../Math/Math.hpp"
#include"../../Math/GameArithmetic.hpp"
#include"../../Resource/Sprite.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"

using namespace DirectX;

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  RenderSpriteShader
	//	@brief  ���b�V�������_�����O�V�F�[�_�[
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/02
	------------------------------------------------------------*/
	class RenderSpriteShader : public RenderBaseShader
	{
	public:
		RenderSpriteShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~RenderSpriteShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	_pGraphicsState		�O���t�B�b�N�X�e�[�g
		//  @param[in]  _pRenderer			�����_���[
		//	return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/02
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�s��X�V
		//	@param[in]	_pViewMtx	�r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/02
		------------------------------------------------------------*/
		void UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_pSprite			�X�v���C�g�|�C���^
		//	@param[in]	_transform			�g�����X�t�H�[��
		//	@param[in]	_r					�ԐF[�ȗ���]
		//	@param[in]	_g					�ΐF[�ȗ���]
		//	@param[in]	_b					�F[�ȗ���]
		//	@param[in]	_a					���ߗ�[�ȗ���]
		//  @param[in]  _blendState			�u�����h�X�e�[�g[�ȗ���]
		//	@param[in]	_isCulling			�J�����O�t���O[�ȗ���]
		//	@param[in]	_fillMode			�`�惂�[�h[�ȗ���]
		//	@param[in]	_priority			�����_�����O�D��x[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/02
		------------------------------------------------------------*/
		HRESULT	Render(
			Sprite* const _pSprite,
			Transform& _transform,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const BlendState _blendState = BlendState::Alpha,
			const BOOL _isCullling = TRUE,
			const FillState _fillMode = FillState::Solid,
			const UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_pTexture			�e�N�X�`���|�C���^
		//	@param[in]	_transform			�g�����X�t�H�[��
		//	@param[in]	_r					�ԐF[�ȗ���]
		//	@param[in]	_g					�ΐF[�ȗ���]
		//	@param[in]	_b					�F[�ȗ���]
		//	@param[in]	_a					���ߗ�[�ȗ���]
		//  @param[in]  _blendState			�u�����h�X�e�[�g[�ȗ���]
		//	@param[in]	_isCulling			�J�����O�t���O[�ȗ���]
		//	@param[in]	_fillMode			�`�惂�[�h[�ȗ���]
		//	@param[in]	_priority			�����_�����O�D��x[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/03
		------------------------------------------------------------*/
		HRESULT	Render(
			BaseTexture* const _pTexture,
			Transform& _transform,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const BlendState _blendState = BlendState::Alpha,
			const BOOL _isCullling = TRUE,
			const FillState _fillMode = FillState::Solid,
			const UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		�`��R�}���h���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/02
		------------------------------------------------------------*/
		void	OnDraw()override final;

	private:
		struct COMMAND
		{
			XMFLOAT4X4 matrix;
			BlendState blendState;
			D3D11_FILL_MODE fillMode;
			D3D11_CULL_MODE cullMode;
			UINT priority;
			FLOAT color[4];
			Sprite* pSprite;
		};

		struct VsConst
		{
			XMFLOAT4X4 matWVP;
		};

		struct PsConst0
		{
			float color[4];
		};

		BOOL isNeedSort;
		Sprite defaultLeftTopSprite;
		Sprite defaultCenterSprite;
		std::vector<COMMAND>	commandList;
		VertexShader*			pVertexShader;
		PixelShader*			pPixelShader;
		ConstBuffer<VsConst>	vsConst;
		ConstBuffer<PsConst0>	psConst0;
		XMFLOAT4X4 projMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 viewProjMatrix;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;

	private:
		/*!-----------------------------------------------------------
		//	@brief		�s��萔�o�b�t�@�}�b�s���O
		//	@param[in]	_mtx				�s��
		//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/02
		------------------------------------------------------------*/
		inline	HRESULT	MapMatrixConstBuffer(const XMFLOAT4X4& _mtx);
	};
}

#endif // _Include_RenderSpriteShader_hpp_