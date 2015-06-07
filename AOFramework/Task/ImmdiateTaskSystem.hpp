/*************************************************************
*	@file   	ImmdiateTaskSystem.hpp
*	@brief  	ImmdiateTaskSystemクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/04/26
*************************************************************/

#ifndef _Include_ImmdiateTaskSystem_hpp_	// インクルードガード
#define _Include_ImmdiateTaskSystem_hpp_

//------------------------------------------------------------
// インクルード
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
	//	@brief  即時ジョブ処理管理クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/26
	------------------------------------------------------------*/
	class ImmdiateTaskSystem
	{
	private:
		//引数戻り値共にvoid型の関数ポインタ
		typedef std::function<void()> JOB_FUNC;
		static const int32_t DEFAULT_PROC_JOB = 1000;

	public:
		/// コンストラクタ
		ImmdiateTaskSystem();

		/// デストラクタ
		~ImmdiateTaskSystem();

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/26
		------------------------------------------------------------*/
		HRESULT Initialize();

		/*!-----------------------------------------------------------
		//	@brief		システム起動
		//  @param[in]  numJobs	ジョブ数[省略可]
		//	@param[in]	numThreads	マニュアルで作成するスレッド指定数[省略可]
		//  @return     S_OK：作成成功　それ以外　作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/26
		------------------------------------------------------------*/
		HRESULT Startup(const int32_t numJobs = DEFAULT_PROC_JOB, const  int32_t numThreads = -1);

		/*!-----------------------------------------------------------
		//	@brief		待機スレッド破棄
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/26
		------------------------------------------------------------*/
		void WaitKillAllThread();

		/*!-----------------------------------------------------------
		//	@brief		ジョブタスクを登録
		//	@param[in]	_pObject	処理するオブジェクトポインタ
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
		//	@brief		内部ジョブ処理機構
		//	@param[in]	_thIndex	スレッドインデックス
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/26
		------------------------------------------------------------*/
		void ProcessJobTask();
	};
}

#endif // _Include_ImmdiateTaskSystem_hpp_