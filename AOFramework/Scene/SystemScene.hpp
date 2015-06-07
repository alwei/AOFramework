/*************************************************************
*	@file   	SystemScene.hpp
*	@brief  	SystemScene�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/07/28
*************************************************************/

#ifndef _Include_SystemScene_hpp_	// �C���N���[�h�K�[�h
#define _Include_SystemScene_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include<DirectXMath.h>
#include<dxgi.h>
#include"../Audio/XAudio2.hpp"
#include"../PhysX/PhysXScene.hpp"

namespace ao
{
	class Window;
	class SceneBase;
	class ApiScene;
	class ShaderScene;
	class SceneControlProxy;
	class ImmdiateTaskSystem;
	class AoTaskSystem;

	/*!-----------------------------------------------------------
	//	@class  SystemScene
	//	@brief  Framework�̃V�X�e���V�[��
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/28
	------------------------------------------------------------*/
	class SystemScene
	{
	public:
		~SystemScene();

		/*!-----------------------------------------------------------
		//	@brief		�C���X�^���X�Ăяo��
		//	@return		���g�̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		static	SystemScene*	GetInstance(){
			static SystemScene singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	_pFirstScene	�ŏ��ɐݒ肷��V�[��
		//	@param[in]  _hInst			�C���X�^���X�n���h��
		//	@param[in]	_wResolution	�𑜓x�i���j[�ȗ���]
		//	@param[in]	_hResolution	�𑜓x�i�����j[�ȗ���]
		//	@param[in]  _nWidth			�E�B���h�E��[�ȗ���]
		//	@param[in]  _nHeight		�E�B���h�E����[�ȗ���]
		//	@param[in]	_renderFormat	�����_�����O�t�H�[�}�b�g[�ȗ���]
		//	@param[in]  _isFullScreen	�t���X�N���[���t���O[�ȗ���]
		//  @param[in]  _pOverWindow	�I�[�o�[���C�h����E�B���h�E[�ȗ���]
		//	@return		HRESULT�ɂ�錋��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/28
		------------------------------------------------------------*/
		HRESULT	Initialize(
			SceneBase* _pFirstScene,
			const HINSTANCE _hInst,
			const UINT _wResolution = 640,
			const UINT _hResolution = 480,
			const UINT _nWidth = 640,
			const UINT _nHeight = 480,
			const DXGI_FORMAT _renderFormat = DXGI_FORMAT_R16G16B16A16_FLOAT,
			const BOOL _isFullScreen = FALSE,
			Window* const _pOverWindow = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E������
		//	@param[in]	_overrideWindow	�I�[�o�[���C�h����E�B���h�E
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		void	OverrideWindow(Window* _overrideWindow){
			this->useWindow = _overrideWindow;
		}

		/*!-----------------------------------------------------------
		//	@brief		�I������
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		void	Finalize();

		/*!-----------------------------------------------------------
		//	@brief		���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/28
		------------------------------------------------------------*/
		void	Run();

	public:
		BOOL isKilled;
		BOOL isMsgUpdate;
		HANDLE hMsgThread;
		Window*	useWindow;
		ApiScene* pApiScene;
		ShaderScene* pShaderScene;
		SceneControlProxy* pSceneCtrlProxy;
		ImmdiateTaskSystem* pImmdiateTaskSystem;
		AoTaskSystem* pAoTaskSystem;
	private:
		/*!-----------------------------------------------------------
		//	@brief		���b�Z�[�W�����p�̃X���b�h
		//	@param[in]  _argList	�C�Ӄ|�C���^
		//	@return		0�F����I��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		static	unsigned WINAPI	TransactMessage(void* _argList);

		/*!-----------------------------------------------------------
		//	@brief		�Q�[������
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		inline	void	GameProcess();

	private:
		SystemScene();
		SystemScene(const SystemScene&) = delete;
		void operator=(const SystemScene&) = delete;
	};
}

#endif // _Include_SystemScene_hpp_