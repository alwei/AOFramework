#include"stdafx.hpp"
#include"AoTaskSystem.hpp"


using namespace ao;


AoTaskSystem::AoTaskSystem() : 
useNumThreads(0),numLogicalProcessors(0),numPhysicalProcessors(0),
isKillThread(FALSE),mainThreadHandle(nullptr),ppExecuteBuffer(nullptr)
{

}


AoTaskSystem::~AoTaskSystem()
{
	this->thGroup.join_all();

	if (this->ppExecuteBuffer)
	{
		delete[] this->ppExecuteBuffer;
	}

	::CloseHandle(this->mainThreadHandle);
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/06/03	00:55	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT AoTaskSystem::Initialize()
{
	//物理コア＋HTが限界作成スレッド数と過程
	this->numLogicalProcessors = boost::thread::hardware_concurrency();
	this->numPhysicalProcessors = boost::thread::physical_concurrency();
	_ASSERT_EXPR(this->numLogicalProcessors, _T("logic processors is 0."));
	_ASSERT_EXPR(this->numPhysicalProcessors, _T("phyasical processors is 0."));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("--------------------------------------------\n\n"));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("[ao task system] initialize.\n\n"));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Processors[ Logical  ] : %d\n"), this->numLogicalProcessors);
	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Processors[ Physical ] : %d\n"), this->numPhysicalProcessors);

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("--------------------------------------------\n\n"));

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		スレッド作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/06/03	00:55	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT AoTaskSystem::Startup(const int32_t numJobs, const int32_t numThreads)
{
	//作成するスレッド数（論理コア数）
	this->useNumThreads = (numThreads == -1) ? this->numLogicalProcessors : numThreads;

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("--------------------------------------------\n\n"));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("[Task killer system] start up.\n\n"));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Use threads : %d\n\n"), this->useNumThreads);

	//ジョブ最大数記憶
	this->numMaxJob = numJobs;
	for (auto& it : this->taskLine)
	{
		it.maxPushableJob = numJobs;
	}

	this->ppExecuteBuffer = new AoObject*[numJobs+1];

	//番兵
	this->ppExecuteBuffer[numJobs] = nullptr;
	
	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Job buffer of task line allocated : %d\n\n"), numJobs);


	//ハンドル生成
	this->mainThreadHandle = ::CreateEvent(nullptr, TRUE, FALSE, _T("MainThread"));

	//スレッド同期機構作成
	this->threadSync.numHandle = this->useNumThreads;
	this->threadSync.pHandles = new HANDLE[this->useNumThreads];

	//スレッド生成
	for (UINT i = 0; i < this->useNumThreads; i++)
	{
		this->thGroup.create_thread(boost::bind(&AoTaskSystem::ProcessJobTask,this,i));
		ao::debug::PrintfColor(ConsoleColor::L_CYAN, _T("Startup job kill thread [%d:index]\n"), i);
	}

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("\n--------------------------------------------\n\n"));

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/06/03	01:30	T.Aoyama	作成
//------------------------------------------------------------------------------
void AoTaskSystem::Update()
{
	for (auto& it : this->taskLine)
	{
		//ジョブ登録
		u32 i = 0;
		for(auto& itJob = it.pJobList.cbegin(); i<it.numPushedJob; ++itJob,++i)
		{
			this->ppExecuteBuffer[i] = (*itJob);
		}

		if (i == 0)
		{
			continue;
		}

		//今回のジョブ最大数を記憶
		this->useNumPushedJob = i;

		//最後にnullptrを追加して終了通知
		this->ppExecuteBuffer[i] = nullptr;

		//現在のセレクトインデックスを初期化
		this->nowSelectedJobIndex = 0;

		//マルチスレッド処理開始
		::SetEvent(this->mainThreadHandle);
		::ResetEvent(this->mainThreadHandle);
		
		//ジョブを全て処理するまで待機
		WaitForMultipleObjects(this->threadSync.numHandle, this->threadSync.pHandles, TRUE, INFINITE);
	}
}


//------------------------------------------------------------------------------
/**	@brief		ジョブプッシュ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/06/04	00:14	T.Aoyama	作成
//------------------------------------------------------------------------------
void AoTaskSystem::PushJobTask(const u32 lineId, AoObject* const pJob)
{
	this->interLock.Lock();
	this->taskLine[lineId].pJobList.push_back(pJob);

	if (this->taskLine[lineId].numPushedJob + 1 >= this->taskLine[lineId].maxPushableJob)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[%s]Connection there are too manys.\n")__AO_FUNCTION__);
	}
	else
	{
		++this->taskLine[lineId].numPushedJob;
	}

	this->interLock.Unlock();
}


//------------------------------------------------------------------------------
/**	@brief		待機スレッド破棄 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/06/03	00:55	T.Aoyama	作成
//------------------------------------------------------------------------------
void AoTaskSystem::WaitKillAllThread()
{
	this->isKillThread = TRUE;
	this->useNumPushedJob = 0;
	this->ppExecuteBuffer[0] = nullptr;
	::SetEvent(this->mainThreadHandle);

	//スレッドが全て終了するまで待機
	this->thGroup.join_all();

	::ResetEvent(this->mainThreadHandle);
}



//------------------------------------------------------------------------------
/**	@brief		ジョブタスク取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/06/03	01:20	T.Aoyama	作成
//------------------------------------------------------------------------------
inline AoObject* AoTaskSystem::GetJobTask()
{
	this->interLock.Lock();

	AoObject* p = this->ppExecuteBuffer[this->nowSelectedJobIndex];
	this->nowSelectedJobIndex = min(this->useNumPushedJob,this->nowSelectedJobIndex+1);

	this->interLock.Unlock();

	return p;
}



//------------------------------------------------------------------------------
/**	@brief		内部ジョブ処理機構 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/06/03	00:55	T.Aoyama	作成
//------------------------------------------------------------------------------
inline void AoTaskSystem::ProcessJobTask(const u32 index)
{
	while (!this->isKillThread)
	{
		WaitForSingleObject(this->mainThreadHandle, INFINITE);

		for (;;)
		{
			AoObject* const p = this->GetJobTask();

			//ジョブがない、もしくはジョブの最大数を超えたらbreak
			if (p == nullptr || this->nowSelectedJobIndex >= this->useNumPushedJob)
			{
				break;
			}

			//登録したジョブ実行
			p->Update();
		}

		::SetEvent(this->threadSync.pHandles[index]);
	}
}