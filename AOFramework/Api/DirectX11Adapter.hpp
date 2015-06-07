/*************************************************************
*	@file   	DirectX11Adapter.hpp
*	@brief  	DirectX11Adapter�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/07/15
*************************************************************/

#ifndef _Include_DirectX11Adapter_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11Adapter_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"DirectX11Header.hpp"
#include"../Utility/SmartComPtr.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11Adapter
	//	@brief  �A�_�v�^�[�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/15
	------------------------------------------------------------*/
	class DirectX11Adapter
	{
	public:
		DirectX11Adapter();
		~DirectX11Adapter();

		/*!-----------------------------------------------------------
		//	@brief		�A�_�v�^�[�쐬
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/15
		------------------------------------------------------------*/
		HRESULT	Create();

	public:
		SmartComPtr<IDXGIAdapter>	inst;
		static	SmartComPtr<IDXGIFactory>	factory;
	};
}

#endif // _Include_DirectX11Adapter_hpp_
