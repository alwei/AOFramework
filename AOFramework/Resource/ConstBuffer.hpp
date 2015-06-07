/*************************************************************
*	@file   	ConstBuffer.hpp
*	@brief  	ConstBuffer�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/03/17
*************************************************************/

#ifndef _Include_ConstBuffer_hpp_	// �C���N���[�h�K�[�h
#define _Include_ConstBuffer_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<D3D11.h>
#include"Buffer.hpp"
#include"../Utility/SmartComPtr.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  ConstBuffer
	//	@brief  �e���v���[�g�萔�o�b�t�@�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/03/17
	------------------------------------------------------------*/
	template< class T1>
	class ConstBuffer final
	{
	public:
		/// �R���X�g���N�^
		ConstBuffer(){
			::SecureZeroMemory(&this->mapData, sizeof(T1));
		}

		/// �f�X�g���N�^
		~ConstBuffer(){}

		/*!-----------------------------------------------------------
		//	@brief		�|�C���^�o�^
		//	@param[in]	_pd3dDevice			�f�o�C�X�|�C���^
		//	@param[in]	_pd3dDeviceContext	�f�o�C�X�R���e�L�X�g�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/17
		------------------------------------------------------------*/
		void	Register(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext){
			this->pd3dDevice = _pd3dDevice;
			this->pd3dDeviceContext = _pd3dDeviceContext;
		}

		/*!-----------------------------------------------------------
		//	@brief		�o�b�t�@����
		//	@param[in]	_usage			�ȗ��F�g�p�p�r(D3D11_USAGE_DEFAULT:D3D11_USAGE_IMMUTABLE:D3D11_USAGE_DYNAMIC:D3D11_USAGE_STAGIN)
		//	@param[in]	_cpuAccessFlag	�ȗ��FCPU�A�N�Z�X�t���O(D3D11_CPU_ACCESS_READ:D3D11_CPU_ACCESS_WRITE)
		//	@return		S_OK�F���������@����ȊO�F�������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/17
		------------------------------------------------------------*/
		HRESULT		Create(
			const D3D11_USAGE _usage = D3D11_USAGE_DYNAMIC,
			const UINT _cpuAccessFlag = D3D11_CPU_ACCESS_WRITE){
			return this->gpuBuffer.Create(&this->mapData, sizeof(T1), _usage, _cpuAccessFlag, this->pd3dDevice);
		}

		/*!-----------------------------------------------------------
		//	@brief		VertexShader�̒萔�Ƀ}�b�s���O
		//	@param[in]	_slotId			�X���b�g�ԍ�[�ȗ���]
		//	@param[in]	_numBuffer		�o�b�t�@��[�ȗ���]
		//	@param[in]	_mapFlag		�}�b�v�t���O[�ȗ���]
		//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/17
		------------------------------------------------------------*/
		HRESULT	MapVertex(
			const UINT _slotId = 0,
			const UINT _numBuffer = 1,
			const D3D11_MAP _mapFlag = D3D11_MAP_WRITE_DISCARD)
		{
			HRESULT hr = this->gpuBuffer.Map(&this->mapData, sizeof(T1), _mapFlag, this->pd3dDeviceContext);
			this->pd3dDeviceContext->VSSetConstantBuffers(_slotId, _numBuffer, this->gpuBuffer.buffer.GetComPtrPtr());
			return hr;
		}

		/*!-----------------------------------------------------------
		//	@brief		GeometryShader�̒萔�Ƀ}�b�s���O
		//	@param[in]	_slotId			�X���b�g�ԍ�[�ȗ���]
		//	@param[in]	_numBuffer		�o�b�t�@��[�ȗ���]
		//	@param[in]	_mapFlag		�}�b�v�t���O[�ȗ���]
		//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/17
		------------------------------------------------------------*/
		HRESULT	MapGeometry(
			const UINT _slotId = 0,
			const UINT _numBuffer = 1,
			const D3D11_MAP _mapFlag = D3D11_MAP_WRITE_DISCARD)
		{
			HRESULT hr = this->gpuBuffer.Map(&this->mapData, sizeof(T1), _mapFlag, this->pd3dDeviceContext);
			this->pd3dDeviceContext->GSSetConstantBuffers(_slotId, _numBuffer, this->gpuBuffer.buffer.GetComPtrPtr());
			return hr;
		}

		/*!-----------------------------------------------------------
		//	@brief		PixelShader�̒萔�Ƀ}�b�s���O
		//	@param[in]	_slotId				�X���b�g�ԍ�[�ȗ���]
		//	@param[in]	_numBuffer			�o�b�t�@��[�ȗ���]
		//	@param[in]	_mapFlag			�}�b�v�t���O[�ȗ���]
		//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/17
		------------------------------------------------------------*/
		HRESULT	MapPixel(
			const UINT _slotId = 0,
			const UINT _numBuffer = 1,
			const D3D11_MAP _mapFlag = D3D11_MAP_WRITE_DISCARD)
		{
			HRESULT hr = this->gpuBuffer.Map(&this->mapData, sizeof(T1), _mapFlag, this->pd3dDeviceContext);
			this->pd3dDeviceContext->PSSetConstantBuffers(_slotId, _numBuffer, this->gpuBuffer.buffer.GetComPtrPtr());
			return hr;
		}

		/*!-----------------------------------------------------------
		//	@brief		ComputeShader�̒萔�Ƀ}�b�s���O
		//	@param[in]	_slotId				�X���b�g�ԍ�[�ȗ���]
		//	@param[in]	_numBuffer			�o�b�t�@��[�ȗ���]
		//	@param[in]	_mapFlag			�}�b�v�t���O[�ȗ���]
		//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/17
		------------------------------------------------------------*/
		HRESULT	MapCompute(
			const UINT _slotId = 0,
			const UINT _numBuffer = 1,
			const D3D11_MAP _mapFlag = D3D11_MAP_WRITE_DISCARD)
		{
			HRESULT hr = this->gpuBuffer.Map(&this->mapData, sizeof(T1), _mapFlag, this->pd3dDeviceContext);
			this->pd3dDeviceContext->CSSetConstantBuffers(_slotId, _numBuffer, this->gpuBuffer.buffer.GetComPtrPtr());
			return hr;
		}

		/*!-----------------------------------------------------------
		//	@brief		��r���Z�q�̃I�[�o�[���[�h
		//	@return		true�F��v�@false�F�s��v
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/22
		------------------------------------------------------------*/
		bool operator==(int addr){
			return ((int)&this->mapData == addr);
		}

		/*!-----------------------------------------------------------
		//	@brief		��r���Z�q�̃I�[�o�[���[�h
		//	@return		true�F�s��v�@false�F��v
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/22
		------------------------------------------------------------*/
		bool operator!=(int addr){
			return ((int)&this->mapData != addr);
		}

		/*!-----------------------------------------------------------
		//	@brief		��r���Z�q�̃I�[�o�[���[�h
		//	@return		true�F��v�@false�F�s��v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		bool operator==(void* addr){
			return ((int)&this->mapData == (int)addr);
		}

		/*!-----------------------------------------------------------
		//	@brief		��r���Z�q�̃I�[�o�[���[�h
		//	@return		true�F�s��v�@false�F��v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		bool operator!=(void* addr){
			return ((int)&this->mapData != (int)addr);
		}

		/*!-----------------------------------------------------------
		//	@brief		�A���[���Z�q�̃I�[�o�[���[�h
		//	@return		�}�b�v�f�[�^�̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/22
		------------------------------------------------------------*/
		T1* operator->(){
			return &this->mapData;
		}

		/*!-----------------------------------------------------------
		//	@brief		�ԐڎQ�Ɖ��Z�q�̃I�[�o�[���[�h
		//	@return		�}�b�v�f�[�^�̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/22
		------------------------------------------------------------*/
		T1& operator*(){
			return this->mapData;
		}

		/*!-----------------------------------------------------------
		//	@brief		�z��Q�Ɖ��Z�q�̃I�[�o�[���[�h
		//	@return		�}�b�v�f�[�^�̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/22
		------------------------------------------------------------*/
		T1& operator[](int n) const{
			return this->mapData[n];
		}

		/*!-----------------------------------------------------------
		//	@brief		&���Z�q�̃I�[�o�[���[�h
		//	@return		�}�b�v�f�[�^�̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/22
		------------------------------------------------------------*/
		T1* operator&(){
			return &this->mapData;
		}

		/*!-----------------------------------------------------------
		//	@brief		&���Z�q�̃I�[�o�[���[�h
		//	@return		�}�b�v�f�[�^�̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		T1* operator&(const T1& _inst){
			return &_inst;
		}

	private:
		T1	mapData;
		Buffer gpuBuffer;
		SmartComPtr<ID3D11Device> pd3dDevice;
		SmartComPtr<ID3D11DeviceContext> pd3dDeviceContext;

	private:
		ConstBuffer(const ConstBuffer& inst) = delete;		///< �R�s�[�R���X�g���N�^�֎~
		explicit ConstBuffer(T1 type) = delete;				///< �Öق̌^�ϊ��ɂ��R���X�g���N�^�֎~
		void operator=(T1 pInst) = delete;					///< �����I�ȑ�����Z�q�֎~
		void operator=(const ConstBuffer& inst) = delete;	///< �����I�ȑ�����Z�q�֎~
	};
}

#endif // _Include_ConstBuffer_hpp_