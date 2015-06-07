/*************************************************************
*	@file   	SceneDatabase.hpp
*	@brief  	SceneDatabase�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneDatabase_hpp_	// �C���N���[�h�K�[�h
#define _Include_SceneDatabase_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"SceneCtrlProxyComposite.hpp"
#include"AoScene.hpp"

namespace ao
{

	/*!-----------------------------------------------------------
	//	@class  SceneDatabase
	//	@brief  �V�[���p�̃f�[�^�x�[�X
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SceneDatabase : public SceneCtrlProxyComposite
	{
	public:
		SceneDatabase(SceneControlProxy* _pSceneCtrlProxy) :
			pNowScene(nullptr),
			pSwapScene(nullptr),
			pDeleteScene(nullptr),
			SceneCtrlProxyComposite(_pSceneCtrlProxy){};
		~SceneDatabase()
		{
			if (this->pNowScene)
			{
				delete this->pNowScene;
			}

			if (this->pSwapScene)
			{
				delete this->pSwapScene;
			}

			if (this->pDeleteScene)
			{
				delete this->pDeleteScene;
			}
		};

	public:
		ISceneBase*	pNowScene;
		ISceneBase* pSwapScene;
		ISceneBase* pDeleteScene;
	};
}

#endif // _Include_SceneDatabase_hpp_