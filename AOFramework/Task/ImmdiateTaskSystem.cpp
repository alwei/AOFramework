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
/**	@brief		������ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/04/26	11:03	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT ImmdiateTaskSystem::Initialize()
{
	//�����R�A�{HT�����E�쐬�X���b�h���Ɖߒ�
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
/**	@brief		�^�X�N�L���[�p�̃X���b�h�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/04/26	11:18	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT ImmdiateTaskSystem::Startup(const int32_t numJobs, const int32_t numThreads)
{
	//�쐬����X���b�h��
	this->useNumThreads = (numThreads == -1) ? 1 : numThreads;

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("--------------------------------------------\n\n"));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("[Task killer system] start up.\n\n"));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Use threads : %d\n\n"), this->useNumThreads);

	//�W���u�����p�̃L���[�쐬
	this->pMainJobQueue = new ao::BlockableQueue<JOB_FUNC>(numJobs);

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Job queue of task line allocated buffer : %d\n\n"), numJobs);

	//�X���b�h����
	for (UINT i = 0; i < this->useNumThreads; i++)
	{
		this->thGroup.create_thread(boost::bind(&ImmdiateTaskSystem::ProcessJobTask, this));
		ao::debug::PrintfColor(ConsoleColor::L_CYAN, _T("Startup job kill thread [%d:index]\n"), i);
	}

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("\n--------------------------------------------\n\n"));

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�ҋ@�X���b�h�j�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/04/26	14:26	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void ImmdiateTaskSystem::WaitKillAllThread()
{
	//�S�X���b�h�ɃW���u��nullptr��push���ďI��������
	for (UINT i = 0; i < this->useNumThreads; i++)
	{
		this->pMainJobQueue->Push(nullptr);
	}

	//�X���b�h���S�ďI������܂őҋ@
	this->thGroup.join_all();
}


//------------------------------------------------------------------------------
/**	@brief		�����W���u�����@�\ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/04/26	14:16	T.Aoyama	�쐬
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

		//�o�^�����W���u���s
		job();
	}
}