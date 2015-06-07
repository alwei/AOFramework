/*************************************************************
*	@file   	IODefine.hpp
*	@brief  	IODefineクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/05/25
*************************************************************/

#ifndef _Include_IODefine_hpp_	// インクルードガード
#define _Include_IODefine_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"SystemDefine.hpp"

namespace ao
{
	enum class ImageFormat : u8
	{
		BMP = 0,
		JPG,
		PNG
	};
}

#endif // _Include_IODefine_hpp_