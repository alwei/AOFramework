/*************************************************************
*	@file   	SceneEnforcer.hpp
*	@brief  	SceneEnforcer�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneEnforcer_hpp_	// �C���N���[�h�K�[�h
#define _Include_SceneEnforcer_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"SceneCtrlProxyComposite.hpp"

namespace ao
{
	class SceneControlProxy;
	class ISceneBase;

	/*!-----------------------------------------------------------
	//	@class  SceneEnforcer
	//	@brief  �V�[�����s�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SceneEnforcer :public SceneCtrlProxyComposite
	{
	public:
		SceneEnforcer(SceneControlProxy* _pSceneCtrlProxy);
		~SceneEnforcer();

		/*!-----------------------------------------------------------
		//	@brief		���s
		//	@param[in]	_pExeScene	���s����V�[��
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		void Execute(ISceneBase* _pExeScene);
	};
}

#endif // _Include_SceneEnforcer_hpp_