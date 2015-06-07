/*************************************************************
*	@file   	SystemComposite.hpp
*	@brief  	SystemComposite�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SystemComposite_hpp_	// �C���N���[�h�K�[�h
#define _Include_SystemComposite_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"../Utility/OwnerComposite.hpp"
#include"SystemScene.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  SystemComposite
	//	@brief  SystemScene���R���|�W�b�g����p���p�N���X
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