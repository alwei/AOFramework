/*************************************************************
*	@file   	SceneProxyComposite.hpp
*	@brief  	SceneProxyComposite�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneProxyComposite_hpp_	// �C���N���[�h�K�[�h
#define _Include_SceneProxyComposite_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"../Utility/OwnerComposite.hpp"
#include"SceneControlProxy.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  SceneProxyComposite
	//	@brief  �V�[���v���L�V���ێ�����p���p�N���X
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