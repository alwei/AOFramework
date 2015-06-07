/*************************************************************
*	@file   	DirectX11Viewport.hpp
*	@brief  	DirectX11Viewport�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/07/16
*************************************************************/

#ifndef _Include_DirectX11Viewport_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11Viewport_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"DirectX11Header.hpp"
#include"DirectX11BaseApi.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11Viewport
	//	@brief  �r���[�|�[�g�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/16
	------------------------------------------------------------*/
	class DirectX11Viewport : public DirectX11BaseApi
	{
	public:
		DirectX11Viewport(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11Viewport()override final;

		/*!-----------------------------------------------------------
		//	@brief		�쐬
		//	@param[in]	_nWidth		����
		//	@param[in]  _nHeight	�c��
		//	@param[in]	_topLeftX	X���̌��_[�ȗ���]
		//	@param[in]	_topLeftY	Y���̌��_[�ȗ���]
		//	@param[in]	_minDepth	�ŏ��[�x[�ȗ���]
		//	@param[in]	_maxDeppth	�ő�[�x[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		void	Create(
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST INT _topLeftX = 0,
			CONST INT _topLeftY = 0,
			CONST FLOAT _minDpeth = 0.0f,
			CONST FLOAT _maxDpeth = 1.0f);

		/*!-----------------------------------------------------------
		//	@brief		�ݒ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		void	SetViewport()
		{
			this->pd3dDeviceContext->RSSetViewports(1, &this->viewport);
		}

	public:
		D3D11_VIEWPORT	viewport;
	};
}

#endif // _Include_DirectX11Viewport_hpp_
