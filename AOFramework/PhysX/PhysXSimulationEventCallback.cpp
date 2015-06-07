#include"stdafx.hpp"
#include"PhysXSimulationEventCallback.hpp"


using namespace physx;


//------------------------------------------------------------------------------
/**	@brief		破壊オブジェクトが破壊された際に呼び出される */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/08	00:27	T.Aoyama	作成
//------------------------------------------------------------------------------
void PhysXSimulationEventCallback::onConstraintBreak(
	physx::PxConstraintInfo* _constraints,
	physx::PxU32 _count
	)
{
	//未使用
	_constraints;
	_count;
}


//------------------------------------------------------------------------------
/**	@brief		PxScene::fetchResults中にアクターが準備完了時に呼び出される */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	00:29	T.Aoyama	作成
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
/**	@brief		PxScene::fetchResults中にアクターがスリープに入った時に呼び出される */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	00:30	T.Aoyama	作成
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
/**	@brief		特定の衝突イベント発生時に通知するのに呼び出される */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	00:30	T.Aoyama	作成
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
/**	@brief		衝突トリガー時に呼び出される */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	00:31	T.Aoyama	作成
//------------------------------------------------------------------------------
void PhysXSimulationEventCallback::onTrigger(
	physx::PxTriggerPair* _pairs,
	physx::PxU32 _count
	)
{
	for (PxU32 i = 0; i < _count; ++i){
		//ignore _pairs when shapes have been deleted
		//図形が削除されていたら処理をスルー
		if (_pairs[i].flags&(PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER | PxTriggerPairFlag::eDELETED_SHAPE_OTHER) ){
			continue;
		}

		//notice trigger event to object
		//衝突トリガー処理
	}
}