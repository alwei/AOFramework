/*************************************************************
*	@file   	SceneExecutor.hpp
*	@brief  	SceneExecutor�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneExecutor_hpp_	// �C���N���[�h�K�[�h
#define _Include_SceneExecutor_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"SceneCtrlProxyComposite.hpp"

namespace ao
{
	class SceneControlProxy;
	class ISceneBase;

	/*!-----------------------------------------------------------
	//	@class  SceneExecutor
	//	@brief  �V�[�����s�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SceneExecutor :public SceneCtrlProxyComposite
	{
	public:
		SceneExecutor(SceneControlProxy* _pSceneCtrlProxy);
		~SceneExecutor();

		/*!-----------------------------------------------------------
		//	@brief		���s
		//	@param[in]	_pExeScene	���s����V�[��
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		void Execute(ISceneBase* _pExeScene);
	};
}

#endif // _Include_SceneExecutor_hpp_