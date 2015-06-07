/*************************************************************
*	@file   	BezierCurve.hpp
*	@brief  	BezierCurveクラスヘッダー
*	@note		Takumi Yamada氏のクラスを改良
*	@author		Takumi Yamada
*	@date		2013/09/01
*************************************************************/

#ifndef _Include_BezierCurve_hpp_	// インクルードガード
#define _Include_BezierCurve_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include <math.h>


//------------------------------------------------------------
// 定数、構造体定義
//------------------------------------------------------------
const static int nPasTgl[12][12] =
{
	{ 1 },
	{ 1, 1 },
	{ 1, 2, 1 },
	{ 1, 3, 3, 1 },
	{ 1, 4, 6, 4, 1 },
	{ 1, 5, 10, 10, 5, 1 },
	{ 1, 6, 15, 20, 15, 6, 1 },
	{ 1, 7, 21, 35, 35, 21, 7, 1 },
	{ 1, 8, 28, 56, 70, 56, 28, 8, 1 },
	{ 1, 9, 36, 84, 126, 126, 84, 36, 9, 1 },
	{ 1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1 },
	{ 1, 11, 55, 165, 330, 464, 464, 330, 165, 55, 11, 1 }
};

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BezierCurve
	//	@brief  ベジェ曲線クラス
	//	@author	Takumi Yamada
	//	@date	2013/09/01
	------------------------------------------------------------*/
	class BezierCurve
	{
	public:

		/*!-----------------------------------------------------------
		//	@brief		シングルトンによる呼び出し
		//	@return		自身のインスタンスポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		static	BezierCurve* GetInstance()
		{
			static BezierCurve singleton;
			return &singleton;
		}

		//Getの引数に使用するパラメータ
		enum ePrm_t : int
		{					// Prm1                   / Prm2
			eSlow_Lv5,      ///< 強ゆっくり動き始める / ゆっくり動き終える
			eSlow_Lv4,      ///< ↑ゆっくり動き始める / ゆっくり動き終える
			eSlow_Lv3,      ///< ゆっくり動き始める / ゆっくり動き終える
			eSlow_Lv2,      ///< ↓ゆっくり動き始める / ゆっくり動き終える
			eSlow_Lv1,      ///< 弱ゆっくり動き始める / ゆっくり動き終える
			eNoAccel,       ///< 直線的な動きをする
			eRapid_Lv1,     ///< 弱急に動き始める      / 急に動き終える
			eRapid_Lv2,     ///< ↑急に動き始める      / 急に動き終える
			eRapid_Lv3,     ///< 急に動き始める      / 急に動き終える
			eRapid_Lv4,     ///< ↓急に動き始める      / 急に動き終える
			eRapid_Lv5,     ///< 強急に動き始める      / 急に動き終える
		};

		/*
		/*!-----------------------------------------------------------
		//	@brief		パラメーター依存の曲線状の値を返す
		//	@param[in]	ePrm1        動き始めのパラメータ(ePrm_tの定義参照)
		//	@param[in]	ePrm2        動き終りのパラメータ(ePrm_tの定義参照)
		//	@param[in]	fRate        変化させたい時刻を0～1で渡す
		//	@return		パラメーターに依存した曲線値
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		float Get(ePrm_t ePrm1, ePrm_t ePrm2, float fRate)
		{
			int n = 3;                //n次元指定
			float y1, y2;
			switch (ePrm1)
			{
			case eSlow_Lv5: y1 = 0;                       n = 11; break;//11次元
			case eSlow_Lv4: y1 = 0;                       n = 9; break;//9次元
			case eSlow_Lv3: y1 = 0;                       n = 7; break;//7次元
			case eSlow_Lv2: y1 = 0;                       n = 5; break;//5次元
			case eSlow_Lv1: y1 = 0;                       n = 3; break;//3次元
			case eNoAccel: y1 = 0.333333f;               n = 3; break;//直線の場合は3次元中1/3の点
			case eRapid_Lv1: y1 = 1;                       n = 3; break;//3次元
			case eRapid_Lv2: y1 = 1;                       n = 5; break;//5次元
			case eRapid_Lv3: y1 = 1;                       n = 7; break;//7次元
			case eRapid_Lv4: y1 = 1;                       n = 9; break;//9次元
			case eRapid_Lv5: y1 = 1;                       n = 11; break;//11次元
			}
			switch (ePrm2)
			{
			case eSlow_Lv5: y2 = 1;                       n = 11; break;//11次元
			case eSlow_Lv4: y2 = 1;                       n = 9; break;//9次元
			case eSlow_Lv3: y2 = 1;                       n = 7; break;//7次元
			case eSlow_Lv2: y2 = 1;                       n = 5; break;//5次元
			case eSlow_Lv1: y2 = 1;                       n = 3; break;//3次元
			case eNoAccel: y2 = 0.6666667f;              n = 3; break;//直線の場合は3次元中2/3の点
			case eRapid_Lv1: y2 = 0;                       n = 3; break;//3次元
			case eRapid_Lv2: y2 = 0;                       n = 5; break;//5次元
			case eRapid_Lv3: y2 = 0;                       n = 7; break;//7次元
			case eRapid_Lv4: y2 = 0;                       n = 9; break;//9次元
			case eRapid_Lv5: y2 = 0;                       n = 11; break;//11次元
			}
			return Get(y1, y2, fRate, n);
		}

	private:
		inline float Get(const float &y1, const float &y2, const float &t, const int &n)
		{
			float b = t > 1 ? 1 : (t < 0 ? 0 : t);
			float a = 1 - b;
			float ay = 0;
			float y[4] = { 0, y1, y2, 1 };
			int m;
			for (int i = 0; i <= n; i++){
				m = i == 0 ? 0 : (i == n ? 3 : (i <= n / 2 ? 1 : 2));//yの添え字決定
				ay += nPasTgl[n][i] * pow(a, n - i)*pow(b, i)*y[m];//※1
			}
			return ay;
		}
	};
}

#endif // _Include_BezierCurve_hpp_
