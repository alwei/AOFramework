/*************************************************************
*	@file   	SceneDatabase.hpp
*	@brief  	SceneDatabaseクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneDatabase_hpp_	// インクルードガード
#define _Include_SceneDatabase_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"SceneCtrlProxyComposite.hpp"
#include"AoScene.hpp"

namespace ao
{

	/*!-----------------------------------------------------------
	//	@class  SceneDatabase
	//	@brief  シーン用のデータベース
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