#include"stdafx.hpp"
#include"SceneExecutor.hpp"
#include"AoScene.hpp"
#include"../System/FpsController.hpp"


using namespace ao;


SceneExecutor::SceneExecutor(SceneControlProxy* _pSceneCtrlProxy) :
SceneCtrlProxyComposite(_pSceneCtrlProxy)
{

}


SceneExecutor::~SceneExecutor()
{

}


//------------------------------------------------------------------------------
/**	@brief		シーン実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/02/28	17:38	T.Aoyama	作成
//------------------------------------------------------------------------------
void SceneExecutor::Execute(ISceneBase* _pExeScene)
{
	if (_pExeScene == nullptr)
	{
		return;
	}

	BOOL isRender = TRUE;
	SceneBase* pExeScene = reinterpret_cast<SceneBase*>(_pExeScene);
	
	//フレームスキップ処理
	if (pExeScene->enableFrameSkip && ao::FpsController::GetInstance()->GetOverTime() > 0)
	{
		isRender = FALSE;
		pExeScene->FrameSkip();
		ao::FpsController::GetInstance()->ResetOverTime();
	}
	
	//初回の実行ならStartCallを呼び出す
	if (!pExeScene->isStart && pExeScene->isLoadEnd)
	{
		pExeScene->StartCall();
	}

	//更新可能なら更新する
	if (pExeScene->enableUpdate)
	{
		pExeScene->Update();
	}

	//レンダリング可能ならレンダリングする
	isRender = (pExeScene->enableRender) ? isRender : FALSE;
	if (isRender)
	{
		pExeScene->Render();
	}
}