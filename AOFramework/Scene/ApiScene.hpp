/*************************************************************
*	@file   	ApiScene.hpp
*	@brief  	ApiScene�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_ApiScene_hpp_	// �C���N���[�h�K�[�h
#define _Include_ApiScene_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"SystemComposite.hpp"
#include"../Api/DirectX11Application.hpp"

namespace ao
{
	class SystemScene;

	/*!-----------------------------------------------------------
	//	@class  ApiScene
	//	@brief  Api��p�̃V�[��
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class ApiScene : public SystemComposite
	{
	public:
		ApiScene(SystemScene* _pSystemScene);
		~ApiScene()override final;

	public:
		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	_hWnd			�E�B���h�E�n���h��
		//	@param[in]	_wResolution	�𑜓x�i���j[�ȗ���]
		//	@param[in]	_hResolution	�𑜓x�i�����j[�ȗ���]
		//	@param[in]  _nWidth			�E�B���h�E��[�ȗ���]
		//	@param[in]  _nHeight		�E�B���h�E����[�ȗ���]
		//	@param[in]	_renderFormat	�����_�����O�t�H�[�}�b�g[�ȗ���]
		//	@param[in]  _isFullScreen	�t���X�N���[���t���O[�ȗ���]
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		HRESULT Initialize(
			const HWND _hWnd,
			const UINT _wResolution = 640,
			const UINT _hResolution = 480,
			const UINT _nWidth = 640,
			const UINT _nHeight = 480,
			const DXGI_FORMAT _renderFormat = DXGI_FORMAT_R16G16B16A16_FLOAT,
			const BOOL _isFullScreen = FALSE);

	public:
		class DirectX11Application			application;
		class DirectX11Resource*			pResource;
		class DirectX11Shader*				pShader;
		class DirectX11GraphicsRenderer*	pRenderer;
		class DirectX11GraphicsState*		pGraphicsState;
		class DirectX11DebugFont*			pDebugFont;
		class DirectX11Profiler*			pProfiler;
	};
}

#endif // _Include_ApiScene_hpp_