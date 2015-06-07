/*************************************************************
*	@file   	DirectX11Shader.hpp
*	@brief  	DirectX11Shader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/08
*************************************************************/

#ifndef _Include_DirectX11Shader_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11Shader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<map>
#include<vector>
#include<array>
#include<list>
#include<d3dcompiler.h>
#include<d3d11shader.h>
#include"DirectX11BaseApi.hpp"
#include"../Resource/ShaderResource.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11Shader
	//	@brief  �V�F�[�_�[�Ǘ��N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/08
	------------------------------------------------------------*/
	class DirectX11Shader : public DirectX11BaseApi
	{
	public:
		DirectX11Shader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11Shader()override final;

		/*!-----------------------------------------------------------
		//	@brief		���_�V�F�[�_�[�̓ǂݍ���
		//	@note		���_���C�A�E�g�̎w�肪�Ȃ���Ύ����擾
		//	@param[in]	_filePath			�ǂݍ��ރt�@�C���p�X
		//	@param[in]	_entryPoint			�G���g���[�|�C���g��[�ȗ���]
		//	@param[in]	_target				SM�o�[�W������[�ȗ���]
		//	@param[in]	_pInclude			�C���N���[�h�w��|�C���^[�ȗ���]
		//	@param[in]	_pDefines			��`�}�N���z��|�C���^[�ȗ���]
		//	@param[in]	_pDesc				�蓮���_���C�A�E�g�|�C���^[�ȗ���]
		//	@param[in]	_size				�蓮���_���C�A�E�g�T�C�Y[�ȗ���]
		//	@param[in]	_flags1				�R���p�C���I�v�V�����t���O�P[�ȗ���]
		//	@param[in]	_flags2				�R���p�C���I�v�V�����t���O�Q[�ȗ���]
		//	@return		���_�V�F�[�_�[�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		VertexShader*	LoadVertexShaderFromFile(
			LPCWSTR _filePath,
			LPCSTR _entryPoint = "main",
			LPCSTR _target = "vs_5_0",
			ID3DInclude* const _pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE,
			const D3D_SHADER_MACRO* _pDefines = nullptr,
			const D3D11_INPUT_ELEMENT_DESC* _pDesc = nullptr,
			const UINT _size = 0,
			const UINT _flags1 = 0,
			const UINT _flags2 = 0);

		/*!-----------------------------------------------------------
		//	@brief		�W�I���g���V�F�[�_�[�̓ǂݍ���
		//	@param[in]	_filePath			�ǂݍ��ރt�@�C���p�X
		//	@param[in]	_entryPoint			�G���g���[�|�C���g��[�ȗ���]
		//	@param[in]	_target				SM�o�[�W������[�ȗ���]
		//	@param[in]	_pInclude			�C���N���[�h�w��|�C���^[�ȗ���]
		//	@param[in]	_pDefines			��`�}�N���z��|�C���^[�ȗ���]
		//	@param[in]	_Flags1				�R���p�C���I�v�V�����t���O�P[�ȗ���]
		//	@param[in]	_Flags2				�R���p�C���I�v�V�����t���O�Q[�ȗ���]
		//	@return		�W�I���g���V�F�[�_�[�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		GeometryShader*	LoadGeometryShaderFromFile(
			LPCWSTR _filePath,
			LPCSTR _entryPoint = "main",
			LPCSTR _target = "gs_5_0",
			ID3DInclude* const _pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE,
			const D3D_SHADER_MACRO* _pDefines = nullptr,
			const UINT _Flags1 = 0,
			const UINT _Flags2 = 0);

		/*!-----------------------------------------------------------
		//	@brief		�s�N�Z���V�F�[�_�[�ǂݍ���
		//	@param[in]	_filePath			�ǂݍ��ރt�@�C���p�X
		//	@param[in]	_entryPoint			�G���g���[�|�C���g��[�ȗ���]
		//	@param[in]	_target				SM�o�[�W������[�ȗ���]
		//	@param[in]	_pInclude			�C���N���[�h�w��|�C���^[�ȗ���]
		//	@param[in]	_pDefines			��`�}�N���z��|�C���^[�ȗ���]
		//	@param[in]	_Flags1				�R���p�C���I�v�V�����t���O�P[�ȗ���]
		//	@param[in]	_Flags2				�R���p�C���I�v�V�����t���O�Q[�ȗ���]
		//	@return		�s�N�Z���V�F�[�_�[�|�C���^�n���h��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		PixelShader*	LoadPixelShaderFromFile(
			LPCWSTR _filePath,
			LPCSTR _entryPoint = "main",
			LPCSTR _target = "ps_5_0",
			ID3DInclude* const _pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE,
			const D3D_SHADER_MACRO* _pDefines = nullptr,
			const UINT _Flags1 = 0,
			const UINT _Flags2 = 0);

		/*!-----------------------------------------------------------
		//	@brief		�R���s���[�g�V�F�[�_�[�̓ǂݍ���
		//	@param[in]	_filePath			�ǂݍ��ރt�@�C���p�X
		//	@param[in]	_entryPoint		�G���g���[�|�C���g��[�ȗ���]
		//	@param[in]	_target			SM�o�[�W������[�ȗ���]
		//	@param[in]	_pInclude			�C���N���[�h�w��|�C���^[�ȗ���]
		//	@param[in]	_pDefines			��`�}�N���z��|�C���^[�ȗ���]
		//	@param[in]	_Flags1				�R���p�C���I�v�V�����t���O�P[�ȗ���]
		//	@param[in]	_Flags2				�R���p�C���I�v�V�����t���O�Q[�ȗ���]
		//	@return		�R���s���[�g�V�F�[�_�[�|�C���^�n���h��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		ComputeShader*	LoadComputeShaderFromFile(
			LPCWSTR _filePath,
			LPCSTR _entryPoint = "main",
			LPCSTR _target = "cs_5_0",
			ID3DInclude* const _pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE,
			const D3D_SHADER_MACRO* _pDefines = nullptr,
			const UINT _Flags1 = 0,
			const UINT _Flags2 = 0);

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
		UavBuffer*	CreateUnorderedAccessBuffer(
			const D3D11_USAGE _usage,
			const D3D11_RESOURCE_MISC_FLAG _miscFlag,
			const UINT _elementSize,
			const UINT _nCount,
			const void* _pInitData = nullptr,
			const D3D11_BIND_FLAG _addBindFlag = (D3D11_BIND_FLAG)0);

		/*!-----------------------------------------------------------
		//	@brief		���_�V�F�[�_�[���\�[�X���
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		void	ReleaseVertexShader();

		/*!-----------------------------------------------------------
		//	@brief		�W�I���g���V�F�[�_�[���\�[�X���
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		void	ReleaseGeometryShader();

		/*!-----------------------------------------------------------
		//	@brief		�s�N�Z���V�F�[�_�[���\�[�X���
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		void	ReleasePixelShader();

		/*!-----------------------------------------------------------
		//	@brief		�R���s���[�g�V�F�[�_�[���\�[�X���
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		void	ReleaseComputeShader();

		/*!-----------------------------------------------------------
		//	@brief		�A���I�[�_�[�A�N�Z�X�o�b�t�@���
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/15
		------------------------------------------------------------*/
		void	ReleaseUavBuffer();

	private:
		std::map<string, VertexShader*>		pVertexShaderFileMap;
		std::map<string, GeometryShader*>	pGeometryShaderFileMap;
		std::map<string, PixelShader*>		pPixelShaderFileMap;
		std::map<string, ComputeShader*>	pComputeShaderFileMap;
		std::list<UavBuffer*> pUavList;

	private:
		/*!-----------------------------------------------------------
		//	@brief		���_���C�A�E�g�̎蓮�쐬
		//	@param[out]	_ppOut	�o�͂��钸�_���C�A�E�g
		//	@param[in]	_pBlob	�R���p�C�����ꂽ�V�F�[�_�[�f�[�^
		//	@param[in]	_desc	���͂��钸�_�������
		//	@param[in]	_size	���͂��钸�_�������T�C�Y
		//	@return		S_OK�F�쐬����	����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		HRESULT	CreateManualInputLayout(
			ID3D11InputLayout** _ppOut,
			ID3DBlob* _pBlob,
			CONST D3D11_INPUT_ELEMENT_DESC* _pDesc,
			UINT _size);

		/*!-----------------------------------------------------------
		//	@brief		���_���C�A�E�g����������
		//	@param[in]	_ppOut	�o�͂��钸�_���C�A�E�g
		//	@return		_pBlob	�V�F�[�_�[�o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		HRESULT	CreateAutoInputLayout(
			ID3D11InputLayout** _ppOut,
			ID3DBlob* _pBlob);
	};
}

#endif // _Include_DirectX11Shader_hpp_