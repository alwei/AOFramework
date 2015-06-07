/*************************************************************
*	@file   	InterLock.hpp
*	@brief  	InterLock�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/06/03
*************************************************************/

#ifndef _Include_InterLock_hpp_	// �C���N���[�h�K�[�h
#define _Include_InterLock_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  InterLock
	//	@brief  �N���e�B�J���Z�N�V����
	//	@author	Tatsunori Aoyama
	//	@date	2015/06/03
	------------------------------------------------------------*/
	class InterLock
	{
	public:
		InterLock()
		{
			InitializeCriticalSection(&this->cs);
		}

		~InterLock()
		{
			DeleteCriticalSection(&this->cs);
		}


		/*!-----------------------------------------------------------
		//	@brief		���b�N
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		inline void Lock()
		{
			EnterCriticalSection(&this->cs);
		}

		/*!-----------------------------------------------------------
		//	@brief		�A�����b�N
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		inline void Unlock()
		{
			LeaveCriticalSection(&this->cs);
		}

	private:
		CRITICAL_SECTION cs;
	};
}

#endif // _Include_InterLock_hpp_