/*************************************************************
*	@file   	DirectX11BaseApi.hpp
*	@brief  	DirectX11BaseApi�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/06/27
*************************************************************/

#ifndef _Include_DirectX11BaseApi_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11BaseApi_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"DirectX11BaseCore.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11BaseApi
	//	@brief  11��pAPI���N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/06/26
	------------------------------------------------------------*/
	class DirectX11BaseApi : public DirectX11BaseCore
	{
	public:
		DirectX11BaseApi(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) : 
			pd3dDeviceContext(_pd3dDeviceContext),
			DirectX11BaseCore(_pd3dDevice){}
		virtual	~DirectX11BaseApi(){}

		/*!-----------------------------------------------------------
		//	@brief		�f�o�C�X�R���e�L�X�g�擾
		//	@return		�f�o�C�X�R���e�L�X�g�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		SmartComPtr<ID3D11DeviceContext> GetDeviceContext()
		{
			return this->pd3dDeviceContext;
		}

	protected:
		SmartComPtr<ID3D11DeviceContext>	pd3dDeviceContext;	///< �f�o�C�X�R���e�L�X�g�|�C���^
	};
}

#endif // _Include_DirectX11BaseApi_hpp_
