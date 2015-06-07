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
/**	@brief		������ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/06/03	00:55	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT AoTaskSystem::Initialize()
{
	//�����R�A�{HT�����E�쐬�X���b�h���Ɖߒ�
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
/**	@brief		�X���b�h�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/06/03	00:55	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT AoTaskSystem::Startup(const int32_t numJobs, const int32_t numThreads)
{
	//�쐬����X���b�h���i�_���R�A���j
	this->useNumThreads = (numThreads == -1) ? this->numLogicalProcessors : numThreads;

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("--------------------------------------------\n\n"));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("[Task killer system] start up.\n\n"));

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Use threads : %d\n\n"), this->useNumThreads);

	//�W���u�ő吔�L��
	this->numMaxJob = numJobs;
	for (auto& it : this->taskLine)
	{
		it.maxPushableJob = numJobs;
	}

	this->ppExecuteBuffer = new AoObject*[numJobs+1];

	//�ԕ�
	this->ppExecuteBuffer[numJobs] = nullptr;
	
	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Job buffer of task line allocated : %d\n\n"), numJobs);


	//�n���h������
	this->mainThreadHandle = ::CreateEvent(nullptr, TRUE, FALSE, _T("MainThread"));

	//�X���b�h�����@�\�쐬
	this->threadSync.numHandle = this->useNumThreads;
	this->threadSync.pHandles = new HANDLE[this->useNumThreads];

	//�X���b�h����
	for (UINT i = 0; i < this->useNumThreads; i++)
	{
		this->thGroup.create_thread(boost::bind(&AoTaskSystem::ProcessJobTask,this,i));
		ao::debug::PrintfColor(ConsoleColor::L_CYAN, _T("Startup job kill thread [%d:index]\n"), i);
	}

	ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("\n--------------------------------------------\n\n"));

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/06/03	01:30	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void AoTaskSystem::Update()
{
	for (auto& it : this->taskLine)
	{
		//�W���u�o�^
		u32 i = 0;
		for(auto& itJob = it.pJobList.cbegin(); i<it.numPushedJob; ++itJob,++i)
		{
			this->ppExecuteBuffer[i] = (*itJob);
		}

		if (i == 0)
		{
			continue;
		}

		//����̃W���u�ő吔���L��
		this->useNumPushedJob = i;

		//�Ō��nullptr��ǉ����ďI���ʒm
		this->ppExecuteBuffer[i] = nullptr;

		//���݂̃Z���N�g�C���f�b�N�X��������
		this->nowSelectedJobIndex = 0;

		//�}���`�X���b�h�����J�n
		::SetEvent(this->mainThreadHandle);
		::ResetEvent(this->mainThreadHandle);
		
		//�W���u��S�ď�������܂őҋ@
		WaitForMultipleObjects(this->threadSync.numHandle, this->threadSync.pHandles, TRUE, INFINITE);
	}
}


//------------------------------------------------------------------------------
/**	@brief		�W���u�v�b�V�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/06/04	00:14	T.Aoyama	�쐬
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
/**	@brief		�ҋ@�X���b�h�j�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/06/03	00:55	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void AoTaskSystem::WaitKillAllThread()
{
	this->isKillThread = TRUE;
	this->useNumPushedJob = 0;
	this->ppExecuteBuffer[0] = nullptr;
	::SetEvent(this->mainThreadHandle);

	//�X���b�h���S�ďI������܂őҋ@
	this->thGroup.join_all();

	::ResetEvent(this->mainThreadHandle);
}



//------------------------------------------------------------------------------
/**	@brief		�W���u�^�X�N�擾 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/06/03	01:20	T.Aoyama	�쐬
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
/**	@brief		�����W���u�����@�\ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/06/03	00:55	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline void AoTaskSystem::ProcessJobTask(const u32 index)
{
	while (!this->isKillThread)
	{
		WaitForSingleObject(this->mainThreadHandle, INFINITE);

		for (;;)
		{
			AoObject* const p = this->GetJobTask();

			//�W���u���Ȃ��A�������̓W���u�̍ő吔�𒴂�����break
			if (p == nullptr || this->nowSelectedJobIndex >= this->useNumPushedJob)
			{
				break;
			}

			//�o�^�����W���u���s
			p->Update();
		}

		::SetEvent(this->threadSync.pHandles[index]);
	}
}