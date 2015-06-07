#include"stdafx.hpp"
#include"FpsController.hpp"
#include"CountTime.hpp"


using namespace ao;


FpsController::FpsController() :
hThread(nullptr),
threadId(0),
frameRate(60),
ms1Frame(0.0166666),
overTime(0),
aveFps(0),
isKilled(false),
bUpdate(false)
{

}


FpsController::~FpsController()
{
	this->Destroy();
}


//------------------------------------------------------------------------------
/**	@brief		fps用のサブスレッド作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	02:55	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT FpsController::Create(DWORD _frameRate)
{
	this->frameRate = _frameRate;
	this->ms1Frame = (1.0 / _frameRate);

	//以前のスレッドを破棄
	this->Destroy();

	//フラグ初期化
	this->isKilled = false;
	this->bUpdate = false;

	//カウント値初期化
	this->aveFps = 0;
	this->frameCount = 0;

	//スレッド作成
	this->hThread = (HANDLE)::_beginthreadex(
		nullptr,
		0,
		FpsController::CallProc,
		this,
		0,
		&this->threadId);

	//エラーチェック
	if (this->hThread == nullptr)
	{
		return E_FAIL;
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		サブスレッドの破棄 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	02:57	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT FpsController::Destroy()
{
	BOOL isSafeEnd = 0;

	//既に破棄されていればスキップ
	if (this->hThread == nullptr)
	{
		return S_OK;
	}

	this->isKilled = true;
	::WaitForSingleObject(this->hThread, INFINITE);
	isSafeEnd = ::CloseHandle(this->hThread);
	this->hThread = nullptr;

	if (isSafeEnd == 0)
	{
		return E_FAIL;
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		チェックスタート */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	03:07	T.Aoyama	作成
//------------------------------------------------------------------------------
void FpsController::Begin()
{
	//クロック測定開始
	this->_1FTimer.Begin();
	if (this->frameCount == 0)
	{
		this->loopTimer.Begin();
	}
}


//------------------------------------------------------------------------------
/**	@brief		計測終了 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	03:09	T.Aoyama	作成
//------------------------------------------------------------------------------
void FpsController::End()
{
	this->_1FTimer.End();

	//デルタタイム取得
	this->deltaTime = max(this->ms1Frame, this->_1FTimer.GetMilliSeconds<double>() / 1000.0f);

	//フレームレート分のオーバーを算出
	this->overTime = this->deltaTime + this->overTime;
	this->overTime = max(0, this->overTime - this->ms1Frame);


	//デルタ修正値取得
	this->deltaFix = 1.0 + this->overTime / this->deltaTime;

	//FPS計測
	this->CountFps();

	//更新不能にする
	this->bUpdate = false;
}


//------------------------------------------------------------------------------
/**	@brief		fpsカウント */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	03:00	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	void FpsController::CountFps()
{
	//分割フレームをフレームレートをミリ秒分割用に倍処理する
	static double divFrame = static_cast<double>(this->frameRate)*1000;

	if (this->frameCount >= this->frameRate)
	{
		this->loopTimer.End();
		this->aveFps = static_cast<double>(divFrame) / this->loopTimer.GetMilliSeconds<double>();
		this->loopTimer.Begin();
		this->frameCount = 0;
	}

	++this->frameCount;
}


//------------------------------------------------------------------------------
/**	@brief		fps計測 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	03:14	T.Aoyama	作成
//------------------------------------------------------------------------------
unsigned FpsController::Process()
{
	static DWORD timeTable[] = { 17, 17, 16 };
	unsigned int mod = _countof(timeTable);
	unsigned int nowTimeTablePos = 0;

	//終了するまでループ
	while (!this->isKilled)
	{
		::Sleep(timeTable[nowTimeTablePos]);
		nowTimeTablePos = (nowTimeTablePos + 1) % mod;
		this->bUpdate = true;
	}

	return false;
}


//------------------------------------------------------------------------------
/**	@brief		サブスレッドに処理するプロシージャ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	03:15	T.Aoyama	作成
//------------------------------------------------------------------------------
unsigned WINAPI FpsController::CallProc(void* _argList)
{
	FpsController* p = static_cast<FpsController*>(_argList);
	return p->Process();
}