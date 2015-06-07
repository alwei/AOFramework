/*************************************************************
*	@file   	xorShift.hpp
*	@brief  	xorShiftクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2013/05/19
*************************************************************/

#ifndef _Include_XorShift_hpp_	// インクルードガード
#define _Include_XorShift_hpp_


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  XorShift
	//	@brief  Xorshifを使用した乱数クラス
	//	@author	Tatsunori Aoyama
	//	@date	2013/05/19
	------------------------------------------------------------*/
	class XorShift
	{
	public:
		~XorShift(){};

		/*!-----------------------------------------------------------
		//	@brief		インスタンス取得
		//	@return		自身の参照
		//	@author		Tatsunori Aoyama
		//	@date		2013/05/19
		------------------------------------------------------------*/
		static	XorShift*	GetInstance(){
			static	XorShift singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		乱数のシード値設定
		//	@param[in]	setSeed 設定するシード値
		//	@author		Tatsunori Aoyama
		//	@date		2013/05/19
		------------------------------------------------------------*/
		void	InitSeed(_int64 setSeed){
			this->seed = setSeed;
			this->z ^= this->seed;
			this->z ^= this->z >> 21;
			this->z ^= this->z << 35;
			this->z ^= this->z >> 4;
			this->w ^= this->z << 2;
			this->w *= 2685821692745602874LL;
			this->z *= 2685821657736338717LL;
		}

		/*!-----------------------------------------------------------
		//	@brief		乱数の取得
		//	@return		取得する乱数
		//	@author		Tatsunori Aoyama
		//	@date		2013/05/19
		------------------------------------------------------------*/
		_int64 GetRandom(){
			_int64 t = (this->x ^ (this->x << 11));
			this->x = this->y;
			this->y = this->z;
			this->z = this->w;
			return (this->w = (this->w ^ (this->w >> 19)) ^ (t ^ (t >> 8)));
		}

		/*!-----------------------------------------------------------
		//	@brief		範囲乱数の取得
		//	@param[in]	min 最小値（範囲に含まれる）
		//	@param[in]	max 最大値（範囲に含まれる）
		//	@return		取得する乱数
		//	@author		Tatsunori Aoyama
		//	@date		2013/05/19
		------------------------------------------------------------*/
		_int64 GetRandom(int min, int max){
			return (this->GetRandom() % (max + 1 - min) + min);
		}

	private:
		_int64 x;
		_int64 y;
		_int64 z;
		_int64 w;
		_int64 seed;
	private:
		XorShift()
		{
			this->x = 123456789;
			this->y = 362436069;
			this->z = 521288629;
			this->w = 88675123;
			this->seed = 1;
		};
	};
}


#endif // _Include_XorShift_hpp_