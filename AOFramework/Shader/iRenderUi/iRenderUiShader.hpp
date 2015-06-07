/*************************************************************
*	@file   	iRenderUiShader.hpp
*	@brief  	iRenderUiShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/02/10
*************************************************************/

#ifndef _Include_iRenderUiShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_iRenderUiShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<list>
#include"../RenderBaseShader.hpp"
#include"../../Math/Math.hpp"
#include"../../Math/GameArithmetic.hpp"
#include"../../Api/DirectX11GraphicsState.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Resource/ConstBuffer.hpp"
#include"../../Resource/ConstVectorBuffer.hpp"
#include"../../Resource/ShaderResource.hpp"
#include"../../Resource/InstanceBuffer.hpp"
#include"../../Resource/Sprite.hpp"

namespace ao
{
	//4Byte�A���C�����g�����̂���
	//���_�萔�o�b�t�@�Ɋp�x�l��4�p�ӂ��Ă���
	struct iUiVsConst0
	{
		float degree[4];
	};

	//4Byte�A���C�����g�����̂���
	//���_�萔��UV�l��p�ӂ��Ă���
	struct iUiVsConst1
	{
		float2 uv[4];
	};

	//4Byte�A���C�����g�����̂���
	//���_�萔�ɑ��p�����[�^�[��p�ӂ��Ă���
	struct iUiVsConst2
	{
		float2 pos;
		float2 size;
		float4 color;
	};

	//���[�U�[�����擾����C���X�^���X
	struct UiInstance
	{
		UINT numPushRender;
		UINT numCommitRender;
		UINT numAllocatedBuffer;
		BaseTexture* pTexture;
		BlendState blendState;
		FillState fillState;
		VertexShader* pVertexShader;
		ConstVectorBuffer<iUiVsConst0> vsConst0;
		ConstVectorBuffer<iUiVsConst1> vsConst1;
		ConstVectorBuffer<iUiVsConst2> vsConst2;
		SmartComPtr<ID3D11Buffer> vertexBuffer;
		UINT priority;
	};

	/*!-----------------------------------------------------------
	//	@class  iRenderUiShader
	//	@brief  UI�C���X�^���X�����_���[
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/10
	------------------------------------------------------------*/
	class iRenderUiShader : public RenderBaseShader
	{
	public:
		iRenderUiShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~iRenderUiShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		������
		//  @param[in]  _pGraphicsState		�O���t�B�b�N�X�e�[�g
		//  @param[in]  _pRenderer			�����_���[
		//  @return     S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/10
		------------------------------------------------------------*/
		HRESULT	Initialize(
			DirectX11GraphicsState* _pGraphicsState,
			DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		�o�b�t�@�쐬
		//	@param[in]	_pTexture		�g�p����e�N�X�`��
		//	@param[in]	_numAllocate	�m�ۂ���o�b�t�@��
		//	@return		�������ꂽ�C���X�^���X
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/11
		------------------------------------------------------------*/
		UiInstance* CreateInstance(
			BaseTexture* _pTexture,
			UINT _numAllocate = 100);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�R�}���h����
		//	@param[in]	_pInstance		�C���X�^���X�|�C���^
		//	@param[in]	_x				X���W
		//	@param[in]	_y				Y���W
		//	@param[in]	_w				����
		//	@param[in]	_h				�c��
		//	@param[in]	_lt				����UV
		//	@param[in]	_rt				�E��UV
		//	@param[in]	_lb				����UV
		//	@param[in]	_rb				�E��UV
		//	@param[in]	_degree			��]�p�x[�ȗ���]
		//	@param[in]	_r				�ԐF[�ȗ���]
		//	@param[in]	_g				�ΐF[�ȗ���]
		//	@param[in]	_b				�F[�ȗ���]
		//	@param[in]	_a				���ߗ�[�ȗ���]
		//  @param[in]  _priority		�v���C�I���e�B[�ȗ���]
		//	@return		S_OK�F�R�}���h�o�^�����@����ȊO�F�R�}���h�o�^���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/10
		------------------------------------------------------------*/
		HRESULT	PushRenderCommand(
			UiInstance* _pInstance,
			const int _x,
			const int _y,
			const float _w,
			const float _h,
			const float2& _lt,
			const float2& _rt,
			const float2& _lb,
			const float2& _rb,
			const float _degree = 0,
			const FLOAT _r = 1.0f,
			const FLOAT _g = 1.0f,
			const FLOAT _b = 1.0f,
			const FLOAT _a = 1.0f,
			UINT priority = 0xCCCCCCC);


		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�R�}���h���s
		//	@param[in]	_pInstance	�C���X�^���X�|�C���^
		//	@param[in]	_blendState	�u�����h�X�e�[�g[�ȗ���]
		//	@param[in]	_fillState	�`�惂�[�h[�ȗ���]
		//	@return		S_OK�F���s�����@����ȊO�F���s���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/12
		------------------------------------------------------------*/
		HRESULT ExecuteCommandList(
			UiInstance* _pInstance,
			const BlendState _blendState = BlendState::Alpha,
			const FillState _fillState = FillState::Solid);

		/*!-----------------------------------------------------------
		//	@brief		�`��R�}���h���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/10
		------------------------------------------------------------*/
		void	OnDraw() override final;

		/*!-----------------------------------------------------------
		//	@brief		�N���A
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/10
		------------------------------------------------------------*/
		void	Clear();

		/*!-----------------------------------------------------------
		//	@brief		�j������
		//	@param[in]	_pInstance	�C���X�^���X
		//  @return     S_OK�F�j�������@����ȊO�F�j�����s
		//	@author		Tatsunori Aoyama
		//	@date		2014/02/12
		------------------------------------------------------------*/
		HRESULT Destory(UiInstance* _pInstance);

	private:
		BOOL isNeedSort;
		std::vector<UiInstance*> commandList;
		std::list<UiInstance*> pUiInstanceList;
		PixelShader*			pPixelShader;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
		ConstBuffer<XMFLOAT4X4> orthoMatrix;
	};
}

#endif // _Include_iRenderUiShader_hpp_
