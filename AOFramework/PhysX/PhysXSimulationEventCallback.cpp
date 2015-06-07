#include"stdafx.hpp"
#include"PhysXSimulationEventCallback.hpp"


using namespace physx;


//------------------------------------------------------------------------------
/**	@brief		�j��I�u�W�F�N�g���j�󂳂ꂽ�ۂɌĂяo����� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/08	00:27	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void PhysXSimulationEventCallback::onConstraintBreak(
	physx::PxConstraintInfo* _constraints,
	physx::PxU32 _count
	)
{
	//���g�p
	_constraints;
	_count;
}


//------------------------------------------------------------------------------
/**	@brief		PxScene::fetchResults���ɃA�N�^�[�������������ɌĂяo����� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	00:29	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void PhysXSimulationEventCallback::onWake(
	physx::PxActor** _actors,
	physx::PxU32 _count
	)
{
	_actors;
	_count;
}


//------------------------------------------------------------------------------
/**	@brief		PxScene::fetchResults���ɃA�N�^�[���X���[�v�ɓ��������ɌĂяo����� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	00:30	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void PhysXSimulationEventCallback::onSleep(
	physx::PxActor** _actors,
	physx::PxU32 _count
	)
{
	_actors;
	_count;
}


//------------------------------------------------------------------------------
/**	@brief		����̏Փ˃C�x���g�������ɒʒm����̂ɌĂяo����� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	00:30	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void PhysXSimulationEventCallback::onContact(
	const physx::PxContactPairHeader& _pairHeader,
	const physx::PxContactPair* _pairs,
	physx::PxU32 _nbPairs
	)
{
	_pairHeader;
	_pairs;
	_nbPairs;
}


//------------------------------------------------------------------------------
/**	@brief		�Փ˃g���K�[���ɌĂяo����� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	00:31	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void PhysXSimulationEventCallback::onTrigger(
	physx::PxTriggerPair* _pairs,
	physx::PxU32 _count
	)
{
	for (PxU32 i = 0; i < _count; ++i){
		//ignore _pairs when shapes have been deleted
		//�}�`���폜����Ă����珈�����X���[
		if (_pairs[i].flags&(PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER | PxTriggerPairFlag::eDELETED_SHAPE_OTHER) ){
			continue;
		}

		//notice trigger event to object
		//�Փ˃g���K�[����
	}
}