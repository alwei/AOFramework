/*************************************************************
*	@file   	ImmdiateTaskSystem.hpp
*	@brief  	ImmdiateTaskSystem�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/04/26
*************************************************************/

#ifndef _Include_ImmdiateTaskSystem_hpp_	// �C���N���[�h�K�[�h
#define _Include_ImmdiateTaskSystem_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include<boost/thread.hpp>
#include"../Define/SystemDefine.hpp"
#include"../Utility/LockFreeQueue.hpp"
#include"../Utility/BlockableQueue.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  ImmdiateTaskSystem
	//	@brief  �����W���u�����Ǘ��N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/26
	------------------------------------------------------------*/
	class ImmdiateTaskSystem
	{
	private:
		//�����߂�l����void�^�̊֐��|�C���^
		typedef std::function<void()> JOB_FUNC;
		static const int32_t DEFAULT_PROC_JOB = 1000;

	public:
		/// �R���X�g���N�^
		ImmdiateTaskSystem();

		/// �f�X�g���N�^
		~ImmdiateTaskSystem();

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/26
		------------------------------------------------------------*/
		HRESULT Initialize();

		/*!-----------------------------------------------------------
		//	@brief		�V�X�e���N��
		//  @param[in]  numJobs	�W���u��[�ȗ���]
		//	@param[in]	numThreads	�}�j���A���ō쐬����X���b�h�w�萔[�ȗ���]
		//  @return     S_OK�F�쐬�����@����ȊO�@�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/26
		------------------------------------------------------------*/
		HRESULT Startup(const int32_t numJobs = DEFAULT_PROC_JOB, const  int32_t numThreads = -1);

		/*!-----------------------------------------------------------
		//	@brief		�ҋ@�X���b�h�j��
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/26
		------------------------------------------------------------*/
		void WaitKillAllThread();

		/*!-----------------------------------------------------------
		//	@brief		�W���u�^�X�N��o�^
		//	@param[in]	_pObject	��������I�u�W�F�N�g�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/26
		------------------------------------------------------------*/
		template<class T>
		void PushJobTask(T jobFunc)
		{
			this->pMainJobQueue->Push(jobFunc);
		}

	private:
		typedef ao::BlockableQueue<JOB_FUNC>* JOB_QUEUE;

		u32 useNumThreads;
		u32 numLogicalProcessors;
		u32 numPhysicalProcessors;
		boost::thread_group thGroup;
		JOB_QUEUE pMainJobQueue;

	private:
		/*!-----------------------------------------------------------
		//	@brief		�����W���u�����@�\
		//	@param[in]	_thIndex	�X���b�h�C���f�b�N�X
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/26
		------------------------------------------------------------*/
		void ProcessJobTask();
	};
}

#endif // _Include_ImmdiateTaskSystem_hpp_