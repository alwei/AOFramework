/*************************************************************
*	@file   	FadeEffectShader.hpp
*	@brief  	FadeEffectShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/12/03
*************************************************************/

#ifndef _Include_FadeEffectShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_FadeEffectShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"../../../AOFramework/Api/DirectX11GraphicsRenderer.hpp"
#include"../../../AOFramework/Api/DirectX11GraphicsState.hpp"
#include"../../../AOFramework/Resource/ShaderResource.hpp"
#include"../../../AOFramework/Resource/ConstBuffer.hpp"
#include"../../../AOFramework/Shader/RenderBaseShader.hpp"


namespace ao
{

	enum class FadeType : int
	{
		BlockIn = 0,
		BlackOut,
		WhiteIn,
		WhiteOut
	};

	/*!-----------------------------------------------------------
	//	@class  FadeEffectShader
	//	@brief  �t�F�[�h�G�t�F�N�g�V�F�[�_�[
	//	@author	Tatsunori Aoyama
	//	@date	2014/12/03
	------------------------------------------------------------*/
	class FadeEffectShader : public RenderBaseShader
	{
	public:
		/// �R���X�g���N�^
		FadeEffectShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		/// �f�X�g���N�^
		~FadeEffectShader()override final;

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	_pGraphicsState		�O���t�B�b�N�X�e�[�g
		//  @param[in]  _pRenderer			�����_���[
		//	return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/01
		------------------------------------------------------------*/
		HRESULT	Initialize(
			ao::DirectX11GraphicsState* _pGraphicsState,
			ao::DirectX11GraphicsRenderer* _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		�t�F�[�h�p�����[�^�[�ݒ�
		//	@param[in]	_fadeType		�t�F�[�h�̎��
		//  @param[in]  _sec			�t�F�[�h���鎞�ԁi�b�j
		//	@param[in]	_init			���������鎞��[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/03
		------------------------------------------------------------*/
		void SetFadeParameter(FadeType _fadeType, float _sec,float _init=0);

		/*!-----------------------------------------------------------
		//	@brief		�X�V
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/03
		------------------------------------------------------------*/
		void Update();

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/03
		------------------------------------------------------------*/
		void Render();

		/*!-----------------------------------------------------------
		//	@brief		�`�惁�\�b�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		void OnDraw();

		/*!-----------------------------------------------------------
		//	@brief		�ݒ莞�Ԃ��擾
		//	@return		�ݒ�̎���
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/09
		------------------------------------------------------------*/
		float GetMaxTime()
		{
			return this->maxFadeTime;
		}

		/*!-----------------------------------------------------------
		//	@brief		���݂̎��Ԃ��擾
		//	@return		���݂̎���
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/09
		------------------------------------------------------------*/
		float GetNowTime()
		{
			return this->nowFadeTime;
		}

		/*!-----------------------------------------------------------
		//	@brief		���K�����ꂽ���Ԃ��擾
		//  @return     ���K�����ꂽ����
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/03
		------------------------------------------------------------*/
		float GetNormalizeTime()
		{
			return this->nowFadeTime / this->maxFadeTime;
		}

		/*!-----------------------------------------------------------
		//	@brief		�t�F�[�h���I���������ǂ���
		//  @return     TRUE�F�I���@FALSE�F�I�����Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/03
		------------------------------------------------------------*/
		BOOL IsFadeEnd()
		{
			return (this->nowFadeTime >= this->maxFadeTime);
		}

	private:
		BOOL isPushRender;
		DirectX11GraphicsState* pGraphicsState;
		DirectX11GraphicsRenderer* pRenderer;
		ConstBuffer<ao::float4> mapColor;
		SmartComPtr<ID3D11Buffer> vertexBuffer;
		VertexShader* pVertexShader;
		PixelShader* pPixelShader;
		float3 fadeColor;
		FadeType fadeType;
		float fadeAlpha;
		float nowFadeTime;
		float maxFadeTime;
	};
}

#endif // _Include_FadeEffectShader_hpp_