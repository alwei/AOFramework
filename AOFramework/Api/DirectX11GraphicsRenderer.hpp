/*************************************************************
*	@file   	DirectX11GraphicsRenderer.hpp
*	@brief  	DirectX11GraphicsRenderer�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/10
*************************************************************/

#ifndef _Include_DirectX11GraphicsRenderer_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11GraphicsRenderer_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<list>
#include<array>
#include<vector>
#include<mutex>
#include<DirectXMath.h>
#include"DirectX11BaseApi.hpp"
#include"../Math/GameArithmetic.hpp"
#include"../Resource/BaseTexture.hpp"
#include"../Shader/RenderBaseShader.hpp"
#include"../Define/RenderingDefine.hpp"
#include"../Resource/ConstBuffer.hpp"
#include"DirectX11RenderTarget.hpp"

using namespace  DirectX;

namespace ao
{
	class DirectX11GraphicsState;

	struct AxisData
	{
		union
		{
			struct
			{
				float sx, sy, sz;
				float sr, sg, sb, sa;
			};
			struct{ float fs[3]; };
		};
		union
		{
			struct
			{
				float ex, ey, ez;
				float er, eg, eb, ea;
			};
			struct{ float fe[3]; };
		};
		AxisData() :sr(1), sg(1), sb(1), sa(1), er(1), eg(1), eb(1), ea(1){}
	};

	/*!-----------------------------------------------------------
	//	@class  DirectX11GraphicsRenderer
	//	@brief  �O���t�B�b�N�����_���[
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/10
	------------------------------------------------------------*/
	class DirectX11GraphicsRenderer : public DirectX11BaseApi
	{
	public:
		DirectX11GraphicsRenderer(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11GraphicsRenderer()override;

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	_pSwapChain			�X���b�v�`�F�C��
		//	@param[in]	_pBackBuffer		�o�b�N�o�b�t�@�|�C���^
		//  @param[in]  _pGraphicsState		�O���t�B�b�N�X�e�[�g
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		HRESULT	Initialize(
			SmartComPtr<IDXGISwapChain> _pSwapChain,
			DirectX11RenderTarget* _pBackBuffer,
			DirectX11GraphicsState* _pGraphicsState);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�J�n
		//	@param[in]	_r	�N���A�ԐF[�ȗ���]
		//	@param[in]	_g	�N���A�ΐF[�ȗ���]
		//	@param[in]	_b	�N���A�F[�ȗ���]
		//	@param[in]	_a	�N���A���ߗ�[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/11
		------------------------------------------------------------*/
		void	RenderBegin(float _r = 0, float _g = 0, float _b = 0, float _a = 0);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�I��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/11
		------------------------------------------------------------*/
		void	RenderEnd();

		/*!-----------------------------------------------------------
		//	@brief		�e�N�X�`���ݒ�
		//	@param[in]	_srSlot			�ݒ肷��X���b�g
		//	@param[in]	_sampSlots		�ݒ肷��T���v���[�X���b�g
		//	@param[in]	_pTexture		�ݒ肷��e�N�X�`��
		//	@param[in]	_isCache		�L���b�V���t���O[�ȗ���]
		//	@param[in]	_isForceBind	�����I��Bind���邩�ǂ���[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		void	SetTexture(
			int _srSlot,
			int _sampSlot,
			BaseTexture* _pTexture,
			BOOL _isCache = TRUE,
			BOOL _isForceBind = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		�e�N�X�`���ݒ�
		//	@param[in]	_srSlot			�ݒ肷��X���b�g
		//	@param[in]	_sampSlots		�ݒ肷��T���v���[�X���b�g
		//  @param[in]  _ppSrView		�ݒ肷��V�F�[�_�[���\�[�X�r���[[�ȗ���]
		//  @param[in]  _ppSampler		�ݒ肷��T���v���[�X�e�[�g[�ȗ���]
		//	@param[in]	_isCache		�L���b�V���t���O[�ȗ���]
		//	@param[in]	_isForceBind	�����I��Bind���邩�ǂ���[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		void	SetTexture(
			int _srSlot,
			int _sampSlot,
			ID3D11ShaderResourceView** _ppSrView = nullptr,
			ID3D11SamplerState** _ppSampler = nullptr,
			BOOL _isCache = TRUE,
			BOOL _isForceBind = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		�e�N�X�`���ݒ�
		//	@param[in]	_num			�ݒ肷�鐔
		//	@param[in]	_srSlotStart	�ݒ肷��J�n�X���b�g
		//	@param[in]	_sampSlotStart	�ݒ肷��J�n�T���v���[�X���b�g
		//	@param[in]	_pTextures		�ݒ肷��e�N�X�`���z��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		void	SetTexture(
			int num,
			int _srSlotStart,
			int _sampSlotStart,
			BaseTexture** _pTextures);

		/*!-----------------------------------------------------------
		//	@brief		���_�e�N�X�`���ݒ�
		//	@param[in]	_srSlot			�ݒ肷��X���b�g
		//	@param[in]	_sampSlots		�ݒ肷��T���v���[�X���b�g
		//	@param[in]	_pTexture		�ݒ肷��e�N�X�`��
		//	@param[in]	_isCache		�L���b�V���t���O[�ȗ���]
		//	@param[in]	_isForceBind	�����I��Bind���邩�ǂ���[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/06
		------------------------------------------------------------*/
		void	SetVertexTexture(
			int _srSlot,
			int _sampSlot,
			BaseTexture* _pTexture,
			BOOL _isCache = TRUE,
			BOOL _isForceBind = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		���_�e�N�X�`���ݒ�
		//	@param[in]	_srSlot			�ݒ肷��X���b�g
		//	@param[in]	_sampSlots		�ݒ肷��T���v���[�X���b�g
		//  @param[in]  _ppSrView		�ݒ肷��V�F�[�_�[���\�[�X�r���[[�ȗ���]
		//  @param[in]  _ppSampler		�ݒ肷��T���v���[�X�e�[�g[�ȗ���]
		//	@param[in]	_isCache		�L���b�V���t���O[�ȗ���]
		//	@param[in]	_isForceBind	�����I��Bind���邩�ǂ���[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/06
		------------------------------------------------------------*/
		void	SetVertexTexture(
			int _srSlot,
			int _sampSlot,
			ID3D11ShaderResourceView** _ppSrView = nullptr,
			ID3D11SamplerState** _ppSampler = nullptr,
			BOOL _isCache = TRUE,
			BOOL _isForceBind = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		���_�e�N�X�`���ݒ�
		//	@param[in]	_num			�ݒ肷�鐔
		//	@param[in]	_srSlotStart	�ݒ肷��J�n�X���b�g
		//	@param[in]	_sampSlotStart	�ݒ肷��J�n�T���v���[�X���b�g
		//	@param[in]	_pTextures		�ݒ肷��e�N�X�`���z��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/06
		------------------------------------------------------------*/
		void	SetVertexTexture(
			int num,
			int _srSlotStart,
			int _sampSlotStart,
			BaseTexture** _pTextures);

		/*!-----------------------------------------------------------
		//	@brief		�v�������_�����O�o�^
		//	@param[in]	_pNode		�o�^�����_���[�m�[�h
		//  @param[in]  _priority	�����_�����O�D��x[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		void	PreRenderPushback(RenderBaseShader* _pNode, UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�o�^
		//	@param[in]	_pNode		�o�^�����_���[�m�[�h
		//	@param[in]	_priority	�����_�����O�D��x[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		void	RenderPushback(RenderBaseShader* _pNode, UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		�|�X�g�����_�����O�o�^
		//	@param[in]	_pNode		�o�^�����_���[�m�[�h
		//	@param[in]	_priority	�����_�����O�D��x[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		void	PostRenderPushback(RenderBaseShader* _pNode, UINT _priority = 0xCCCCCCC);

		/*!-----------------------------------------------------------
		//	@brief		�v���o�[�X�g�����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		HRESULT PreRenedrBurst();

		/*!-----------------------------------------------------------
		//	@brief		�o�[�X�g�����_�����O
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		HRESULT	RenderBurst();

		/*!-----------------------------------------------------------
		//	@brief		�|�X�g�o�[�X�g�����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/08
		------------------------------------------------------------*/
		HRESULT	PostRenderBurst();

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O��S�Ĕj��
		//	@author		Tatsunori Aoyama
		//	@date		2014/�O�X/15
		------------------------------------------------------------*/
		void	ClearRender();

		/*!-----------------------------------------------------------
		//	@brief		���`��
		//	@param[in]	_sx			�J�nX���W
		//	@param[in]	_sy			�J�nY���W
		//	@param[in]	_sz			�J�nZ���W
		//	@param[in]	_ex			�I�[X���W
		//	@param[in]	_ey			�I�[Y���W
		//	@param[in]	_ez			�I�[Z���W
		//	@param[in]	_r			�ԐF[�ȗ���]
		//	@param[in]	_g			�ΐF[�ȗ���]
		//	@param[in]	_b			�F[�ȗ���]
		//	@param[in]	_a			���ߗ�[�ȗ���]
		//	@param[in]	_pOrigin	���[���h�ϊ�����s��[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/15
		------------------------------------------------------------*/
		HRESULT	RenderAxis(
			const float _sx,
			const float _sy,
			const float _sz,
			const float _ex,
			const float _ey,
			const float _ez,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const XMFLOAT4X4* _pOrigin = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		���`��̃o�[�X�g
		//	@note		���炭���d
		//	@param[in]	_axisData	���f�[�^
		//	@param[in]	_num		���̖{��
		//	@param[in]	_r			�ԐF[�ȗ���]
		//	@param[in]	_g			�ΐF[�ȗ���]
		//	@param[in]	_b			�F[�ȗ���]
		//	@param[in]	_a			���ߗ�[�ȗ���]
		//	@param[in]	_pOrigin	���[���h�ϊ�����s��[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/07
		------------------------------------------------------------*/
		HRESULT	RenderAxis(
			const AxisData* _axisData,
			const int _num,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const XMFLOAT4X4* _pOrigin = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		���`��
		//	@note		���_�o�b�t�@���}�j���A���w��
		//	@param[in]	_pVB		�}�b�v���钸�_�o�b�t�@
		//	@param[in]	_numRender	�����_�����O���鎲�{��
		//	@param[in]	_r			�S�̂̐ԐF[�ȗ���]
		//	@param[in]	_g			�S�̗̂ΐF[�ȗ���]
		//	@param[in]	_b			�S�̂̐F[�ȗ���]
		//	@param[in]	_a			�S�̂̓��ߗ�[�ȗ���]
		//	@param[in]	_pOrigin	���[���h�ϊ�����s��[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/15
		------------------------------------------------------------*/
		HRESULT	RenderAxis(
			ID3D11Buffer* _pVB,
			const int _numRender,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f,
			const XMFLOAT4X4* _pOrigin = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�p�����`��
		//	@param[in]	_pose	�p��
		//  @param[in]  _length	����[�ȗ���]
		//	@param[in]	_r		�ԐF[�ȗ���]
		//	@param[in]	_g		�ΐF[�ȗ���]
		//	@param[in]	_b		�F[�ȗ���]
		//	@param[in]	_a		���ߗ�[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/08
		------------------------------------------------------------*/
		HRESULT	RenderPose(
			Transform& _pose,
			const float _length = 2.0f,
			float _r = 1.0f,
			float _g = 1.0f,
			float _b = 1.0f,
			float _a = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		�O���b�h�`��
		//	@param[in]	_x		X���W[�ȗ���]
		//	@param[in]	_y		Y���W[�ȗ���]
		//	@param[in]	_z		Z���W[�ȗ���]
		//	@param[in]	_num	�O���b�h��[�ȗ���]
		//	@param[in]	_space	�Ԋu[�ȗ���]
		//	@param[in]	_r		�ԐF[�ȗ���]
		//	@param[in]	_g		�ΐF[�ȗ���]
		//	@param[in]	_b		�F[�ȗ���]
		//	@param[in]	_a		���ߗ�[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/14
		------------------------------------------------------------*/
		HRESULT RenderGrid(
			const float _x = 0.0f,
			const float _y = 0.0f,
			const float _z = 0.0f,
			const int _num = 10,
			const float _space = 5.0f,
			const float _r = 1.0f,
			const float _g = 1.0f,
			const float _b = 1.0f,
			const float _a = 1.0f);


		/*!-----------------------------------------------------------
		//	@brief		��ʏ�փX�v���C�g�̊ȈՃ����_�����O
		//	@param[in]	_x			X���W
		//	@param[in]	_y			Y���W
		//	@param[in]	_width		����
		//	@param[in]	_height		�c��
		//	@param[in]	_color		�F[�ȗ���]
		//	@param[in]	_pTexture	�e�N�X�`��[�ȗ���]
		//	@param[in]	_sampType	�T���v���[�^�C�v[�ȗ���]
		//	@param[in]	_orthoType	���ˉe�s��̃^�C�v[�ȗ���]
		//	@return		S_OK�F�����_�����O�����@����ȊO�F�����_�����O���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/20
		------------------------------------------------------------*/
		HRESULT RenderSprite(
			const float _x,
			const float _y,
			const float _width,
			const float _height,
			const color _color = color::white,
			BaseTexture* const _pTexture = nullptr,
			const SamplerState _sampType = SamplerState::POINT,
			const OrthoType _orthoType = OrthoType::LeftTop);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�s��̍X�V
		//	@param[in]	_viewMtx	�r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/15
		------------------------------------------------------------*/
		void	UpdateViewMatrix(const XMFLOAT4X4* _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		���ˉe�s��擾(���㌴�_)
		//	@param[out]	_pOut	�o�͂��鐳�ˉe�s��
		//	@return		�o�͂���ˉe�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		const XMFLOAT4X4*	GetLeftTopOrthoMatrix(XMFLOAT4X4* _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		���ˉe�s��擾(���S���_)
		//	@param[out]	_pOut	�o�͂��鐳�ˉe�s��
		//	@return		�o�͂���ˉe�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		const XMFLOAT4X4*	GetCenterOrthoMatrix(XMFLOAT4X4* _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�ˉe�s��擾
		//	@param[out]	_pOut	�o�͂���ˉe�s��
		//	@return		�o�͂���ˉe�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		const XMFLOAT4X4*	GetProjMatrix(XMFLOAT4X4* _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�s��擾
		//	@param[out]	_pOut	�o�͂���r���[�s��
		//	@return		�o�͂���r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		const XMFLOAT4X4*	GetViewMatrix(XMFLOAT4X4* _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�s��擾
		//	@param[out]	_pOut	�o�͂���r���[�ˉe�s��
		//	@return		�o�͂���r���[�ˉe�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/17
		------------------------------------------------------------*/
		const XMFLOAT4X4*	GetViewProjMatrix(XMFLOAT4X4* _pOut = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�o�b�N�o�b�t�@�r���[�|�[�g�擾
		//	@param[out]	_pOut	�o�͂���r���[�|�[�g
		//  @return     �o�͂���r���[�|�[�g
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		const D3D11_VIEWPORT* GetViewport(D3D11_VIEWPORT* _pOut)
		{
			return this->pRenderTarget->GetViewport(_pOut);
		}

		/*!-----------------------------------------------------------
		//	@brief		���C���̃����_�[�^�[�Q�b�g���擾
		//	@return		���C���̃����_�[�^�[�Q�b�g�e�N�X�`��
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/22
		------------------------------------------------------------*/
		BaseTexture* GetMainTexture()
		{
			return this->pRenderTarget->GetRenderTexture();
		}

	private:
		static const u32 MAX_GRID = 3000;
		static const u32 MAX_GRID_BUFFER = (MAX_GRID * 2 + 1) * 2;
		static const u32 MAX_TEXTURE = 128;

		struct GridBuffer
		{
			struct Vertex
			{
				float x, y, z;
				float r, g, b, a;
				Vertex() :r(1), g(1), b(1), a(1){}
			};
			std::array<Vertex, MAX_GRID_BUFFER > vertex;
			SmartComPtr<ID3D11Buffer> buffer;
		};

		template<u32 NUM>
		struct AxisBuffer
		{
			struct Vertex
			{
				float x, y, z;
				float r, g, b, a;
				Vertex() :r(1), g(1), b(1), a(1){}
			};
			std::array<Vertex, NUM> vertex;
			SmartComPtr<ID3D11Buffer> buffer;
		};

		struct ShaderPKG
		{
			SmartComPtr<ID3D11VertexShader> vertex;
			SmartComPtr<ID3D11InputLayout> inputLayout;
			SmartComPtr<ID3D11PixelShader> pixel;
		};

		struct GridConstBuffer
		{
			struct VsConst
			{
				DirectX::XMFLOAT4X4	matWVP;
			};
			struct PsConst
			{
				float color[4];
			};
			VsConst rawVsBuffer;
			PsConst rawPsBuffer;
			SmartComPtr<ID3D11Buffer> vsBuffer;
			SmartComPtr<ID3D11Buffer> psBuffer;
		};


		struct SpriteConstBuffer
		{
			matrix matWP;
			color color;
		};

		struct RenderNode
		{
			RenderBaseShader* shader;
			UINT priority;
		};

		struct Projection
		{
			float fovAngleY;
			float aspect;
			float nearZ;
			float farZ;
		};

	private:
		BOOL isNeedSortRenderList;
		BOOL isNeedSortPreRenderList;
		BOOL isNeedSortPostRenderList;
		SmartComPtr<IDXGISwapChain> pSwapChain;
		DirectX11RenderTarget* pRenderTarget;
		DirectX11GraphicsState* pGraphicsState;
		std::vector<RenderNode> renderList;
		std::vector<RenderNode> preRenderList;
		std::vector<RenderNode> postRenderList;
		AxisBuffer<2> axisBuffer;
		AxisBuffer<6> poseBuffer;
		GridBuffer xGridBuffer;
		GridBuffer zGridBuffer;
		ShaderPKG gridShader;
		ShaderPKG spriteShader;
		ConstBuffer<SpriteConstBuffer> spriteConstBuffer;
		SmartComPtr<ID3D11Buffer> pSpriteVertexBuffer;
		GridConstBuffer gridConstBuffer;
		Projection projection;
		XMFLOAT4X4 projMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 viewProjMatrix;
		XMFLOAT4X4 leftTopOrthoMatrix;
		XMFLOAT4X4 centerOrthoMatrix;
		std::array<ao_intptr, MAX_TEXTURE>	logSrViewBuffer;
		std::array<ao_intptr, MAX_TEXTURE>	logSamplerBuffer;
		std::array<ao_intptr, MAX_TEXTURE>	logVertexSrViewBuffer;
		std::array<ao_intptr, MAX_TEXTURE>	logVertexSamplerBuffer;
		std::array<BaseTexture, MAX_TEXTURE> nullTexture;

	private:
		/*!-----------------------------------------------------------
		//	@brief		null�e�N�X�`���쐬
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		inline HRESULT	CreateNullTexture();

		/*!-----------------------------------------------------------
		//	@brief		null�p�̃T���v���[�X�e�[�g�쐬
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		inline HRESULT CreateNullSamplerState();

		/*!-----------------------------------------------------------
		//	@brief		���萔�o�b�t�@
		//	@param[in]	_worldMtx	���[���h�s��
		//	@param[in]	_color		�F�z��
		//	@return		S_OK�F�}�b�v�����@����ȊO�F�}�b�v���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/15
		------------------------------------------------------------*/
		inline HRESULT	MapAxisConstBuffer(const XMFLOAT4X4& _worldMtx, const float* const _color);

		/*!-----------------------------------------------------------
		//	@brief		�V�F�[�_�[�t�@�C���̓ǂݍ���
		//	@param[in]	_vsFilePath	���_�V�F�[�_�[�t�@�C���p�X
		//	@param[in]	_psFilePath	�s�N�Z���V�F�[�_�[�t�@�C���p�X
		//	@param[out]	_pOutShader	�o�͂���V�F�[�_�[�|�C���^	
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/20
		------------------------------------------------------------*/
		HRESULT LoadShaderFile(LPCWSTR _vsFilePath, LPCWSTR _psFilePath, ShaderPKG* const _pOutShader);
	};
}

#endif // _Include_DirectX11GraphicsRenderer_hpp_