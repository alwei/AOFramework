#include"SceneEnforcer.hpp"
#include"../System/FpsController.hpp"
#include"AoScene.hpp"


using namespace ao;


SceneEnforcer::SceneEnforcer(SceneControlProxy* _pSceneCtrlProxy) :
SceneCtrlProxyComposite(_pSceneCtrlProxy)
{

}


SceneEnforcer::~SceneEnforcer()
{

}


//------------------------------------------------------------------------------
/**	@brief		シーン実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/02/28	17:38	T.Aoyama	作成
//------------------------------------------------------------------------------
void SceneEnforcer::Execute(ISceneBase* _pExeScene)
{
	if (_pExeScene == nullptr)
		return;

	SceneBase* pExeScene = dynamic_cast<SceneBase*>(_pExeScene);

	BOOL isRender = TRUE;
	
	//フレームスキップ処理
	if (pExeScene->enableFrameSkip && ao::FpsController::GetInstance()->GetOverTime() > 0)
	{
		isRender = FALSE;
		pExeScene->FrameSkip();
		ao::FpsController::GetInstance()->ResetOverTime();
	}
	
	//初回の実行ならStartCallを呼び出す
	if (!pExeScene->isStart && pExeScene->isLoadEnd){
		pExeScene->StartCall();
	}

	if (pExeScene->enableUpdate)
		pExeScene->Update();

	isRender = (pExeScene->enableRender) ? isRender : FALSE;
	if (isRender)
		pExeScene->Render();
}