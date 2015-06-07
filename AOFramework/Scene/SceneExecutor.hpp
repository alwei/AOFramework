/*************************************************************
*	@file   	SceneExecutor.hpp
*	@brief  	SceneExecutorクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneExecutor_hpp_	// インクルードガード
#define _Include_SceneExecutor_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"SceneCtrlProxyComposite.hpp"

namespace ao
{
	class SceneControlProxy;
	class ISceneBase;

	/*!-----------------------------------------------------------
	//	@class  SceneExecutor
	//	@brief  シーン執行クラス
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SceneExecutor :public SceneCtrlProxyComposite
	{
	public:
		SceneExecutor(SceneControlProxy* _pSceneCtrlProxy);
		~SceneExecutor();

		/*!-----------------------------------------------------------
		//	@brief		実行
		//	@param[in]	_pExeScene	実行するシーン
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		void Execute(ISceneBase* _pExeScene);
	};
}

#endif // _Include_SceneExecutor_hpp_