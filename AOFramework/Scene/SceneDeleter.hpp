/*************************************************************
*	@file   	SceneDeleter.hpp
*	@brief  	SceneDeleter�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneDeleter_hpp_	// �C���N���[�h�K�[�h
#define _Include_SceneDeleter_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"SceneCtrlProxyComposite.hpp"

namespace ao
{
	class SceneControlProxy;
	class ISceneBase;

	/*!-----------------------------------------------------------
	//	@class  SceneDeleter
	//	@brief  �V�[���폜�p�̃N���X
	//	@author	Tatsunori Aoyama
	//	@date	2015/00/28
	------------------------------------------------------------*/
	class SceneDeleter : public SceneCtrlProxyComposite
	{
	public:
		SceneDeleter(SceneControlProxy* _pSceneCtrlProxy);
		~SceneDeleter();

		/*!-----------------------------------------------------------
		//	@brief		�V�[���̍폜
		//	@param[in]	_ppDeleteScene	�폜����V�[���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		void DeleteScene(ISceneBase** _ppDeleteScene);
	};
}

#endif // _Include_SceneDeleter_hpp_