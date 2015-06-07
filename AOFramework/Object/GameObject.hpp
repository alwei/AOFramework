/*************************************************************
*	@file   	GameObject.hpp
*	@brief  	GameObject�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/25
*************************************************************/

#ifndef _Include_GameObject_hpp_	// �C���N���[�h�K�[�h
#define _Include_GameObject_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"TaskObject.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  GameObject
	//	@brief  �Q�[���I�u�W�F�N�g
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/25
	------------------------------------------------------------*/
	class GameObject : public TaskObject
	{
	public:
		/// �R���X�g���N�^
		GameObject() : TaskObject(_T("GameObject")){};
		GameObject(AOLPSTR _name) : TaskObject(_name){};

		/// �f�X�g���N�^
		virtual	~GameObject(){};
	};
}

#endif // _Include_GameObject_hpp_