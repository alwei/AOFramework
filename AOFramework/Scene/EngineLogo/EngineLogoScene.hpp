/*************************************************************
*	@file   	EngineLogoScene.hpp
*	@brief  	EngineLogoScene�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/15
*************************************************************/

#ifndef _Include_EngineLogoScene_hpp_	// �C���N���[�h�K�[�h
#define _Include_EngineLogoScene_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"../AoScene.hpp"
#include"../../Resource/Sprite.hpp"
#include"../../Utility/MsTime.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  EngineLogoScene
	//	@brief  �G���W�����S�V�[��
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/15
	------------------------------------------------------------*/
	class EngineLogoScene : public ao::SceneBase
	{
	public:
		/// �R���X�g���N�^
		EngineLogoScene();

		/// �f�X�g���N�^
		~EngineLogoScene();


		/*!-----------------------------------------------------------
		//	@brief		�Z�b�g�A�b�v
		//	@param[in]	_pMainScene	���[�U�[�w��̃��C���V�[��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/15
		------------------------------------------------------------*/
		void Setup(SceneBase* _pMainScene)
		{
			this->pMainScene = _pMainScene;
		}

		/*!-----------------------------------------------------------
		//	@brief		���[�h�O�Ɏ��s����
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/15
		------------------------------------------------------------*/
		void	InitPreLoad();

		/*!-----------------------------------------------------------
		//	@brief		���[�h�X���b�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/15
		------------------------------------------------------------*/
		void	LoadThread();

		/*!-----------------------------------------------------------
		//	@brief		�V�[�����s�O�ɃR�[�������
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/15
		------------------------------------------------------------*/
		void	OnStartCall();

	protected:
		/*!-----------------------------------------------------------
		//	@brief		�X�V
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/15
		------------------------------------------------------------*/
		void	OnUpdate();

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/15
		------------------------------------------------------------*/
		void	OnRender();

	private:
		enum class State : int
		{
			FadeStandby = 0,
			FadeIn,
			FadeNow,
			FadeOut,
			FadeEnd
		};

		BOOL isSkip;
		State nowState;
		ao::Sprite* engineLogo;
		SceneBase* pMainScene;
		MsTime msTime;
		float logoAlpha;
	};
}

#endif // _Include_EngineLogoScene_hpp_