/*************************************************************
*	@file   	CountTime.hpp
*	@brief  	CountTime�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/01/13
*************************************************************/

#ifndef _Include_CountTime_hpp_	// �C���N���[�h�K�[�h
#define _Include_CountTime_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<chrono>


namespace ao
{
	using namespace std;
	/*!-----------------------------------------------------------
	//	@class  CountTime
	//	@brief  ���Ԍv��
	//	@author	Tatsunori Aoyama
	//	@date	2015/01/13
	------------------------------------------------------------*/
	class CountTime
	{
	public:
		CountTime(){};
		~CountTime(){};

		/*!-----------------------------------------------------------
		//	@brief		�J�E���g�J�n
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		void Begin()
		{
			this->beginTime= std::chrono::system_clock::now();
		}

		/*!-----------------------------------------------------------
		//	@brief		�J�E���g��~
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		void End()
		{
			this->endTime = std::chrono::system_clock::now();
			this->resultTime = this->endTime - this->beginTime;
			this->nano = chrono::duration_cast<chrono::nanoseconds>(this->resultTime);
			this->micro = chrono::duration_cast<chrono::microseconds>(this->resultTime);
			this->milli = chrono::duration_cast<chrono::milliseconds>(this->resultTime);
			this->seconds = chrono::duration_cast<chrono::seconds>(this->resultTime);
			this->minute = chrono::duration_cast<chrono::minutes>(this->resultTime);
			this->hours = chrono::duration_cast<chrono::hours>(this->resultTime);
		}


		/*!-----------------------------------------------------------
		//	@brief		�i�m�b�擾
		//	@return		�i�m�b
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		template<class T = double>
		T GetNanoSeconds()
		{
			return static_cast<T>(this->nano.count());
		}

		/*!-----------------------------------------------------------
		//	@brief		�}�C�N���b�擾
		//	@return		�}�C�N���b
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		template<class T = double>
		T GetMicroSeconds()
		{
			return static_cast<T>(this->micro.count());
		}

		/*!-----------------------------------------------------------
		//	@brief		�~���b�擾
		//	@return		�~���b
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		template<class T = float>
		T GetMilliSeconds()
		{
			return static_cast<T>(this->milli.count());
		}

		/*!-----------------------------------------------------------
		//	@brief		�b�擾
		//	@return		�b
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		template<class T = int>
		T GetSeconds()
		{
			return static_cast<T>(this->seconds.count());
		}

		/*!-----------------------------------------------------------
		//	@brief		���擾
		//	@return		��
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		template<class T = int>
		T GetMinute()
		{
			return static_cast<T>(this->minute.count());
		}

		/*!-----------------------------------------------------------
		//	@brief		���擾
		//	@return		��
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		template<class T = int>
		T GetHours()
		{
			return static_cast<T>(this->hours.count());
		}

	private:
		chrono::system_clock::time_point beginTime;
		chrono::system_clock::time_point endTime;
		chrono::system_clock::duration resultTime;
		chrono::nanoseconds nano;
		chrono::microseconds micro;
		chrono::milliseconds milli;
		chrono::seconds seconds;
		chrono::minutes minute;
		chrono::hours hours;
	};
}

#endif // _Include_CountTime_hpp_