/*************************************************************
*	@file   	Buffer.hpp
*	@brief  	Buffer�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/12/21
*************************************************************/

#ifndef _Include_Buffer_hpp_	// �C���N���[�h�K�[�h
#define _Include_Buffer_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include<d3d11.h>
#include"../Utility/SmartComPtr.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  Buffer
	//	@brief  �o�b�t�@�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/12/21
	------------------------------------------------------------*/
	class Buffer
	{
	public:
		Buffer();
		~Buffer();

		/*!-----------------------------------------------------------
		//	@brief		�o�b�t�@����
		//	@param[in]	_pd3dDevice	�f�o�C�X�|�C���^
		//	@return		S_OK�F���������@����ȊO�F�������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		HRESULT Create(
			const void* _initSrc,
			const UINT _dataSize,
			const D3D11_USAGE _usage,
			const UINT _cpuAccessFlag,
			SmartComPtr<ID3D11Device> _pd3dDevice);

		/*!-----------------------------------------------------------
		//	@brief		�}�b�v����
		//	@param[in]	_mapSrc				�}�b�v�\�[�X
		//	@param[in]	_dataSize			�f�[�^�T�C�Y
		//	@param[in]	_pd3dDeviceContext	�f�o�C�X�R���e�L�X�g
		//	@return		S_OK�F�}�b�v�����@����ȊO�F�}�b�v���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		HRESULT	Map(
			const void* _mapSrc,
			const UINT _dataSize,
			const D3D11_MAP _mapFlag,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

	public:
		SmartComPtr<ID3D11Buffer> buffer;
	};
}

#endif // _Include_Buffer_hpp_