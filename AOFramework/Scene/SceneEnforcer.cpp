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
/**	@brief		�V�[�����s */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/02/28	17:38	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneEnforcer::Execute(ISceneBase* _pExeScene)
{
	if (_pExeScene == nullptr)
		return;

	SceneBase* pExeScene = dynamic_cast<SceneBase*>(_pExeScene);

	BOOL isRender = TRUE;
	
	//�t���[���X�L�b�v����
	if (pExeScene->enableFrameSkip && ao::FpsController::GetInstance()->GetOverTime() > 0)
	{
		isRender = FALSE;
		pExeScene->FrameSkip();
		ao::FpsController::GetInstance()->ResetOverTime();
	}
	
	//����̎��s�Ȃ�StartCall���Ăяo��
	if (!pExeScene->isStart && pExeScene->isLoadEnd){
		pExeScene->StartCall();
	}

	if (pExeScene->enableUpdate)
		pExeScene->Update();

	isRender = (pExeScene->enableRender) ? isRender : FALSE;
	if (isRender)
		pExeScene->Render();
}