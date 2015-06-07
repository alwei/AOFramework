/*************************************************************
*	@file   	GameObject.hpp
*	@brief  	GameObjectクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/25
*************************************************************/

#ifndef _Include_GameObject_hpp_	// インクルードガード
#define _Include_GameObject_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"TaskObject.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  GameObject
	//	@brief  ゲームオブジェクト
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/25
	------------------------------------------------------------*/
	class GameObject : public TaskObject
	{
	public:
		/// コンストラクタ
		GameObject() : TaskObject(_T("GameObject")){};
		GameObject(AOLPSTR _name) : TaskObject(_name){};

		/// デストラクタ
		virtual	~GameObject(){};
	};
}

#endif // _Include_GameObject_hpp_