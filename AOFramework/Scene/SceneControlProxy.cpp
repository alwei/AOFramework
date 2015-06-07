#include"stdafx.hpp"
#include"SceneControlProxy.hpp"
#include"AoScene.hpp"
#include"SceneExecutor.hpp"
#include"SceneChanger.hpp"
#include"SceneDeleter.hpp"
#include"SceneLoadThread.hpp"
#include"SceneDatabase.hpp"
#include"../System/FpsController.hpp"
#include"../Debug/DebugSystem.hpp"


//エラー検出マクロ
#ifdef __AO_DEBUG__
#define SECENE_CONTROL_PROXY_CHECK_ERROR(hr)	\
	if( FAILED(hr) )							\
	{											\
		return;									\
	}				
#else
#define SECENE_CONTROL_PROXY_CHECK_ERROR(_hr) _hr
#endif


using namespace ao;


SceneControlProxy::SceneControlProxy(SystemScene* _pSystemScene) :
pSceneExecutor(new SceneExecutor(this)),
pSceneChanger(new SceneChanger(this)),
pSceneDeleter(new SceneDeleter(this)),
pSceneLoadThread(new SceneLoadThread(this)),
pSceneDatabase(new SceneDatabase(this)),
SystemComposite(_pSystemScene)
{
	this->compositeList.push_back(this->pSceneDatabase);
	this->compositeList.push_back(this->pSceneLoadThread);
	this->compositeList.push_back(this->pSceneDeleter);
	this->compositeList.push_back(this->pSceneChanger);
	this->compositeList.push_back(this->pSceneExecutor);
}


SceneControlProxy::~SceneControlProxy()
{
	if (this->compositeList.empty())
	{
		return;
	}

	for (auto& it : this->compositeList)
	{
		delete it;
	}
}


//------------------------------------------------------------------------------
/**	@brief		最初のシーン設定 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/01	00:52	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT SceneControlProxy::SetFirstScene(ISceneBase* _pFirstScene)
{
	this->pSceneDatabase->pNowScene = _pFirstScene;
	return this->InitLoad();
}


//------------------------------------------------------------------------------
/**	@brief		実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/０７/31	22:57	T.Aoyama	作成
//------------------------------------------------------------------------------
void SceneControlProxy::Execute()
{
	this->pSceneExecutor->Execute(this->pSceneDatabase->pNowScene);
	
	if (this->pSceneDatabase->pDeleteScene)
	{
		ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("delete scene %s.\n"),this->pSceneDatabase->pDeleteScene->GetSceneName());
		SECENE_CONTROL_PROXY_CHECK_ERROR(this->ExchangeScene());
		SECENE_CONTROL_PROXY_CHECK_ERROR(this->InitLoad());
	}
}


//------------------------------------------------------------------------------
/**	@brief		終了処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/02	09:37	T.Aoyama	作成
//------------------------------------------------------------------------------
void SceneControlProxy::Finalize()
{
	this->pSceneLoadThread->WaitForLoadThread();

	for (auto& it : this->compositeList)
	{
		delete it;
	}

	this->compositeList.clear();
}


//------------------------------------------------------------------------------
/**	@brief		新しいシーンの読み込み */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/01	01:59	T.Aoyama	作成
//------------------------------------------------------------------------------
void SceneControlProxy::LoadLevel(SceneBase* _pNewScene)
{
	this->pSceneChanger->SwapScene(_pNewScene,this->pSceneDatabase);
}


//------------------------------------------------------------------------------
/**	@brief		ロードの初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/01	00:49	T.Aoyama	作成
//------------------------------------------------------------------------------
FORCEINLINE HRESULT SceneControlProxy::InitLoad()
{
	if (this->pSceneDatabase->pNowScene == nullptr)
	{
		return S_FALSE;
	}

	//ロード処理開始
	this->pSceneDatabase->pNowScene->InitPreLoad();

	//シーンポインタの例外対応処理
	const auto& database = this->pSceneDatabase;
	const auto& pLoadResourceScene = (database->pNowScene) ? database->pNowScene : database->pSwapScene;

	HRESULT hr = this->pSceneLoadThread->BeginLoadThread(pLoadResourceScene);
#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[SceneControlProxy::InitLoad]Failed begin init load.\n"));
		delete this->pSceneDatabase->pNowScene;
		this->pSceneDatabase->pNowScene = nullptr;
	}
#endif

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		シーンの削除 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/01	00:24	T.Aoyama	作成
//------------------------------------------------------------------------------
FORCEINLINE HRESULT SceneControlProxy::ExchangeScene()
{
	this->pSceneDeleter->DeleteScene(&this->pSceneDatabase->pDeleteScene);
	this->pSceneChanger->ChangeScene(this->pSceneDatabase);

	return S_OK;
}