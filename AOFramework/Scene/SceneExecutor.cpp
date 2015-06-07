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
/**	@brief		�V�[�����s */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/02/28	17:38	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneExecutor::Execute(ISceneBase* _pExeScene)
{
	if (_pExeScene == nullptr)
	{
		return;
	}

	BOOL isRender = TRUE;
	SceneBase* pExeScene = reinterpret_cast<SceneBase*>(_pExeScene);
	
	//�t���[���X�L�b�v����
	if (pExeScene->enableFrameSkip && ao::FpsController::GetInstance()->GetOverTime() > 0)
	{
		isRender = FALSE;
		pExeScene->FrameSkip();
		ao::FpsController::GetInstance()->ResetOverTime();
	}
	
	//����̎��s�Ȃ�StartCall���Ăяo��
	if (!pExeScene->isStart && pExeScene->isLoadEnd)
	{
		pExeScene->StartCall();
	}

	//�X�V�\�Ȃ�X�V����
	if (pExeScene->enableUpdate)
	{
		pExeScene->Update();
	}

	//�����_�����O�\�Ȃ烌���_�����O����
	isRender = (pExeScene->enableRender) ? isRender : FALSE;
	if (isRender)
	{
		pExeScene->Render();
	}
}