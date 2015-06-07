/*************************************************************
*	@file   	SystemComposite.hpp
*	@brief  	SystemCompositeクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SystemComposite_hpp_	// インクルードガード
#define _Include_SystemComposite_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../Utility/OwnerComposite.hpp"
#include"SystemScene.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  SystemComposite
	//	@brief  SystemSceneがコンポジットする継承用クラス
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SystemComposite : public OwnerComposite < SystemScene >
	{
	public:
		SystemComposite(SystemScene* _pSystemScene) : OwnerComposite(_pSystemScene){};
		virtual ~SystemComposite(){};
	};
}

#endif // _Include_SystemComposite_hpp_