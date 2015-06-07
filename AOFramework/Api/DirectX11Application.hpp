/*************************************************************
*	@file   	DirectX11Application.hpp
*	@brief  	DirectX11Application�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/06/27
*************************************************************/

#ifndef _Include_DirectX11Application_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11Application_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"DirectX11Header.hpp"
#include"DirectX11BaseApi.hpp"
#include"../Utility/SmartComPtr.hpp"

namespace ao
{
	class DirectX11Adapter;
	class DirectX11EnumDisplay;
	class DirectX11RenderTarget;
	class DirectX11Viewport;

	/*!-----------------------------------------------------------
	//	@class  DirectX11Application
	//	@brief  11��p�A�v���P�[�V�����N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/06/26
	------------------------------------------------------------*/
	class DirectX11Application
	{
	public:
		DirectX11Application();

		~DirectX11Application();

		/*!-----------------------------------------------------------
		//	@brief		�f�B�X�v���C�̏�����
		//	@param[in]  _backBufferFormat	�o�b�N�o�b�t�@�t�H�[�}�b�g[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		HRESULT	InitDisplay( CONST DXGI_FORMAT _backBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT );

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	_hWnd				�E�B���h�E�n���h��
		//	@param[in]  _nWidth				�𑜓x�̕�
		//	@param[in]  _nHeight			�𑜓x�̍���
		//	@param[in]  _renderFormat		�����_�����O�t�H�[�}�b�g
		//	@param[in]  _isWindowMode		�E�B���h�E���[�h[�ȗ���]
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		HRESULT	InitDirect3D(
			CONST HWND _hWnd,
			CONST UINT _nWidth,
			CONST UINT _nHeight,
			CONST DXGI_FORMAT _renderFormat = DXGI_FORMAT_R16G16B16A16_FLOAT,
			CONST BOOL _isWindowMode = TRUE);

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E���[�h�̕ύX
		//	@param[in]	_isResize	�o�b�N�o�b�t�@�̉𑜓x�ύX�t���O[�ȗ���]
		//	@return		S_OK�F�ύX�����@����ȊO�F�ύX���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/23
		------------------------------------------------------------*/
		HRESULT	ChangeWindowMode(CONST BOOL _isResize = FALSE);

		/*!-----------------------------------------------------------
		//	@brief		�o�b�N�o�b�t�@�擾
		//	@return		�o�b�N�o�b�t�@�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/11
		------------------------------------------------------------*/
		DirectX11RenderTarget* GetBackBuffer()
		{
			return this->pBackBuffer;
		}

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

		/*!-----------------------------------------------------------
		//	@brief		�X���b�v�`�F�C���擾
		//	@return		�X���b�v�`�F�C��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/11
		------------------------------------------------------------*/
		SmartComPtr<IDXGISwapChain> GetSwapChain()
		{
			return this->swapChain;
		}

	private:
		bool	bInitDisplay;		///< �f�B�X�v���C�̏������t���O
		bool	isWindowMode;		///< �E�B���h�E���[�h
		HWND	hWnd;				///< �E�B���h�E�n���h��
		DXGI_FORMAT							useFormat;			///< �g�p����t�H�[�}�b�g
		DirectX11Adapter*					pAdapter;			///< �A�_�v�^�[
		DirectX11EnumDisplay*				pEnumDisplay;		///< �f�B�X�v���C��
		DirectX11RenderTarget*				pBackBuffer;		///< �o�b�N�o�b�t�@
		DirectX11Viewport*					pViewport;			///< �r���[�|�[�g
		SmartComPtr<IDXGISwapChain>			swapChain;			///< �X���b�v�`�F�[��
		SmartComPtr<ID3D11Device>			pd3dDevice;			///< �f�o�C�X�|�C���^
		SmartComPtr<ID3D11DeviceContext>	pd3dDeviceContext;	///< �f�o�C�X�R���e�L�X�g�|�C���^

	private:
		/*!-----------------------------------------------------------
		//	@brief		�f�o�C�X�쐬
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	CreateDevice();

		/*!-----------------------------------------------------------
		//	@brief		�X���b�v�`�F�C���쐬
		//	@param[in]  _pDisplayMode			�f�B�X�v���C���
		//	@param[in]  _isMultiSampleEnable	�}���`�T���v���̗L���L��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	CreateSwapChain(
			CONST DXGI_MODE_DESC* _pDisplayMode,
			const bool _isMultiSampleEnable = false);

		/*!-----------------------------------------------------------
		//	@brief		�����_�[�^�[�Q�b�g�쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	CreateRenderTargetView();

		/*!-----------------------------------------------------------
		//	@brief		�[�x�X�e���V���쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	CreateDepthStencilView();

		/*!-----------------------------------------------------------
		//	@brief		�r���[�|�[�g�쐬
		//  @param[in]  _nWidth		����
		//  @param[in]  _nHeight	����
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	CreateViewport(UINT _nWidth,UINT _nHeight);

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E�̏�Ԃ�ݒ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	SetWindowAssociation();

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E���[�h�ύX�̍œK��
		//	param[in]	_nWidth		�𑜓x�̉���[�ȗ���]
		//	param[in]	_nHeight	�𑜓x�̏c��[�ȗ���]
		//	@return		S_OK�F�œK�������@����ȊO�F�œK�����s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/23
		------------------------------------------------------------*/
		inline	HRESULT	ChangeWindowModeOptimization(CONST UINT _nWidth, CONST UINT _nHeight);

		/*!-----------------------------------------------------------
		//	@brief		�f�t�H���g�̃��X�^���C�U��ݒ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	SetDefaultRasterize();

		/*!-----------------------------------------------------------
		//	@brief		�f�t�H���g�̐[�x�X�e���V���X�e�[�g��ݒ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/16
		------------------------------------------------------------*/
		inline	HRESULT	SetDefaultDepthStencilState();
	};
}

#endif	// _Include_DirectX11Application_hpp_