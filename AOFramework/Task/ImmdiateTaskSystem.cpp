#include"stdafx.hpp"
#include"ImmdiateTaskSystem.hpp"
#include"../Debug/DebugSystem.hpp"
#include"../Object/AoObject.hpp"
#include"CountTime.hpp"


using namespace ao;


ImmdiateTaskSystem::ImmdiateTaskSystem() :
numLogicalProcessors(0), numPhysicalProcessors(0), useNumThreads(0), pMainJobQueue(nullptr)
{

}


ImmdiateTaskSystem::~ImmdiateTaskSystem()
{
	this->thGroup.join_all();

	if (this->pMainJobQueue)
	{
		delete this->pMainJobQueue;
	}
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/04/26	11:03	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT ImmdiateTaskSystem::Initialize()
{
	//物理コア＋HTが限界作成スレッド数と過程
	this->numLogicalProcessors = boost::thread::hardware_concurrency();
	this->numPhysicalProcessors = boost::thread::physical_concurrency();
	_ASSERT_EXPR(this->numLogicalProcessors, _T("logic processors is 0."));
	_ASSERT_EXPR(this->numPhysicalProcessors, _T("phyasical processors is 0."));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("--------------------------------------------\n\n"));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("[Immdiate task system] initialize.\n\n"));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Processors[ Logical  ] : %d\n"), this->numLogicalProcessors);
	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Processors[ Physical ] : %d\n"), this->numPhysicalProcessors);

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("--------------------------------------------\n\n"));

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		タスクキラー用のスレッド作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/04/26	11:18	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT ImmdiateTaskSystem::Startup(const int32_t numJobs, const int32_t numThreads)
{
	//作成するスレッド数
	this->useNumThreads = (numThreads == -1) ? 1 : numThreads;

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("--------------------------------------------\n\n"));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("[Task killer system] start up.\n\n"));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Use threads : %d\n\n"), this->useNumThreads);

	//ジョブ処理用のキュー作成
	this->pMainJobQueue = new ao::BlockableQueue<JOB_FUNC>(numJobs);

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Job queue of task line allocated buffer : %d\n\n"), numJobs);

	//スレッド生成
	for (UINT i = 0; i < this->useNumThreads; i++)
	{
		this->thGroup.create_thread(boost::bind(&ImmdiateTaskSystem::ProcessJobTask, this));
		ao::debug::PrintfColor(ConsoleColor::L_CYAN, _T("Startup job kill thread [%d:index]\n"), i);
	}

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("\n--------------------------------------------\n\n"));

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		待機スレッド破棄 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/04/26	14:26	T.Aoyama	作成
//------------------------------------------------------------------------------
void ImmdiateTaskSystem::WaitKillAllThread()
{
	//全スレッドにジョブにnullptrをpushして終了させる
	for (UINT i = 0; i < this->useNumThreads; i++)
	{
		this->pMainJobQueue->Push(nullptr);
	}

	//スレッドが全て終了するまで待機
	this->thGroup.join_all();
}


//------------------------------------------------------------------------------
/**	@brief		内部ジョブ処理機構 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/04/26	14:16	T.Aoyama	作成
//------------------------------------------------------------------------------
void ImmdiateTaskSystem::ProcessJobTask()
{
	JOB_FUNC job;

	for (;;)
	{
		job = this->pMainJobQueue->Pop();

		if (job == nullptr)
		{
			return;
		}

		//登録したジョブ実行
		job();
	}
}