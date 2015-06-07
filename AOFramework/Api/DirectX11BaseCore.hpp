/*************************************************************
*	@file   	DirectX11BaseCore.hpp
*	@brief  	DirectX11BaseCore�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/04/27
*************************************************************/

#ifndef _Include_DirectX11BaseCore_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11BaseCore_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"DirectX11Header.hpp"
#include"../Define/SystemDefine.hpp"
#include"../Utility/SmartComPtr.hpp"
#include"../Debug/DebugSystem.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11BaseCore
	//	@brief  ID3D11Device�����R�A�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/27
	------------------------------------------------------------*/
	class DirectX11BaseCore
	{
	public:
		/// �R���X�g���N�^
		DirectX11BaseCore(SmartComPtr<ID3D11Device> _pd3dDevice) : pd3dDevice(_pd3dDevice){};

		/// �f�X�g���N�^
		virtual ~DirectX11BaseCore(){};

		/*!-----------------------------------------------------------
		//	@brief		�f�o�C�X�擾
		//	@return		�f�o�C�X�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		SmartComPtr<ID3D11Device> GetDevice()
		{
			return this->pd3dDevice;
		}

	protected:
		SmartComPtr<ID3D11Device> pd3dDevice;
	};
};

#endif // _Include_DirectX11BaseCore_hpp_