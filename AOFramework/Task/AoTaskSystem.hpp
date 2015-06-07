/*************************************************************
*	@file   	AoTaskSystem.hpp
*	@brief  	AoTaskSystemクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/06/02
*************************************************************/

#ifndef _Include_AoTaskSystem_hpp_	// インクルードガード
#define _Include_AoTaskSystem_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<boost\thread.hpp>
#include"Object\AoObject.hpp"
#include"../Utility/InterLock.hpp"
#include"../Define/TaskDefine.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  AoTaskSystem
	//	@brief  AoObject専用のタスクシステム
	//	@author	Tatsunori Aoyama
	//	@date	2014/06/02
	------------------------------------------------------------*/
	class AoTaskSystem
	{
		static const u32 DEFAULT_PROC_NUM_JOB = 3000;
	public:
		/// コンストラクタ
		AoTaskSystem();

		/// デストラクタ
		~AoTaskSystem();

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		HRESULT Initialize();

		/*!-----------------------------------------------------------
		//	@brief		システム起動
		//  @param[in]  numJobs	ジョブ数[省略可]
		//	@param[in]	numThreads	マニュアルで作成するスレッド指定数[省略可]
		//	@return		S_OK：機動成功　それ以外：機動失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		HRESULT Startup(const int32_t numJobs = DEFAULT_PROC_NUM_JOB, const int32_t numThreads = -1);

		/*!-----------------------------------------------------------
		//	@brief		更新
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		void Update();

		/*!-----------------------------------------------------------
		//	@brief		ジョブプッシュ
		//	@param[in]	lineId	ラインＩＤ
		//	@param[in]	pJob	ジョブポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		void PushJobTask(const u32 lineId, AoObject* const pJob);

		/*!-----------------------------------------------------------
		//	@brief		全てのスレッド破棄
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
		//	@brief		ジョブタスク取得
		//	@return		ジョブポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2015/06/03
		------------------------------------------------------------*/
		inline AoObject* GetJobTask();

		/*!-----------------------------------------------------------
		//	@brief		内部ジョブ処理機構
		//	@param[in]	index	スレッドインデックス
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/26
		------------------------------------------------------------*/
		inline void ProcessJobTask(const u32 index);
	};
}

#endif // _Include_AoTaskSystem_hpp_