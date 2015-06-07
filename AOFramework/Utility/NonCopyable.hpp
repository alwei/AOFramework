#ifndef __NONCOPYABLE_HPP__
#define __NONCOPYABLE_HPP__

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  NonCopyable
	//	@brief  �T�v�R�s�[�֎~�N���X
	//	@note	�h�����Ďg�p���Ă�������
	//	@author	Tatsunori Aoyama
	//	@date	2013/05/13
	------------------------------------------------------------*/
	class	NonCopyable
	{
	public:
		NonCopyable(){}
		virtual ~NonCopyable(){}
	private:
		void operator = (const NonCopyable& instCopy) = delete;
		void operator = (const NonCopyable&& instCopy) = delete;
		NonCopyable(const NonCopyable& instCopy) = delete;
		NonCopyable(const NonCopyable&& instCopy) = delete;
	};
}

#endif