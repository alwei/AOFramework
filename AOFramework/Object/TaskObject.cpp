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
/**	@brief		�^�X�N�Ǘ��҂�o�^ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	01:46	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void TaskObject::RegisterManager(TaskSystem* _pManager)
{
	this->pTaskManager = _pManager;
}


//------------------------------------------------------------------------------
/**	@brief		�^�X�N�ɖ��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	01:44	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void TaskObject::SetName(const string& _name)
{
	this->name = _name;
}


//------------------------------------------------------------------------------
/**	@brief		���O�擾 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	01:44	T.Aoyama	�쐬
//------------------------------------------------------------------------------
const string& TaskObject::GetName() const
{
	return this->name;
}