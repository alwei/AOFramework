/*************************************************************
*	@file   	PhysXSimulationEventCallback.hpp
*	@brief  	PhysXSimulationEventCallback�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/07/08
*************************************************************/

#ifndef _Include_PhysXSimulationEventCallback_hpp_	// �C���N���[�h�K�[�h
#define _Include_PhysXSimulationEventCallback_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<list>
#include"PhysXHeader.hpp"


/*!-----------------------------------------------------------
//	@class  PhysXSimulationEventCallback
//	@brief  �����R�[���o�b�N�Ǘ��N���X
//	@author	Tatsunori Aoyama
//	@date	2014/07/08
------------------------------------------------------------*/
class PhysXSimulationEventCallback : public physx::PxSimulationEventCallback
{
public:
	PhysXSimulationEventCallback(){};
	virtual	~PhysXSimulationEventCallback(){};

	/*!-----------------------------------------------------------
	//	@brief		�j��I�u�W�F�N�g���j�󂳂ꂽ�ۂɌĂяo�����
	//	@note		_constraints	�ݒ肳�ꂽ�j��I�u�W�F�N�g���(������)
	//	@param[in]  _count		�Ăяo����
	//	@author		Tatsunori Aoyama
	//	@date		2014/07/08
	------------------------------------------------------------*/
	virtual	void	onConstraintBreak(
		physx::PxConstraintInfo* _constraints,
		physx::PxU32 _count) final;

	/*!-----------------------------------------------------------
	//	@brief		PxScene::fetchResults���ɃA�N�^�[�������������ɌĂяo�����
	//	@param[in]	_actors	�A�N�^�[�̃_�u���|�C���^
	//	@param[in]  _count	�Ăяo����
	//	@author		Tatsunori Aoyama
	//	@date		2014/07/08
	------------------------------------------------------------*/
	virtual	void	onWake(physx::PxActor** _actors, physx::PxU32 _count);

	/*!-----------------------------------------------------------
	//	@brief		PxScene::fetchResults���ɃA�N�^�[���X���[�v�ɓ��������ɌĂяo�����
	//	@param[in]	_actor	�A�N�^�[�̃_�u���|�C���^
	//	@param[in]  _count	�Ăяo����
	//	@author		Tatsunori Aoyama
	//	@date		2014/07/08
	------------------------------------------------------------*/
	virtual	void	onSleep(physx::PxActor** _actors, physx::PxU32 _count);

	/*!-----------------------------------------------------------
	//	@brief		����̏Փ˃C�x���g�������ɒʒm����̂ɌĂяo�����
	//	@note		���̃C���^�[�t�F�[�X���\�b�h�͎������Ȃ���΂Ȃ�Ȃ�
	//	@param[in]	_pairHeader	��̌`��A�N�^�[�w�b�_
	//	@param[in]  _pairs		�Փ˓��m�̏��i�[�N���X	
	//	@param[in]  _nbPairs		�Փ˓��m�̐�
	//	@author		Tatsunori Aoyama
	//	@date		2014/07/08
	------------------------------------------------------------*/
	virtual	void	onContact(
		const physx::PxContactPairHeader& _pairHeader,
		const physx::PxContactPair* _pairs,
		physx::PxU32 _nbPairs) final;

	/*!-----------------------------------------------------------
	//	@brief		�Փ˃g���K�[���ɌĂяo�����
	//	@param[in]	_pairs	�Փ˓��m�̏��i�[�N���X
	//	@param[in]  _count	�Ăяo����
	//	@author		Tatsunori Aoyama
	//	@date		2014/07/08
	------------------------------------------------------------*/
	virtual	void	onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count);

private:
	PhysXSimulationEventCallback(const PhysXSimulationEventCallback&){};
	void operator = (const PhysXSimulationEventCallback&){};
};

#endif // _Include_PhysXSimulationEventCallback_hpp_
