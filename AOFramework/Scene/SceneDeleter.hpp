/*************************************************************
*	@file   	SceneDeleter.hpp
*	@brief  	SceneDeleterクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneDeleter_hpp_	// インクルードガード
#define _Include_SceneDeleter_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"SceneCtrlProxyComposite.hpp"

namespace ao
{
	class SceneControlProxy;
	class ISceneBase;

	/*!-----------------------------------------------------------
	//	@class  SceneDeleter
	//	@brief  シーン削除用のクラス
	//	@author	Tatsunori Aoyama
	//	@date	2015/00/28
	------------------------------------------------------------*/
	class SceneDeleter : public SceneCtrlProxyComposite
	{
	public:
		SceneDeleter(SceneControlProxy* _pSceneCtrlProxy);
		~SceneDeleter();

		/*!-----------------------------------------------------------
		//	@brief		シーンの削除
		//	@param[in]	_ppDeleteScene	削除するシーンポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		void DeleteScene(ISceneBase** _ppDeleteScene);
	};
}

#endif // _Include_SceneDeleter_hpp_