/*************************************************************
*	@file   	InterLock.hpp
*	@brief  	InterLockクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/06/03
*************************************************************/

#ifndef _Include_InterLock_hpp_	// インクルードガード
#define _Include_InterLock_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  InterLock
	//	@brief  クリティカルセクション
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
		//	@brief		ロック
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		inline void Lock()
		{
			EnterCriticalSection(&this->cs);
		}

		/*!-----------------------------------------------------------
		//	@brief		アンロック
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