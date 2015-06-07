/*************************************************************
*	@file   	TaskSystem.hpp
*	@brief  	TaskSystem�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/92/18
*************************************************************/

#ifndef _Include_TaskSystem_hpp_	// �C���N���[�h�K�[�h
#define _Include_TaskSystem_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<list>
#include"../Object/TaskObject.hpp"


namespace ao
{
	class TaskObject;

	/*!-----------------------------------------------------------
	//	@class  TaskSystem
	//	@brief  �^�X�N�V�X�e��
	//	@author	Tatsunori Aoyama
	//	@date	2015/92/18
	------------------------------------------------------------*/
	class TaskSystem
	{
	public:
		TaskSystem();
		virtual ~TaskSystem();

		/*!-----------------------------------------------------------
		//	@brief		�^�X�N�̒ǉ�
		//	@param[in]	_pPushTask	�ǉ�����^�X�N�|�C���^
		//	@param[in]	_taskName	�o�^����^�X�N��
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/22
		------------------------------------------------------------*/
		virtual void PushTask(TaskObject* const _pPushTask);

		/*!-----------------------------------------------------------
		//	@brief		�^�X�N�̒ǉ�
		//	@param[in]	_pPushTask	�ǉ�����^�X�N�|�C���^
		//	@param[in]	_taskName	�o�^����^�X�N��
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/22
		------------------------------------------------------------*/
		virtual void PushTask(TaskObject* const _pPushTask,const string& _taskName);

		/*!-----------------------------------------------------------
		//	@brief		�Z�b�g�A�b�v
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void Setup();

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void Initialize();

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�̓ǂݍ���
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/23
		------------------------------------------------------------*/
		virtual void LoadResource();

		/*!-----------------------------------------------------------
		//	@brief		�N������
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void Awake();

		/*!-----------------------------------------------------------
		//	@brief		�J�n����
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void Start();

		/*!-----------------------------------------------------------
		//	@brief		���O�X�V
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void FastUpdate();

		/*!-----------------------------------------------------------
		//	@brief		�X�V
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void Update();

		/*!-----------------------------------------------------------
		//	@brief		�x���X�V
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void LateUpdate();

		/*!-----------------------------------------------------------
		//	@brief		�v�������_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/23
		------------------------------------------------------------*/
		virtual void PreRender();

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/23
		------------------------------------------------------------*/
		virtual void Render();

		/*!-----------------------------------------------------------
		//	@brief		�|�X�g�����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/23
		------------------------------------------------------------*/
		virtual void PostRender();

		/*!-----------------------------------------------------------
		//	@brief		�^�X�N�̔j��
		//	@param[in]	_pDestoryObj	�j������^�X�N�I�u�W�F�N�g
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/19
		------------------------------------------------------------*/
		virtual void Destory(TaskObject* _pDestoryObj);

		/*!-----------------------------------------------------------
		//	@brief		�w��̃^�X�N���Ń^�X�N�I�u�W�F�N�g������
		//	@param[in]	_taskName	�����擾����^�X�N��
		//	@return		�^�X�N�|�C���^ or nullptr
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/21
		------------------------------------------------------------*/
		virtual TaskObject* SearchGetTaskObject(const string& _taskName);

	protected:
		std::list<TaskObject*> taskList;
	};
}

#endif // _Include_TaskSystem_hpp_