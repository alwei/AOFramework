/*************************************************************
*	@file   	LockFreeQueue.hpp
*	@brief  	LockFreeQueueクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/04/26
*************************************************************/

#ifndef _Include_LockFreeQueue_hpp_	// インクルードガード
#define _Include_LockFreeQueue_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<boost\lockfree\queue.hpp>

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  LockFreeQueue
	//	@brief  ロック不要のキュー
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/26
	------------------------------------------------------------*/
	template<typename T>
	class LockFreeQueue
	{
	public:
		/// 引数付きコンストラクタ
		LockFreeQueue(int32_t _maxQueue) : numMaxQueue(_maxQueue)
		{
			this->que = new boost::lockfree::queue<T>(_maxQueue);
		}

		/// デストラクタ
		~LockFreeQueue()
		{
			delete this->que;
		};

		/*!-----------------------------------------------------------
		//	@brief		キューの最大数登録
		//	@param[in]	_numMaxQueue	最大登録キュー数
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/24
		------------------------------------------------------------*/
		void SetMaxQueue(int32_t _numMaxQueue)
		{
			this->numMaxQueue = _numMaxQueue;
			this->que->reserve(this->numMaxQueue);
		}

		/*!-----------------------------------------------------------
		//	@brief		キューにプッシュ
		//	@param[in]	_data	プッシュするデータ
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/26
		------------------------------------------------------------*/
		inline bool Push(T _data)
		{
			return this->que->push(_data);
		}

		/*!-----------------------------------------------------------
		//	@brief		キューからポップ
		//  @return     取り出したデータ
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/26
		------------------------------------------------------------*/
		inline bool Pop(T* popData)
		{
			return this->que->pop(*popData);
		}

		/*!-----------------------------------------------------------
		//	@brief		キューが空かどうか
		//	@return		true：空　false：空じゃない
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/24
		------------------------------------------------------------*/
		inline bool Empty() const
		{
			return this->que->empty();
		}

	private:
		int32_t numMaxQueue;
		boost::lockfree::queue<T>* que;
	};
}

#endif // _Include_LockFreeQueue_hpp_