/*************************************************************
*	@file   	SceneProxyComposite.hpp
*	@brief  	SceneProxyCompositeクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneProxyComposite_hpp_	// インクルードガード
#define _Include_SceneProxyComposite_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../Utility/OwnerComposite.hpp"
#include"SceneControlProxy.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  SceneProxyComposite
	//	@brief  シーンプロキシが保持する継承用クラス
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SceneCtrlProxyComposite : public OwnerComposite < SceneControlProxy >
	{
	public:
		SceneCtrlProxyComposite(SceneControlProxy* _pSceneCtrlProxy) : OwnerComposite(_pSceneCtrlProxy){};
		virtual ~SceneCtrlProxyComposite(){};
	};
}

#endif // _Include_SceneProxyComposite_hpp_