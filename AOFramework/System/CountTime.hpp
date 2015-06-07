/*************************************************************
*	@file   	CountTime.hpp
*	@brief  	CountTimeクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/01/13
*************************************************************/

#ifndef _Include_CountTime_hpp_	// インクルードガード
#define _Include_CountTime_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<chrono>


namespace ao
{
	using namespace std;
	/*!-----------------------------------------------------------
	//	@class  CountTime
	//	@brief  時間計測
	//	@author	Tatsunori Aoyama
	//	@date	2015/01/13
	------------------------------------------------------------*/
	class CountTime
	{
	public:
		CountTime(){};
		~CountTime(){};

		/*!-----------------------------------------------------------
		//	@brief		カウント開始
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		void Begin()
		{
			this->beginTime= std::chrono::system_clock::now();
		}

		/*!-----------------------------------------------------------
		//	@brief		カウント停止
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
		//	@brief		ナノ秒取得
		//	@return		ナノ秒
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		template<class T = double>
		T GetNanoSeconds()
		{
			return static_cast<T>(this->nano.count());
		}

		/*!-----------------------------------------------------------
		//	@brief		マイクロ秒取得
		//	@return		マイクロ秒
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		template<class T = double>
		T GetMicroSeconds()
		{
			return static_cast<T>(this->micro.count());
		}

		/*!-----------------------------------------------------------
		//	@brief		ミリ秒取得
		//	@return		ミリ秒
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		template<class T = float>
		T GetMilliSeconds()
		{
			return static_cast<T>(this->milli.count());
		}

		/*!-----------------------------------------------------------
		//	@brief		秒取得
		//	@return		秒
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		template<class T = int>
		T GetSeconds()
		{
			return static_cast<T>(this->seconds.count());
		}

		/*!-----------------------------------------------------------
		//	@brief		分取得
		//	@return		分
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/13
		------------------------------------------------------------*/
		template<class T = int>
		T GetMinute()
		{
			return static_cast<T>(this->minute.count());
		}

		/*!-----------------------------------------------------------
		//	@brief		時取得
		//	@return		時
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