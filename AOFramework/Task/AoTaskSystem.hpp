/*************************************************************
*	@file   	AoTaskSystem.hpp
*	@brief  	AoTaskSystem�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/06/02
*************************************************************/

#ifndef _Include_AoTaskSystem_hpp_	// �C���N���[�h�K�[�h
#define _Include_AoTaskSystem_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<boost\thread.hpp>
#include"Object\AoObject.hpp"
#include"../Utility/InterLock.hpp"
#include"../Define/TaskDefine.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  AoTaskSystem
	//	@brief  AoObject��p�̃^�X�N�V�X�e��
	//	@author	Tatsunori Aoyama
	//	@date	2014/06/02
	------------------------------------------------------------*/
	class AoTaskSystem
	{
		static const u32 DEFAULT_PROC_NUM_JOB = 3000;
	public:
		/// �R���X�g���N�^
		AoTaskSystem();

		/// �f�X�g���N�^
		~AoTaskSystem();

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		HRESULT Initialize();

		/*!-----------------------------------------------------------
		//	@brief		�V�X�e���N��
		//  @param[in]  numJobs	�W���u��[�ȗ���]
		//	@param[in]	numThreads	�}�j���A���ō쐬����X���b�h�w�萔[�ȗ���]
		//	@return		S_OK�F�@�������@����ȊO�F�@�����s
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		HRESULT Startup(const int32_t numJobs = DEFAULT_PROC_NUM_JOB, const int32_t numThreads = -1);

		/*!-----------------------------------------------------------
		//	@brief		�X�V
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		void Update();

		/*!-----------------------------------------------------------
		//	@brief		�W���u�v�b�V��
		//	@param[in]	lineId	���C���h�c
		//	@param[in]	pJob	�W���u�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		void PushJobTask(const u32 lineId, AoObject* const pJob);

		/*!-----------------------------------------------------------
		//	@brief		�S�ẴX���b�h�j��
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		void WaitKillAllThread();

	private:
		struct TaskLine
		{
			TaskLine() : maxPushableJob(0),numPushedJob(0){};
			~TaskLine(){}
			u32 maxPushableJob;
			u32 numPushedJob;
			std::list<AoObject*> pJobList;
		};

		struct ThreadSync
		{
			ThreadSync() :numHandle(0), pHandles(nullptr){};
			~ThreadSync()
			{
				if (this->pHandles)
				{
					delete[] this->pHandles;
				}
			};
			u32 numHandle;
			HANDLE* pHandles;
		};

	private:
		u32 useNumThreads;
		u32 numLogicalProcessors;
		u32 numPhysicalProcessors;
		u32 nowSelectedJobIndex;
		u32 numMaxJob;
		u32 useNumPushedJob;
		u32 isKillThread;
		HANDLE mainThreadHandle;
		AoObject** ppExecuteBuffer;
		ThreadSync threadSync;
		std::array<TaskLine,static_cast<u32>(TASK_LINE::MAX)> taskLine;
		boost::thread_group thGroup;
		InterLock interLock;

	private:
		/*!-----------------------------------------------------------
		//	@brief		�W���u�^�X�N�擾
		//	@return		�W���u�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		inline AoObject* GetJobTask();

		/*!-----------------------------------------------------------
		//	@brief		�����W���u�����@�\
		//	@param[in]	index	�X���b�h�C���f�b�N�X
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/26
		------------------------------------------------------------*/
		inline void ProcessJobTask(const u32 index);
	};
}

#endif // _Include_AoTaskSystem_hpp_