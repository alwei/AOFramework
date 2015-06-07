/*************************************************************
*	@file   	AoObject.hpp
*	@brief  	AoObject�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/25
*************************************************************/

#ifndef _Include_AoObject_hpp_	// �C���N���[�h�K�[�h
#define _Include_AoObject_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<tchar.h>
#include"AoDCI.hpp"
#include"../Define/SystemDefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  AoObject
	//	@brief  ���I�u�W�F�N�g
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/25
	------------------------------------------------------------*/
	class AoObject
	{
		AO_ROOT_CLASS_DEFINE;
	public:
		/// �R���X�g���N�^
		AoObject();
		AoObject(AOLPSTR _name);
		AoObject(const string& _name);

		/// �f�X�g���N�^
		virtual	~AoObject();

		/*!-----------------------------------------------------------
		//	@brief		�X�V
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