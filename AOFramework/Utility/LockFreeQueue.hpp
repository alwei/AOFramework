/*************************************************************
*	@file   	LockFreeQueue.hpp
*	@brief  	LockFreeQueue�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/04/26
*************************************************************/

#ifndef _Include_LockFreeQueue_hpp_	// �C���N���[�h�K�[�h
#define _Include_LockFreeQueue_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<boost\lockfree\queue.hpp>

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  LockFreeQueue
	//	@brief  ���b�N�s�v�̃L���[
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/26
	------------------------------------------------------------*/
	template<typename T>
	class LockFreeQueue
	{
	public:
		/// �����t���R���X�g���N�^
		LockFreeQueue(int32_t _maxQueue) : numMaxQueue(_maxQueue)
		{
			this->que = new boost::lockfree::queue<T>(_maxQueue);
		}

		/// �f�X�g���N�^
		~LockFreeQueue()
		{
			delete this->que;
		};

		/*!-----------------------------------------------------------
		//	@brief		�L���[�̍ő吔�o�^
		//	@param[in]	_numMaxQueue	�ő�o�^�L���[��
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/24
		------------------------------------------------------------*/
		void SetMaxQueue(int32_t _numMaxQueue)
		{
			this->numMaxQueue = _numMaxQueue;
			this->que->reserve(this->numMaxQueue);
		}

		/*!-----------------------------------------------------------
		//	@brief		�L���[�Ƀv�b�V��
		//	@param[in]	_data	�v�b�V������f�[�^
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/26
		------------------------------------------------------------*/
		inline bool Push(T _data)
		{
			return this->que->push(_data);
		}

		/*!-----------------------------------------------------------
		//	@brief		�L���[����|�b�v
		//  @return     ���o�����f�[�^
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/26
		------------------------------------------------------------*/
		inline bool Pop(T* popData)
		{
			return this->que->pop(*popData);
		}

		/*!-----------------------------------------------------------
		//	@brief		�L���[���󂩂ǂ���
		//	@return		true�F��@false�F�󂶂�Ȃ�
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