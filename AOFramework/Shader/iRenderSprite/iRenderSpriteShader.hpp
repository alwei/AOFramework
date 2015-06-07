/*************************************************************
*	@file   	iRenderSpriteShader.hpp
*	@brief  	iRenderSpriteShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/09/29
*************************************************************/

#ifndef _Include_iRenderSpriteShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_iRenderSpriteShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"../RenderBaseShader.hpp"
#include"../../Math/Math.hpp"
#include"../../Math/GameArithmetic.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/InstanceBuffer.hpp"
#include"../../Resource/Sprite.hpp"

namespace ao
{
	class SpriteInstance
	{
	public:
		struct Instance
		{
			XMFLOAT4X4 matrix;	//�s��
			XMFLOAT4 color;		//�F
		};
		int numPushRender;
		int maxPushRender;
		Sprite* pSprite;
		Instance* pInstance;
		SmartComPtr<ID3D11Buffer> buffer;
	};

	/*!-----------------------------------------------------------
	//	@class  iRenderSpriteShader
	//	@brief  �X�v���C�g�����_���[
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/29
	------------------------------------------------------------*/
	class iRenderSpriteShader : public RenderBaseShader
	{
	public:
		iRenderSpriteShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~iRenderSpriteShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		������
		//  @param[in]  _pGraphicsState		�O���t�B�b�N�X�e�[�g
		//  @param[in]  _pRenderer			�����_���[
		//  @return     S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		�C���X�^���X�쐬
		//	@param[in]	_numInstance	�C���X�^���X��
		//  @param[in]  _pSprite		��������X�v���C�g
		//  @param[in]  _pivot			�X�v���C�g��_[�ȗ���]
		//  @return     �C���X�^���X
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		SpriteInstance* CreateInstance(UINT _numInstance, Sprite* _pSprite, SpritePivot _pivot = SpritePivot::Center);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�s��X�V
		//	@param[in]	_pViewMtx	�r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�R�}���h����
		//	@param[in]	_pInstance		�C���X�^���X
		//	@param[in]	_transform		�p��
		//	@param[in]	_r				�ԐF[�ȗ���]
		//	@param[in]	_g				�ΐF[�ȗ���]
		//	@param[in]	_b				�F[�ȗ���]
		//	@param[in]	_a				���ߗ�[�ȗ���]
		//	@return		S_OK�F�R�}���h�o�^�����@����ȊO�F�R�}���h�o�^���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		HRESULT	PushRenderCommand(
			SpriteInstance* const _pInstance,
			Transform& _transform,
			const FLOAT _r = 1.0f,
			const FLOAT _g = 1.0f,
			const FLOAT _b = 1.0f,
			const FLOAT _a = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�R�}���h���s
		//	@note		�R�}���h�o�b�t�@��
		//	@param[in]	_pInstance			�C���X�^���X
		//  @param[in]  _isBillboard		�r���{�[�h�t���O
		//	@param[in]	_blendState			�u�����h�X�e�[�g[�ȗ���]
		//  @param[in]  _fillState			�`�惂�[�h[�ȗ���]
		//  @param[in]  _priority			�v���C�I���e�B[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		HRESULT	ExecuteCommandList(
			SpriteInstance* const _pInstance,
			BlendState _blendState = BlendState::Alpha,
			const FillState _fillState = FillState::Solid,
			UINT priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		�`��R�}���h���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void	OnDraw()override final;

		/*!-----------------------------------------------------------
		//	@brief		�N���A
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void	Clear();

	private:
		struct COMMAND
		{
			UINT priority;
			UINT numRender;
			BOOL isBillboard;
			BlendState blendState;
			FillState fillState;
			SpriteInstance* pInstance;
		};

		struct PsConst
		{
			FLOAT color[4];
		};

	private:
		BOOL isNeedSort;
		std::mutex mutex;
		std::vector<COMMAND> commandList;
		std::list<SpriteInstance*>	pInstanceDataList;
		VertexShader*			pVertexShader;
		PixelShader*			pPixelShader;
		ConstBuffer<PsConst>	psConst;
		XMFLOAT4X4 projMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 viewProjMatrix;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
	};
}

#endif // _Include_iRenderSpriteShader_hpp_
