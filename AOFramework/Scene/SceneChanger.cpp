#include"stdafx.hpp"
#include"SceneChanger.hpp"
#include"SceneDatabase.hpp"
#include"AoScene.hpp"


using namespace ao;


//------------------------------------------------------------------------------
/**	@brief		�V�[���̃X���b�s���O */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/02/28	15:12	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneChanger::SwapScene(ISceneBase* _pNextScene,SceneDatabase* _pSceneDatabase)
{
	if (_pNextScene == nullptr)
	{
		return;
	}

	_pSceneDatabase->pDeleteScene = _pSceneDatabase->pNowScene;
	_pSceneDatabase->pSwapScene = _pNextScene;
	_pSceneDatabase->pNowScene = nullptr;

	reinterpret_cast<SceneBase*>(_pSceneDatabase->pDeleteScene)->enableRender = FALSE;
}


//------------------------------------------------------------------------------
/**	@brief		�V�[���̕ύX */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/04/09	22:27	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneChanger::ChangeScene(SceneDatabase* _pSceneDatabase)
{
	if (_pSceneDatabase->pSwapScene == nullptr)
	{
		return;
	}

	_pSceneDatabase->pNowScene = _pSceneDatabase->pSwapScene;
	_pSceneDatabase->pSwapScene = nullptr;
}