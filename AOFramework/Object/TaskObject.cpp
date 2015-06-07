#include"stdafx.hpp"
#include"TaskObject.hpp"


using namespace ao;
AO_CLASS_IMPLEMENT(TaskObject);


TaskObject::TaskObject() : AoObject(_T("TaskObject"))
{

}


TaskObject::TaskObject(AOLPSTR _name) : AoObject(_name)
{

}


TaskObject::~TaskObject()
{

}


//------------------------------------------------------------------------------
/**	@brief		タスク管理者を登録 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	01:46	T.Aoyama	作成
//------------------------------------------------------------------------------
void TaskObject::RegisterManager(TaskSystem* _pManager)
{
	this->pTaskManager = _pManager;
}


//------------------------------------------------------------------------------
/**	@brief		タスクに命名 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	01:44	T.Aoyama	作成
//------------------------------------------------------------------------------
void TaskObject::SetName(const string& _name)
{
	this->name = _name;
}


//------------------------------------------------------------------------------
/**	@brief		名前取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	01:44	T.Aoyama	作成
//------------------------------------------------------------------------------
const string& TaskObject::GetName() const
{
	return this->name;
}