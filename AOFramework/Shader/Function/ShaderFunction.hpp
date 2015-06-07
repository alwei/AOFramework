/*************************************************************
*	@file   	ShaderFunction.hpp
*	@brief  	�V�F�[�_�[����֐��Q
*	@author		Tatsunori Aoyama
*	@date		2014/08/10
*************************************************************/
#ifndef _Include_ShaderFunction_hpp_	// �C���N���[�h�K�[�h
#define _Include_ShaderFunction_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<vector>
#include<d3d11.h>
#include<d3d11shader.h>
#include"../../Define/SystemDefine.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@brief		������V�F�[�_�[�̃R���p�C��
	//	@param[out]	_ppOutBlob			�o�͂���Blob�|�C���^
	//	@param[in]	_pSrcData			�f�[�^�|�C���^
	//	@param[in]	_SrcDataSize		�f�[�^�T�C�Y
	//	@param[in]	_pEntrypoint		�G���g���[��
	//	@param[in]	_pTarget			�o�[�W������
	//	@param[in]	_pDefines			�V�F�[�_�[�}�N���z��[�ȗ���]
	//	@param[in]	_pInclude			�C���N���[�h�|�C���^[�ȗ���]
	//	@param[out]	_ppErrorMsgs		�o�͂���G���[�|�C���^[�ȗ���]
	//	@return		S_OK�F�R���p�C�������@����ȊO�F�R���p�C�����s
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CompileStringShader(
		ID3DBlob** _ppOutBlob,
		void* const _pSrcData,
		UINT _SrcDataSize,
		LPCSTR _pEntrypoint,
		LPCSTR _pTarget,
		const D3D_SHADER_MACRO* const _pDefines=nullptr,
		ID3DInclude* const _pInclue=nullptr,
		ID3DBlob** _ppErrorMsgs=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		���_���C�A�E�g��1�v�f���擾
	//	@param[out]	_pInputElementDesc	�o�͂��钸�_���C�A�E�g���
	//	@param[in]	_pReflection		���t���N�V�����|�C���^
	//	@param[in]	_paramIndex			�p�����[�^�[�C���f�b�N�X
	//	@param[in]	_byteOffset			�o�C�g�I�t�Z�b�g
	//	@return		1�v�f�̃o�C�g��
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	UINT GetInputElementDesc(
		D3D11_INPUT_ELEMENT_DESC* _pInputElementDesc,
		ID3D11ShaderReflection* _pReflection,
		UINT _paramIndex,
		UINT _byteOffset);

	/*!-----------------------------------------------------------
	//	@brief		���_���C�A�E�g���������o
	//	@param[in]	_pd3dDevice		�f�o�C�X�|�C���^
	//	@param[out]	_ppOut			�o�͂��钸�_���C�A�E�g
	//	@param[in]	_pBlob			�R���p�C���ς݃V�F�[�_�[
	//	@return		S_OK�F���o�����@����ȊO�F���o���s
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	 CreateAutoVertexInputLayout(
		ID3D11Device* const _pd3dDevice,
		ID3D11InputLayout**_ppOut,
		ID3DBlob* const _pBlob);

	/*!-----------------------------------------------------------
	//	@brief		���_�V�F�[�_�[�쐬
	//	@param[in]	_pd3dDevice			�f�o�C�X�|�C���^
	//	@param[out]	_ppOutShader		�o�͂���V�F�[�_�[�|�C���^
	//	@param[out]	_ppOutInputLayout	�o�͂��钸�_���C�A�E�g
	//	@param[in]	_pSrcData			�f�[�^�|�C���^
	//	@param[in]	_SrcDataSize		�f�[�^�T�C�Y
	//	@param[in]	_pEntrypoint		�G���g���[��[�ȗ���]
	//	@param[in]	_pTarget			�o�[�W������[�ȗ���]
	//	@param[in]	_pDefines			�V�F�[�_�[�}�N���z��[�ȗ���]
	//	@param[in]	_pInclude			�C���N���[�h�|�C���^[�ȗ���]
	//	@param[in]	_pClassLinkage		�N���X�����P�[�W�|�C���^[�ȗ���]
	//	@param[out]	_ppErrorMsgs		�o�͂���G���[�|�C���^[�ȗ���]
	//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateVertexShader(
		ID3D11Device* _pd3dDevice,
		ID3D11VertexShader**_ppOutShader,
		ID3D11InputLayout**_ppOutInputLayout,
		void* const _pSrcData,
		UINT _SrcDataSize,
		LPCSTR _pEntrypoint="main",
		LPCSTR _pTarget="vs_5_0",
		const D3D_SHADER_MACRO* const _pDefines=nullptr,
		ID3DInclude* const _pInclude=nullptr,
		ID3D11ClassLinkage* _pClassLinkage=nullptr,
		ID3DBlob**_ppErrorMsgs=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		�W�I���g���V�F�[�_�[�쐬
	//	@param[in]	_pd3dDevice			�f�o�C�X�|�C���^
	//	@param[out]	_ppOutShader		�o�͂���V�F�[�_�[�|�C���^
	//	@param[in]	_pSrcData			�f�[�^�|�C���^
	//	@param[in]	_SrcDataSize		�f�[�^�T�C�Y
	//	@param[in]	_pEntrypoint		�G���g���[��[�ȗ���]
	//	@param[in]	_pTarget			�o�[�W������[�ȗ���]
	//	@param[in]	_pDefines			�V�F�[�_�[�}�N���z��[�ȗ���]
	//	@param[in]	_pInclude			�C���N���[�h�|�C���^[�ȗ���]
	//	@param[in]	_pClassLinkage		�N���X�����P�[�W�|�C���^[�ȗ���]
	//	@param[out]	_ppErrorMsgs		�o�͂���G���[�|�C���^[�ȗ���]
	//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateGeometryShader(
		ID3D11Device* _pd3dDevice,
		ID3D11GeometryShader**_ppOutShader,
		void* const _pSrcData,
		UINT _SrcDataSize,
		LPCSTR _pEntrypoint="main",
		LPCSTR _pTarget="gs_5_0",
		const D3D_SHADER_MACRO* const _pDefines=nullptr,
		ID3DInclude* const _pInclude=nullptr,
		ID3D11ClassLinkage* _pClassLinkage=nullptr,
		ID3DBlob**_ppErrorMsgs=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		�s�N�Z���V�F�[�_�[�쐬
	//	@param[in]	_pd3dDevice			�f�o�C�X�|�C���^
	//	@param[out]	_ppOutShader		�o�͂���V�F�[�_�[�|�C���^
	//	@param[in]	_pSrcData			�f�[�^�|�C���^
	//	@param[in]	_SrcDataSize		�f�[�^�T�C�Y
	//	@param[in]	_pEntrypoint		�G���g���[��[�ȗ���]
	//	@param[in]	_pTarget			�o�[�W������[�ȗ���]
	//	@param[in]	_pDefines			�V�F�[�_�[�}�N���z��[�ȗ���]
	//	@param[in]	_pInclude			�C���N���[�h�|�C���^[�ȗ���]
	//	@param[in]	_pClassLinkage		�N���X�����P�[�W�|�C���^[�ȗ���]
	//	@param[out]	_ppErrorMsgs		�o�͂���G���[�|�C���^[�ȗ���]
	//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreatePixelShader(
		ID3D11Device* _pd3dDevice,
		ID3D11PixelShader**_ppOutShader,
		void* const _pSrcData,
		UINT _SrcDataSize,
		LPCSTR _pEntrypoint="main",
		LPCSTR _pTarget="ps_5_0",
		const D3D_SHADER_MACRO* const _pDefines=nullptr,
		ID3DInclude* const _pInclude=nullptr,
		ID3D11ClassLinkage* _pClassLinkage=nullptr,
		ID3DBlob**_ppErrorMsgs=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		�R���s���[�g�V�F�[�_�[�쐬
	//	@param[in]	_pd3dDevice			�f�o�C�X�|�C���^
	//	@param[out]	_ppOutShader		�o�͂���V�F�[�_�[�|�C���^
	//	@param[in]	_pSrcData			�f�[�^�|�C���^
	//	@param[in]	_SrcDataSize		�f�[�^�T�C�Y
	//	@param[in]	_pEntrypoint		�G���g���[��[�ȗ���]
	//	@param[in]	_pTarget			�o�[�W������[�ȗ���]
	//	@param[in]	_pDefines			�V�F�[�_�[�}�N���z��[�ȗ���]
	//	@param[in]	_pInclude			�C���N���[�h�|�C���^[�ȗ���]
	//	@param[in]	_pClassLinkage		�N���X�����P�[�W�|�C���^[�ȗ���]
	//	@param[out]	_ppErrorMsgs		�o�͂���G���[�|�C���^[�ȗ���]
	//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateComputeShader(
		ID3D11Device* const _pd3dDevice,
		ID3D11ComputeShader** const _ppOutShader,
		void* const _pSrcData,
		const UINT _SrcDataSize,
		const LPCSTR _pEntrypoint="main",
		const LPCSTR _pTarget="cs_5_0",
		const D3D_SHADER_MACRO* const _pDefines=nullptr,
		ID3DInclude* const _pInclude=nullptr,
		ID3D11ClassLinkage* _pClassLinkage=nullptr,
		ID3DBlob**_ppErrorMsgs=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		�o�b�t�@�̍쐬
	//	@param[in]	_pd3dDevice			�f�o�C�X�|�C���^
	//	@param[out]	_ppOutBuffer		�o�͂���o�b�t�@�̃_�u���|�C���^
	//	@param[in]	_pSrcData			���͂���f�[�^
	//	@param[in]	_size				���͂���f�[�^�T�C�Y
	//	@param[in]	_usage				�d�l�p�r
	//	@param[in]	_cpuAccessFlag		CPU�̃A�N�Z�X���t���O
	//	@param[in]	_bindFlag		�o�C���h�����t���O
	//	@return		S_OK�F�o�b�t�@�̍쐬�ɐ����@����ȊO�F�o�b�t�@�̍쐬�Ɏ��s
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateBuffer(
		ID3D11Device* const _pd3dDevice,
		ID3D11Buffer** const _ppOutBuffer,
		const void* const _pSrcData,
		const UINT _size,
		const D3D11_USAGE _usage,
		const UINT _cpuAccessFlag,
		const D3D11_BIND_FLAG _bindFlag);

	/*!-----------------------------------------------------------
	//	@brief		���_�o�b�t�@�̍쐬
	//	@param[in]	_pd3dDevice	�f�o�C�X�|�C���^
	//	@param[out]	_ppOutBuffer		�o�͂���o�b�t�@�̃_�u���|�C���^
	//	@param[in]	_size				���͂���f�[�^�T�C�Y
	//	@param[in]	_pSrcData			���͂���f�[�^[�ȗ���]
	//	@param[in]	_usage				�d�l�p�r[�ȗ���]
	//	@param[in]	_cpuAccessFlag		CPU�̃A�N�Z�X���t���O[�ȗ���]
	//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateVertexBuffer(
		ID3D11Device* const _pd3dDevice,
		ID3D11Buffer** const _ppOutBuffer,
		const UINT _size,
		const void* const _pSrcData = nullptr,
		const D3D11_USAGE _usage = D3D11_USAGE_DEFAULT,
		const UINT _cpuAccessFlag = FALSE);

	/*!-----------------------------------------------------------
	//	@brief		�C���f�b�N�X�o�b�t�@�̍쐬
	//	@param[in]	_pd3dDevice	�f�o�C�X�|�C���^
	//	@param[out]	_ppOutBuffer		�o�͂���o�b�t�@�̃_�u���|�C���^
	//	@param[in]	_size				���͂���f�[�^�T�C�Y
	//	@param[in]	_pSrcData			���͂���f�[�^[�ȗ���]
	//	@param[in]	_usage				�d�l�p�r[�ȗ���]
	//	@param[in]	_cpuAccessFlag		CPU�̃A�N�Z�X���t���O[�ȗ���]
	//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateIndexBuffer(
		ID3D11Device* const _pd3dDevice,
		ID3D11Buffer** const _ppOutBuffer,
		const UINT _size,
		const void* const _pSrcData,
		const D3D11_USAGE _usage = D3D11_USAGE_DEFAULT,
		const UINT _cpuAccessFlag = FALSE);

	/*!-----------------------------------------------------------
	//	@brief		�萔�o�b�t�@�̍쐬
	//	@param[in]	_pd3dDevice			�f�o�C�X�|�C���^
	//	@param[out]	_ppOutBuffer		�o�͂���o�b�t�@�̃_�u���|�C���^
	//	@param[in]	_size				���͂���f�[�^�T�C�Y
	//	@param[in]	_pSrcData			���͂���f�[�^[�ȗ���]
	//	@param[in]	_usage				�d�l�p�r[�ȗ���]
	//	@param[in]	_cpuAccessFlag		CPU�̃A�N�Z�X���t���O[�ȗ���]
	//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateConstantBuffer(
		ID3D11Device* const _pd3dDevice,
		ID3D11Buffer** const _ppOutBuffer,
		const UINT _size,
		const void* const _pSrcData = nullptr,
		const D3D11_USAGE _usage = D3D11_USAGE_DYNAMIC,
		const UINT _cpuAccessFlag = D3D10_CPU_ACCESS_WRITE);

	/*!-----------------------------------------------------------
	//	@brief		�A���I�[�_�[�A�N�Z�X�o�b�t�@�̍쐬
	//	@note		���_�o�b�t�@��D3D11_RESOURCE_MISC_BUFFER_STRUCTURED���w��ł��Ȃ�
	//	@param[in]	_pd3dDevice		�f�o�C�X�|�C���^
	//	@param[out]	_ppOutBuffer	�o�͂���o�b�t�@
	//	@param[out]	_ppOutUaView	�o�͂���A�N�Z�X�r���[
	//	@param[in]	_usage			�g�p�p�r
	//	@param[in]	_miscFlag		���\�[�X�t���O
	//	@param[in]	_elementSize	�v�f�T�C�Y
	//	@param[in]	_nCount			�v�f��
	//	@param[in]	_pInitData		�������f�[�^[�ȗ���]
	//	@param[in]	_addFkag		�ǉ��t���O[�ȗ���]
	//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
	//	@author		Tatsunori Aoyama
	//	@date		2014/11/15
	------------------------------------------------------------*/
	HRESULT	CreateUnorderedAccessBuffer(
		ID3D11Device* const _pd3dDevice,
		ID3D11Buffer** const _ppOutBuffer,
		ID3D11UnorderedAccessView** const _ppOutUaView,
		const D3D11_RESOURCE_MISC_FLAG _miscFlag,
		const UINT _elementSize,
		const UINT _nCount,
		const void* _pInitData = nullptr,
		const D3D11_BIND_FLAG _addBindFlag = (D3D11_BIND_FLAG)0);

	/*!-----------------------------------------------------------
	//	@brief		�Ώۃr���[����V�F�[�_�[���\�[�X�r���[���擾
	//	@param[in]	_pd3dDevice		�g�p����f�o�C�X�|�C���^
	//	@param[out]	_ppOutSrView	�o�͂���V�F�[�_�[���\�[�X�r���[
	//	@param[in]	_pView			���o���郌���_�[�^�[�Q�b�g�r���[
	//	@param[in]  _pDesc			�V�F�[�_�[���\�[�X�r���[���
	//	@return		S_OK�F�擾�����@����ȊO�F�擾���s
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	template<class T>
	inline	HRESULT	GetShaderResourceView(
		ID3D11Device* const _pd3dDevice,
		ID3D11ShaderResourceView** const _ppOutSrView,
		T* const _pView,
		const D3D11_SHADER_RESOURCE_VIEW_DESC* const _pDesc = nullptr
		)
	{
		SmartComPtr<ID3D11Resource> pTemp;
		_pView->GetResource(pTemp.ToCreator());
		HRESULT hr = _pd3dDevice->CreateShaderResourceView( pTemp.GetComPtr(),_pDesc,_ppOutSrView );
#ifdef __AO_DEBUG__
		if( FAILED( hr ) )
			ao::debug::PrintfColor(ConsoleColor::H_RED,_T("[GetShaderResourceView]Failed create shader resource view.\n"));
#endif
		return hr;
	}

	/*!-----------------------------------------------------------
	//	@brief		���_�o�b�t�@�̏�������
	//	@param[in]	_pd3dDeviceContext	�f�o�C�X�R���e�L�X�g�|�C���^
	//	@param[in]	pBuffer				�o�b�t�@�|�C���^
	//	@param[in]	_pSrcData			���f�[�^�|�C���^
	//	@param[in]	_size				���f�[�^�T�C�Y
	//	@author		Tatsunori Aoyama
	//	@date		2013/08/15
	------------------------------------------------------------*/
	inline	void	OverWriteVertexBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		ID3D11Buffer* const _pBuffer,
		const void* const _pSrcData,
		const UINT _size
		)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		_pd3dDeviceContext->Map( _pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
		::CopyMemory( mappedResource.pData, _pSrcData, _size );
		_pd3dDeviceContext->Unmap( _pBuffer, 0 );
	}

	/*!-----------------------------------------------------------
	//	@brief		���_�V�F�[�_�[�̒萔�o�b�t�@���X�V
	//	@note		GPU�̃A�N�Z�X���ۂ��Ȃ�(���\�[�X�͓��I�ɍ쐬���邱��)
	//	@param[in]	_pd3dDeviceContext	�f�o�C�X�R���e�L�X�g�|�C���^
	//	@param[in]	_slotId				�X���b�g�ԍ�
	//	@param[in]	_numBuffer			�o�b�t�@�̐�
	//	@param[in]	_ppBuffer			�ݒ肷��o�b�t�@
	//	@param[in]	_pSrcData			�ݒ肷��f�[�^�\�[�X
	//	@param[in]	_pDstBox			�T�u���\�[�X�̒�`BOX[�ȗ���]
	//	@author		Tatsunori Aoyama
	//	@date		2013/06/16
	------------------------------------------------------------*/
	void	UpdateVertexConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const D3D11_BOX* const _pDstBox=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		�W�I���g���V�F�[�_�[�̒萔�o�b�t�@���X�V
	//	@note		GPU�̃A�N�Z�X���ۂ��Ȃ�(���\�[�X�͓��I�ɍ쐬���邱��)
	//	@param[in]	_slotId				�X���b�g�ԍ�
	//	@param[in]	_numBuffer			�o�b�t�@�̐�
	//	@param[in]	_ppBuffer			�ݒ肷��o�b�t�@
	//	@param[in]	_pSrcData			�\�[�X���ƂȂ�f�[�^
	//	@param[in]	_pDstBox			�T�u���\�[�X�̒�`BOX[�ȗ���]
	//	@author		Tatsunori Aoyama
	//	@date		2013/08/07
	------------------------------------------------------------*/
	void	UpdateGeometryConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const D3D11_BOX* const _pDstBox=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		�s�N�Z���V�F�[�_�[�̒萔�o�b�t�@���X�V
	//	@note		GPU�̃A�N�Z�X���ۂ��Ȃ�(���\�[�X�͓��I�ɍ쐬���邱��)
	//	@param[in]	_slotId				�X���b�g�ԍ�
	//	@param[in]	_numBuffer			�o�b�t�@�̐�
	//	@param[in]	_ppBuffer			�ݒ肷��o�b�t�@
	//	@param[in]	_pSrcData			�\�[�X���ƂȂ�f�[�^
	//	@param[in]	_pDstBox			�T�u���\�[�X�̒�`BOX[�ȗ���]
	//	@author		Tatsunori Aoyama
	//	@date		2013/08/07
	------------------------------------------------------------*/
	void	UpdatePixelConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const D3D11_BOX* const _pDstBox=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		�R���s���[�g�V�F�[�_�[�̒萔�o�b�t�@���X�V
	//	@note		GPU�̃A�N�Z�X���ۂ��Ȃ�(���\�[�X�͓��I�ɍ쐬���邱��)
	//	@param[in]	_slotId				�X���b�g�ԍ�
	//	@param[in]	_numBuffer			�o�b�t�@�̐�
	//	@param[in]	_ppBuffer			�ݒ肷��o�b�t�@
	//	@param[in]	_pSrcData			�\�[�X���ƂȂ�f�[�^
	//	@param[in]	_pDstBox			�T�u���\�[�X�̒�`BOX[�ȗ���]
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	void	UpdateComputeConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const D3D11_BOX* const _pDstBox = nullptr);

	/*!-----------------------------------------------------------
	//	@brief		���_�V�F�[�_�[�̒萔�o�b�t�@���X�V
	//	@note		MAP�ɂ��GPU�A�N�Z�X���ۂ���
	//	@param[in]	_pd3dDeviceContext	MAP����f�o�C�X�R���e�L�X�g��
	//	@param[in]	_slotId				�X���b�g�ԍ�
	//	@param[in]	_numBuffer			�o�b�t�@�̐�
	//	@param[in]	_ppBuffer			�ݒ肷��o�b�t�@
	//	@param[in]	_pSrcData			�\�[�X���ƂȂ�f�[�^
	//	@param[in]	_SrcDataSize		�\�[�X���̃f�[�^�T�C�Y
	//	@param[in]	_mapFlag			MAP�t���O[�ȗ���]
	//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
	//	@author		Tatsunori Aoyama
	//	@date		2013/07/12
	------------------------------------------------------------*/
	HRESULT	MapVertexConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const UINT _SrcDataSize,
		const D3D11_MAP _mapFlag=D3D11_MAP_WRITE_DISCARD);

	/*!-----------------------------------------------------------
	//	@brief		�W�I���g���V�F�[�_�[�̒萔�o�b�t�@���X�V
	//	@note		MAP�ɂ��GPU�A�N�Z�X���ۂ���
	//	@param[in]	_pd3dDeviceContext	MAP����f�o�C�X�R���e�L�X�g��
	//	@param[in]	_slotId				�X���b�g�ԍ�
	//	@param[in]	_numBuffer			�o�b�t�@�̐�
	//	@param[in]	_ppBuffer			�ݒ肷��o�b�t�@
	//	@param[in]	_pSrcData			�\�[�X���ƂȂ�f�[�^
	//	@param[in]	_SrcDataSize		�\�[�X���̃f�[�^�T�C�Y
	//	@param[in]	_mapFlag			MAP�t���O[�ȗ���]
	//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
	//	@author		Tatsunori Aoyama
	//	@date		2013/07/12
	------------------------------------------------------------*/
	HRESULT	MapGeometryConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const UINT _SrcDataSize,
		const D3D11_MAP _mapFlag=D3D11_MAP_WRITE_DISCARD);

	/*!-----------------------------------------------------------
	//	@brief		�s�N�Z���V�F�[�_�[�̒萔�o�b�t�@���X�V
	//	@note		MAP�ɂ��GPU�A�N�Z�X���ۂ���
	//	@param[in]	_pd3dDeviceContext	MAP����f�o�C�X�R���e�L�X�g��
	//	@param[in]	_slotId				�X���b�g�ԍ�
	//	@param[in]	_numBuffer			�o�b�t�@�̐�
	//	@param[in]	_ppBuffer			�ݒ肷��o�b�t�@
	//	@param[in]	_pSrcData			�\�[�X���ƂȂ�f�[�^
	//	@param[in]	_SrcDataSize		�\�[�X���̃f�[�^�T�C�Y
	//	@param[in]	_mapFlag			MAP�t���O1[�ȗ���]
	//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
	//	@author		Tatsunori Aoyama
	//	@date		2013/08/07
	------------------------------------------------------------*/
	HRESULT	MapPixelConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const UINT _SrcDataSize,
		const D3D11_MAP _mapFlag=D3D11_MAP_WRITE_DISCARD);

	/*!-----------------------------------------------------------
	//	@brief		�R���s���[�g�V�F�[�_�[�̒萔�o�b�t�@���X�V
	//	@param[in]	_pd3dDeviceContext	MAP����f�o�C�X�R���e�L�X�g��
	//	@param[in]	_slotId				�X���b�g�ԍ�
	//	@param[in]	_numBuffer			�o�b�t�@�̐�
	//	@param[in]	_ppBuffer			�ݒ肷��o�b�t�@
	//	@param[in]	_pSrcData			�\�[�X���ƂȂ�f�[�^
	//	@param[in]	_SrcDataSize		�\�[�X���̃f�[�^�T�C�Y
	//	@param[in]	_mapFlag			MAP�t���O[�ȗ���]
	//	@return		S_OK�F�}�b�s���O�����@����ȊO�F�}�b�s���O���s
	//	@author		Tatsunori Aoyama
	//	@date		2013/11/04
	------------------------------------------------------------*/
	HRESULT	MapComputeConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const UINT _SrcDataSize,
		const D3D11_MAP _mapFlag = D3D11_MAP_WRITE_DISCARD);

	/*!-----------------------------------------------------------
	//	@brief		�e�N�X�`���̃Z�b�g
	//	@param[in]	_pd3dDeviceContext	�f�o�C�X�R���e�L�X�g�|�C���^
	//	@param[in]	_ppSrView			�V�F�[�_�[���\�[�X�r���[�|�C���^
	//	@param[in]	_ppSampState		�T���v���[�X�e�[�g�|�C���^
	//	@param[in]	_srViewSlot			�r���[�X���b�g
	//	@param[in]	_sampStateSlot		�T���v���[�X���b�g
	//	@author		Tatsunori Aoyama
	//	@date		2013/08/09
	------------------------------------------------------------*/
	void	SetTexture(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _srViewSlot = 0,
		const UINT _sampStateSlot = 0,
		ID3D11ShaderResourceView* const *_ppSrView = nullptr,
		ID3D11SamplerState* const *_ppSampState = nullptr);
}

#endif // _Include_ShaderFunction_hpp_