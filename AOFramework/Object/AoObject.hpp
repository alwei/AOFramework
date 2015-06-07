/*************************************************************
*	@file   	AoObject.hpp
*	@brief  	AoObjectクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/25
*************************************************************/

#ifndef _Include_AoObject_hpp_	// インクルードガード
#define _Include_AoObject_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<tchar.h>
#include"AoDCI.hpp"
#include"../Define/SystemDefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  AoObject
	//	@brief  基底オブジェクト
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/25
	------------------------------------------------------------*/
	class AoObject
	{
		AO_ROOT_CLASS_DEFINE;
	public:
		/// コンストラクタ
		AoObject();
		AoObject(AOLPSTR _name);
		AoObject(const string& _name);

		/// デストラクタ
		virtual	~AoObject();

		/*!-----------------------------------------------------------
		//	@brief		更新
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/24
		------------------------------------------------------------*/
		virtual void Update(){};

	public:
		string name;

	private:
	};
}

#endif // _Include_AoObject_hpp_