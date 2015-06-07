#include"stdafx.hpp"
#include"AoScene.hpp"
#include"SceneLoadThread.hpp"
#include"../Debug/DebugSystem.hpp"


using namespace ao;


SceneLoadThread::SceneLoadThread(SceneControlProxy* _pSceneCtrlProxy) :
hLoadThread(nullptr),
SceneCtrlProxyComposite(_pSceneCtrlProxy)
{

}


SceneLoadThread::~SceneLoadThread()
{

}


//------------------------------------------------------------------------------
/**	@brief		ロード用のスレッド生成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/02/28	00:36	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT SceneLoadThread::BeginLoadThread(ISceneBase* _pISceneBase)
{
	this->hLoadThread = (HANDLE)_beginthreadex(nullptr, 0, SceneLoadThread::LoadThread, _pISceneBase, 0, nullptr);
	if (this->hLoadThread == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[SceneLoadThread::BeginLoadThread]Failed begin load thread.\n"));
		return E_FAIL;
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		スレッド処理の待機 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/01	01:17	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT SceneLoadThread::WaitForLoadThread()
{
	DWORD errorBit = 0;
	if (this->hLoadThread == nullptr)
	{
		return E_FAIL;
	}

	DWORD dwWaitResult = ::WaitForSingleObject(this->hLoadThread, INFINITE);
	switch (dwWaitResult)
	{
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT:
		errorBit |= 1;
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[SceneLoadThread::WaitForLoadThread]\nLoad thread has timed out."));
		break;
	}

	BOOL hCloseResult = ::CloseHandle(this->hLoadThread);
	if (hCloseResult == 0)
	{
		errorBit |= 1;
	}

	this->hLoadThread = nullptr;

	return (errorBit)? E_FAIL : S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		ロード用のプロシージャ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/01	00:28	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	unsigned WINAPI SceneLoadThread::LoadThread(void* _argList)
{
	SceneBase* p = reinterpret_cast<SceneBase*>(_argList);
	p->LoadThread();
	p->isLoadEnd = TRUE;

	return 0;
}