/*************************************************************
*	@file   	BlockableQueue.hpp
*	@brief  	BlockableQueue�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/05/24
*************************************************************/

#ifndef _Include_BlockableQueue_hpp_	// �C���N���[�h�K�[�h
#define _Include_BlockableQueue_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<queue>
#include<boost\thread\mutex.hpp>
#include<boost\thread\condition_variable.hpp>

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BlockableQueue
	//	@brief  �u���b�L���O�\�ȃL���[
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
		//	@brief		�v�b�V��
		//	@param[in]	_data	�v�b�V������f�[�^�[
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/24
		------------------------------------------------------------*/
		inline void Push(T _data)
		{
			boost::unique_lock < boost::mutex > lock(this->mutex);

			//�ő吔�𒴂����f�[�^���v�b�V�����悤�Ƃ���Ύ����|�b�v����܂Ńu���b�N���܂��B
			this->pushWait.wait(lock, [this]{ return (que.size() != maxQueue); });
			this->que.push(_data);
			this->popWait.notify_one();
		}

		/*!-----------------------------------------------------------
		//	@brief		�|�b�v
		//	@return		�|�b�v�����f�[�^
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/24
		------------------------------------------------------------*/
		inline T Pop()
		{
			boost::unique_lock < boost::mutex > lock(this->mutex);

			//�L���[��0�łȂ��Ȃ�ҋ@�����Ɏ��o���������s��
			this->popWait.wait(lock, [this]{ return (this->que.size() != 0); });
			T data = this->que.front();
			this->que.pop();
			this->pushWait.notify_one();
			return data;
		}

		/*!-----------------------------------------------------------
		//	@brief		�󂩂ǂ���
		//	@return		true�F��@false�F�󂶂�Ȃ�
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