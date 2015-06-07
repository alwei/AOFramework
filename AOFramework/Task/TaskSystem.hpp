/*************************************************************
*	@file   	TaskSystem.hpp
*	@brief  	TaskSystemクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/92/18
*************************************************************/

#ifndef _Include_TaskSystem_hpp_	// インクルードガード
#define _Include_TaskSystem_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<list>
#include"../Object/TaskObject.hpp"


namespace ao
{
	class TaskObject;

	/*!-----------------------------------------------------------
	//	@class  TaskSystem
	//	@brief  タスクシステム
	//	@author	Tatsunori Aoyama
	//	@date	2015/92/18
	------------------------------------------------------------*/
	class TaskSystem
	{
	public:
		TaskSystem();
		virtual ~TaskSystem();

		/*!-----------------------------------------------------------
		//	@brief		タスクの追加
		//	@param[in]	_pPushTask	追加するタスクポインタ
		//	@param[in]	_taskName	登録するタスク名
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/22
		------------------------------------------------------------*/
		virtual void PushTask(TaskObject* const _pPushTask);

		/*!-----------------------------------------------------------
		//	@brief		タスクの追加
		//	@param[in]	_pPushTask	追加するタスクポインタ
		//	@param[in]	_taskName	登録するタスク名
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/22
		------------------------------------------------------------*/
		virtual void PushTask(TaskObject* const _pPushTask,const string& _taskName);

		/*!-----------------------------------------------------------
		//	@brief		セットアップ
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void Setup();

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void Initialize();

		/*!-----------------------------------------------------------
		//	@brief		リソースの読み込み
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/23
		------------------------------------------------------------*/
		virtual void LoadResource();

		/*!-----------------------------------------------------------
		//	@brief		起動処理
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void Awake();

		/*!-----------------------------------------------------------
		//	@brief		開始処理
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void Start();

		/*!-----------------------------------------------------------
		//	@brief		事前更新
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void FastUpdate();

		/*!-----------------------------------------------------------
		//	@brief		更新
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void Update();

		/*!-----------------------------------------------------------
		//	@brief		遅延更新
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void LateUpdate();

		/*!-----------------------------------------------------------
		//	@brief		プリレンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/23
		------------------------------------------------------------*/
		virtual void PreRender();

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/23
		------------------------------------------------------------*/
		virtual void Render();

		/*!-----------------------------------------------------------
		//	@brief		ポストレンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/23
		------------------------------------------------------------*/
		virtual void PostRender();

		/*!-----------------------------------------------------------
		//	@brief		タスクの破棄
		//	@param[in]	_pDestoryObj	破棄するタスクオブジェクト
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void Destory(TaskObject* _pDestoryObj);

		/*!-----------------------------------------------------------
		//	@brief		指定のタスク名でタスクオブジェクトを検索
		//	@param[in]	_taskName	検索取得するタスク名
		//	@return		タスクポインタ or nullptr
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/21
		------------------------------------------------------------*/
		virtual TaskObject* SearchGetTaskObject(const string& _taskName);

	protected:
		std::list<TaskObject*> taskList;
	};
}

#endif // _Include_TaskSystem_hpp_