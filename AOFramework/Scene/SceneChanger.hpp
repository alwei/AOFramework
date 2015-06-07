/*************************************************************
*	@file   	SceneChanger.hpp
*	@brief  	SceneChangerクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneChanger_hpp_	// インクルードガード
#define _Include_SceneChanger_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"SceneCtrlProxyComposite.hpp"

namespace ao
{
	class SceneControlProxy;
	class ISceneBase;
	class SceneDatabase;

	/*!-----------------------------------------------------------
	//	@class  SceneChanger
	//	@brief  シーン変更クラス
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SceneChanger : public SceneCtrlProxyComposite
	{
	public:
		SceneChanger(SceneControlProxy* _pSceneCtrlProxy) : SceneCtrlProxyComposite(_pSceneCtrlProxy){};
		~SceneChanger(){};

		/*!-----------------------------------------------------------
		//	@brief		シーンのスワッピング
		//	@param[in]	_pNextScene		次のシーン
		//	@param[in]	_pSceneDatabase	シーンデータベース
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		void SwapScene(ISceneBase* _pNextScene, SceneDatabase* _pSceneDatabase);

		/*!-----------------------------------------------------------
		//	@brief		シーンの変更
		//	@param[in]	_pSceneDatabase	シーンデータベース
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/09
		------------------------------------------------------------*/
		void ChangeScene(SceneDatabase* _pSceneDatabase);
	};
}

#endif // _Include_SceneChanger_hpp_