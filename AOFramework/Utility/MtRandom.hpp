/*************************************************************
*	@file   	MtRandom.hpp
*	@brief  	MtRandomクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/09/03
*************************************************************/

#ifndef _Include_MtRandom_hpp_	// インクルードガード
#define _Include_MtRandom_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<random>

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  MtRandom
	//	@brief  メルセンヌ・ツイスターによるC++11乱数
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/03
	------------------------------------------------------------*/
	class MtRandom
	{
	public:
		/// コンストラクタ
		MtRandom()
		{
			this->mt.seed(std::random_device()());
		};


		/*!-----------------------------------------------------------
		//	@brief		自身の呼び出し
		//  @return     自身のインスタンス呼び出し
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		static	MtRandom* GetInstance()
		{
			static MtRandom singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		整数乱数取得
		//	@param[in]	_min	最小値[省略可]
		//  @param[in]  _max	最大値[省略可]
		//  @return     生成された乱数
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		int	GetIntRandom(int _min = INT_MIN, int _max = INT_MAX)
		{
			std::uniform_int_distribution<int> dist(_min, _max);
			return dist(this->mt);
		}

		/*!-----------------------------------------------------------
		//	@brief		浮動小数乱数取得
		//	@param[in]	_min	最小値[省略可]
		//  @param[in]  _max	最大値[省略可]
		//  @return     生成された乱数
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		float GetFloatRandom(float _min = -1.0f, float _max = 1.0f)
		{
			std::uniform_real_distribution<float> dist(_min, _max);
			return dist(this->mt);
		}

		/// デストラクタ
		~MtRandom(){};

	private:
		std::mt19937 mt;
	};
}

#endif // _Include_MtRandom_hpp_