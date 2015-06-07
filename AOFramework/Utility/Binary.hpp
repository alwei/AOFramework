/*************************************************************
*	@file   	Binary.hpp
*	@brief  	Binaryクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/05/11
*************************************************************/

#ifndef _Include_Binary_hpp_	// インクルードガード
#define _Include_Binary_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../Define/SystemDefine.hpp"

/*!-----------------------------------------------------------
//	@struct  Binary
//	@brief  バイナリテンプレート
//	@author	Tatsunori Aoyama
//	@date	2015/05/11
------------------------------------------------------------*/
template<u32 N>
struct Binary
{
	static const u32 value = Binary<N / 10>::value * 2 + N % 10;
};

template<>
struct Binary < 0 >
{
	static const u32 value = 0;
};


/*!-----------------------------------------------------------
//	@struct  Binary
//	@brief  バイナリテンプレート(ランタイム版)
//	@author	Tatsunori Aoyama
//	@date	2015/05/11
------------------------------------------------------------*/
template<u32 N>
inline const u32 ToBinary()
{
	u32 result = 0;
	for (u32 bit = 0x1; N; N /= 10, bit << 1)
	{
		if (N % 10)
		{
			result += bit;
		}
	}

	return result;
}

#endif // _Include_Binary_hpp_