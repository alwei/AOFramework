/*************************************************************
*	@file   	MsTime.hpp
*	@brief  	MsTimeクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/01/01
*************************************************************/

#ifndef _Include_MsTime_hpp_	// インクルードガード
#define _Include_MsTime_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"BezierCurve.hpp"
#include"../System/FpsController.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  MsTime
	//	@brief  時間補間クラス
	//	@author	Tatsunori Aoyama
	//	@date	2015/01/01
	------------------------------------------------------------*/
	class MsTime
	{
	public:
		MsTime() :
			nowTime(0),
			maxTime(1),
			normalizeTime(0){};

		~MsTime(){};

		/*!-----------------------------------------------------------
		//	@brief		パラメーター設定
		//	@param[in]	_setTime	設定タイム
		//	@param[in]	_initTime	初期化時間[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/01
		------------------------------------------------------------*/
		void SetParameter(float _setTime,float _initTime=0)
		{
			this->nowTime = _initTime;
			this->maxTime = _setTime;
			this->normalizeTime = this->nowTime / this->maxTime;
		}

		/*!-----------------------------------------------------------
		//	@brief		更新
		//	@param[in]	_limitDelta	デルタタイムの制御値(2.0f=30F限界）
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/01
		------------------------------------------------------------*/
		void Update(float _limitDelta = 0.032f)
		{
			float d = min(_limitDelta, ao::FpsController::GetInstance()->GetDeltaTime());
			this->nowTime = min(this->nowTime + d, this->maxTime);
			this->normalizeTime = this->nowTime / this->maxTime;
		}

		/*!-----------------------------------------------------------
		//	@brief		正規化された時間取得
		//	@return		正規化された時間（0~1.0f）
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/01
		------------------------------------------------------------*/
		const float GetNormalizedTime() const
		{
			return this->normalizeTime;
		}

		/*!-----------------------------------------------------------
		//	@brief		終了したかどうか
		//	@return		TRUE：終了　FALSE：終了していない
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/01
		------------------------------------------------------------*/
		const BOOL IsEnd() const
		{
			return (this->nowTime >= this->maxTime);
		}

		/*!-----------------------------------------------------------
		//	@brief		リセット
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/01
		------------------------------------------------------------*/
		void Reset()
		{
			this->nowTime = 0;
			this->normalizeTime = 0;
		}

	public:
		float nowTime;
		float maxTime;
		float normalizeTime;
	};
}

#endif // _Include_MsTime_hpp_