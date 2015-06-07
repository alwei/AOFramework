/*************************************************************
*	@file   	RenderingDefine.hpp
*	@brief  	RenderingDefine�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/05/11
*************************************************************/

#ifndef _Include_RenderingDefine_hpp_	// �C���N���[�h�K�[�h
#define _Include_RenderingDefine_hpp_

#include"../Utility/Binary.hpp"

namespace ao
{
	namespace RenderDef
	{
		static const u32 LIGHTING = Binary<1>::value;
		static const u32 CAST_SHADOW = Binary<10>::value;
		static const u32 RECV_SHADOW = Binary<100>::value;
	}

	enum class SamplerState : int32_t
	{
		LINEAR = 0,
		ANISOTROPIC,
		POINT,
		MAX
	};

	enum class OrthoType : int32_t
	{
		LeftTop = 0,
		Center = 1
	};
}


#endif // _Include_RenderingDefine_hpp_