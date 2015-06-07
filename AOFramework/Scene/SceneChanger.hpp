/*************************************************************
*	@file   	SceneChanger.hpp
*	@brief  	SceneChanger�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneChanger_hpp_	// �C���N���[�h�K�[�h
#define _Include_SceneChanger_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"SceneCtrlProxyComposite.hpp"

namespace ao
{
	class SceneControlProxy;
	class ISceneBase;
	class SceneDatabase;

	/*!-----------------------------------------------------------
	//	@class  SceneChanger
	//	@brief  �V�[���ύX�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SceneChanger : public SceneCtrlProxyComposite
	{
	public:
		SceneChanger(SceneControlProxy* _pSceneCtrlProxy) : SceneCtrlProxyComposite(_pSceneCtrlProxy){};
		~SceneChanger(){};

		/*!-----------------------------------------------------------
		//	@brief		�V�[���̃X���b�s���O
		//	@param[in]	_pNextScene		���̃V�[��
		//	@param[in]	_pSceneDatabase	�V�[���f�[�^�x�[�X
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		void SwapScene(ISceneBase* _pNextScene, SceneDatabase* _pSceneDatabase);

		/*!-----------------------------------------------------------
		//	@brief		�V�[���̕ύX
		//	@param[in]	_pSceneDatabase	�V�[���f�[�^�x�[�X
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/09
		------------------------------------------------------------*/
		void ChangeScene(SceneDatabase* _pSceneDatabase);
	};
}

#endif // _Include_SceneChanger_hpp_