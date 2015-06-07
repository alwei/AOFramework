#include"stdafx.hpp"
#include"SystemScene.hpp"
#include"SceneControlProxy.hpp"
#include"EngineLogo\EngineLogoScene.hpp"
#include"../System/FpsController.hpp"
#include"../Input/Keyboard.hpp"
#include"../Input/Mouse.hpp"
#include"../Input/XInput.hpp"
#include"../PhysX/PhysX.hpp"
#include"../Debug/DebugSystem.hpp"
#include"../Window/Window.hpp"
#include"../Scene/ApiScene.hpp"
#include"../Scene/ShaderScene.hpp"
#include"../Task/ImmdiateTaskSystem.hpp"
#include"../Task/AoTaskSystem.hpp"
#include"../../source/Scene/Title/TitleScene.hpp"


using namespace ao;

namespace
{
	//エラー検出マクロ
#ifdef __AO_DEBUG__
#define SYSTEM_SCENE_CHECK_ERROR(hr)	\
		if(FAILED(hr))					\
		{								\
			delete _pFirstScene;		\
			this->isKilled = TRUE;		\
			return hr;					\
		}
#else
#define SYSTEM_SCENE_CHECK_ERROR(hr)
#endif
}


SystemScene::SystemScene() :
isKilled(FALSE),
isMsgUpdate(FALSE),
hMsgThread(nullptr),
pSceneCtrlProxy(new SceneControlProxy(this)),
pImmdiateTaskSystem(new ImmdiateTaskSystem()),
pAoTaskSystem(new AoTaskSystem)
{
	// SSE/SSE2/ARM-NEON CPU命令の対応チェック
	if (DirectX::XMVerifyCPUSupport())
	{
		ao::debug::PrintfColor(ConsoleColor::H_GREEN, _T("this cpu has DirectXMath support.\n\n"));
	}
	else
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("this cpu does not support DirecctXMath\n\n"));
	}

	//デフォルトのウィンドウ登録
	this->useWindow = new Window();
}


SystemScene::~SystemScene()
{
	this->isKilled = TRUE;
	if (this->hMsgThread != nullptr)
	{
		::WaitForSingleObject(this->hMsgThread, INFINITE);
		::CloseHandle(this->hMsgThread);
	}

	delete this->pAoTaskSystem;
	delete this->pImmdiateTaskSystem;
	delete this->pSceneCtrlProxy;
	delete this->pShaderScene;
	delete this->pApiScene;
	delete this->useWindow;
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/28	10:29	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT SystemScene::Initialize(
	SceneBase* _pFirstScene,
	const HINSTANCE _hInst,
	const UINT _wResolution,
	const UINT _hResolution,
	const UINT _nWidth,
	const UINT _nHeight,
	const DXGI_FORMAT _renderFormat,
	const BOOL _isFullScreen,
	Window* const _pOverWindow)
{
	HRESULT hr = E_FAIL;

	ao::FpsController::GetInstance()->Create();

#if 0
	//メッセージ処理用の専用スレッド作成
	this->hMsgThread = (HANDLE)::beginthreadex(
		nullptr,
		0,
		SystemScene::TransactMessage,
		this,
		0,
		nullptr
		);
#endif
	if (_pOverWindow)
	{
		delete this->useWindow;
		this->useWindow = _pOverWindow;
	}

	//ウィンドウの初期化
	hr = this->useWindow->Initialize(_hInst, _nWidth, _nHeight, _isFullScreen);
	SYSTEM_SCENE_CHECK_ERROR(hr);

	//PhysX初期化
	hr = PhysX::GetInstance()->Initialize();
	SYSTEM_SCENE_CHECK_ERROR(hr);

	//XAudio初期化
	hr = sXAudio2Manager()->Init();
	SYSTEM_SCENE_CHECK_ERROR(hr);

	
	//即時実行型のスレッドプール初期化
	this->pImmdiateTaskSystem->Initialize();
	this->pImmdiateTaskSystem->Startup();

	//AoObject専用のスレッドプール初期化
	this->pAoTaskSystem->Initialize();
	this->pAoTaskSystem->Startup(100000);

	//ApiSceneの初期化
	this->pApiScene = new ApiScene(this);
	hr = this->pApiScene->Initialize(
		this->useWindow->GetHWnd(),
		_wResolution, _hResolution,
		_nWidth, _nHeight,
		_renderFormat,
		_isFullScreen);
	SYSTEM_SCENE_CHECK_ERROR(hr);

	//ShaderSceneの初期化
	this->pShaderScene = new ShaderScene(this);
	hr = this->pShaderScene->Initialize(
		this->pApiScene->application.GetDevice(),
		this->pApiScene->application.GetDeviceContext(),
		this->pApiScene->pGraphicsState,
		this->pApiScene->pRenderer);
	SYSTEM_SCENE_CHECK_ERROR(hr);

	//シーン設定
	EngineLogoScene* pEngineLogo = new EngineLogoScene();
	pEngineLogo->Setup(_pFirstScene);

	this->pSceneCtrlProxy->SetFirstScene(pEngineLogo);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		終了処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	09:39	T.Aoyama	作成
//------------------------------------------------------------------------------
void SystemScene::Finalize()
{
	this->pAoTaskSystem->WaitKillAllThread();
	this->pImmdiateTaskSystem->WaitKillAllThread();
	this->pSceneCtrlProxy->Finalize();
	sXAudio2Manager()->UnInit();
}


//------------------------------------------------------------------------------
/**	@brief		実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/28	10:29	T.Aoyama	作成
//------------------------------------------------------------------------------
void SystemScene::Run()
{
	while (!this->isKilled)
	{
		this->GameProcess();
	}
}


//------------------------------------------------------------------------------
/**	@brief		メッセージ処理用のスレッド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	02:40	T.Aoyama	作成
//------------------------------------------------------------------------------
unsigned WINAPI SystemScene::TransactMessage(void* _argList)
{
	SystemScene* scene = static_cast<SystemScene*>(_argList);

	while (!scene->isKilled)
	{
		if (!scene->isMsgUpdate)
		{
			continue;
		}

		// TODO: ウィンドウのメッセージマルチスレッド処理は凍結


		if (scene->useWindow->IsDestroyed())
		{
			scene->isKilled = TRUE;
		}
	}

	return 0;
}


//------------------------------------------------------------------------------
/**	@brief		ゲーム処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	09:48	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	void SystemScene::GameProcess()
{
	this->useWindow->ProcessMessage();

	//更新可能ならゲームフレーム更新
	if (ao::FpsController::GetInstance()->IsUpdatable())
	{
		ao::FpsController::GetInstance()->Begin();

		ao::Keyboard::GetInstance()->Update();
		ao::Mouse::GetInstance()->Update();
		ao::XInput::GetInstance()->Update();

		this->pSceneCtrlProxy->Execute();

		if (this->useWindow->IsDestroyed())
		{
			this->isKilled = TRUE;
			return;
		}

		ao::FpsController::GetInstance()->End();
	}
}