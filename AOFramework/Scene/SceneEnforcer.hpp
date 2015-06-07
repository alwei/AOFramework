/*************************************************************
*	@file   	SceneEnforcer.hpp
*	@brief  	SceneEnforcerクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneEnforcer_hpp_	// インクルードガード
#define _Include_SceneEnforcer_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"SceneCtrlProxyComposite.hpp"

namespace ao
{
	class SceneControlProxy;
	class ISceneBase;

	/*!-----------------------------------------------------------
	//	@class  SceneEnforcer
	//	@brief  シーン執行クラス
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SceneEnforcer :public SceneCtrlProxyComposite
	{
	public:
		SceneEnforcer(SceneControlProxy* _pSceneCtrlProxy);
		~SceneEnforcer();

		/*!-----------------------------------------------------------
		//	@brief		実行
		//	@param[in]	_pExeScene	実行するシーン
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		void Execute(ISceneBase* _pExeScene);
	};
}

#endif // _Include_SceneEnforcer_hpp_