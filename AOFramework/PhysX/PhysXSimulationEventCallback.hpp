/*************************************************************
*	@file   	PhysXSimulationEventCallback.hpp
*	@brief  	PhysXSimulationEventCallbackクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/07/08
*************************************************************/

#ifndef _Include_PhysXSimulationEventCallback_hpp_	// インクルードガード
#define _Include_PhysXSimulationEventCallback_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<list>
#include"PhysXHeader.hpp"


/*!-----------------------------------------------------------
//	@class  PhysXSimulationEventCallback
//	@brief  物理コールバック管理クラス
//	@author	Tatsunori Aoyama
//	@date	2014/07/08
------------------------------------------------------------*/
class PhysXSimulationEventCallback : public physx::PxSimulationEventCallback
{
public:
	PhysXSimulationEventCallback(){};
	virtual	~PhysXSimulationEventCallback(){};

	/*!-----------------------------------------------------------
	//	@brief		破壊オブジェクトが破壊された際に呼び出される
	//	@note		_constraints	設定された破壊オブジェクト情報(未完成)
	//	@param[in]  _count		呼び出す数
	//	@author		Tatsunori Aoyama
	//	@date		2014/07/08
	------------------------------------------------------------*/
	virtual	void	onConstraintBreak(
		physx::PxConstraintInfo* _constraints,
		physx::PxU32 _count) final;

	/*!-----------------------------------------------------------
	//	@brief		PxScene::fetchResults中にアクターが準備完了時に呼び出される
	//	@param[in]	_actors	アクターのダブルポインタ
	//	@param[in]  _count	呼び出す数
	//	@author		Tatsunori Aoyama
	//	@date		2014/07/08
	------------------------------------------------------------*/
	virtual	void	onWake(physx::PxActor** _actors, physx::PxU32 _count);

	/*!-----------------------------------------------------------
	//	@brief		PxScene::fetchResults中にアクターがスリープに入った時に呼び出される
	//	@param[in]	_actor	アクターのダブルポインタ
	//	@param[in]  _count	呼び出す数
	//	@author		Tatsunori Aoyama
	//	@date		2014/07/08
	------------------------------------------------------------*/
	virtual	void	onSleep(physx::PxActor** _actors, physx::PxU32 _count);

	/*!-----------------------------------------------------------
	//	@brief		特定の衝突イベント発生時に通知するのに呼び出される
	//	@note		このインターフェースメソッドは実装しなければならない
	//	@param[in]	_pairHeader	二つの形状アクターヘッダ
	//	@param[in]  _pairs		衝突同士の情報格納クラス	
	//	@param[in]  _nbPairs		衝突同士の数
	//	@author		Tatsunori Aoyama
	//	@date		2014/07/08
	------------------------------------------------------------*/
	virtual	void	onContact(
		const physx::PxContactPairHeader& _pairHeader,
		const physx::PxContactPair* _pairs,
		physx::PxU32 _nbPairs) final;

	/*!-----------------------------------------------------------
	//	@brief		衝突トリガー時に呼び出される
	//	@param[in]	_pairs	衝突同士の情報格納クラス
	//	@param[in]  _count	呼び出す数
	//	@author		Tatsunori Aoyama
	//	@date		2014/07/08
	------------------------------------------------------------*/
	virtual	void	onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count);

private:
	PhysXSimulationEventCallback(const PhysXSimulationEventCallback&){};
	void operator = (const PhysXSimulationEventCallback&){};
};

#endif // _Include_PhysXSimulationEventCallback_hpp_
