/*************************************************************
*	@file   	RenderBaseShader.hpp
*	@brief  	RenderBaseShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/10
*************************************************************/

#ifndef _Include_RenderBaseShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_RenderBaseShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"BaseShader.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  RenderBaseShader
	//	@brief  �����_�����O���V�F�[�_�[
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/10
	------------------------------------------------------------*/
	class RenderBaseShader : public BaseShader
	{
	public:
		RenderBaseShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) : 
			BaseShader(_pd3dDevice,_pd3dDeviceContext){};

		virtual	~RenderBaseShader()override{};

		/*!-----------------------------------------------------------
		//	@brief		�v�b�V���o�b�N�J�n���̏���
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		virtual	void	OnPushbackEnter(){};

		/*!-----------------------------------------------------------
		//	@brief		�v�b�V���o�b�N�I�����̏���
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		virtual	void	OnPushbackQuit(){};

		/*!-----------------------------------------------------------
		//	@brief		�`�惁�\�b�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		virtual	void	OnDraw(){};

		/*!-----------------------------------------------------------
		//	@brief		���U���g����
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/19
		------------------------------------------------------------*/
		virtual	void	OnResult(){};

		/*!-----------------------------------------------------------
		//	@brief		�N���A����
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/03
		------------------------------------------------------------*/
		virtual void	OnClear(){};
	};
}

#endif // _Include_RenderBaseShader_hpp_