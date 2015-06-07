/*************************************************************
*	@file   	BlockableQueue.hpp
*	@brief  	BlockableQueueクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/05/24
*************************************************************/

#ifndef _Include_BlockableQueue_hpp_	// インクルードガード
#define _Include_BlockableQueue_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<queue>
#include<boost\thread\mutex.hpp>
#include<boost\thread\condition_variable.hpp>

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BlockableQueue
	//	@brief  ブロッキング可能なキュー
	//	@author	Tatsunori Aoyama
	//	@date	2015/05/24
	------------------------------------------------------------*/
	template<class T>
	class BlockableQueue
	{
	public:
		BlockableQueue(int32_t _max) : maxQueue(_max){};
		~BlockableQueue(){};

		/*!-----------------------------------------------------------
		//	@brief		プッシュ
		//	@param[in]	_data	プッシュするデーター
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/24
		------------------------------------------------------------*/
		inline void Push(T _data)
		{
			boost::unique_lock < boost::mutex > lock(this->mutex);

			//最大数を超えたデータをプッシュしようとすれば次がポップするまでブロックします。
			this->pushWait.wait(lock, [this]{ return (que.size() != maxQueue); });
			this->que.push(_data);
			this->popWait.notify_one();
		}

		/*!-----------------------------------------------------------
		//	@brief		ポップ
		//	@return		ポップしたデータ
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/24
		------------------------------------------------------------*/
		inline T Pop()
		{
			boost::unique_lock < boost::mutex > lock(this->mutex);

			//キューが0でないなら待機せずに取り出し処理を行う
			this->popWait.wait(lock, [this]{ return (this->que.size() != 0); });
			T data = this->que.front();
			this->que.pop();
			this->pushWait.notify_one();
			return data;
		}

		/*!-----------------------------------------------------------
		//	@brief		空かどうか
		//	@return		true：空　false：空じゃない
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/24
		------------------------------------------------------------*/
		inline bool Empty() const
		{
			return this->que.empty();
		}

	private:
		int32_t maxQueue;
		boost::mutex mutex;
		boost::condition_variable pushWait;
		boost::condition_variable popWait;
		std::queue<T> que;
	};
}

#endif // _Include_BlockableQueue_hpp_