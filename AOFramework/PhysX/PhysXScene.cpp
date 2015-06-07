#include"stdafx.hpp"
#include"PhysXScene.hpp"


using namespace ao;
using namespace physx;

namespace {
	const float gSimulationTime = 1.0f / 60.0f;
}


PhysXScene::PhysXScene() :
pCpuDispatcher(nullptr),
pPhysicsScene(nullptr)
{
	this->sceneName.assign(_T("no name"));
}


PhysXScene::~PhysXScene()
{
	this->Finalize();
}


//------------------------------------------------------------------------------
/**	@brief		形状に図形をアタッチ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	01:18	T.Aoyama	作成
//------------------------------------------------------------------------------
void PhysXScene::AttachShape(PxRigidActor* _pActor, PxShape* _shape)
{
	std::lock_guard< std::mutex > lock(this->mutex);
	_pActor->attachShape(*_shape);
}


//------------------------------------------------------------------------------
/**	@brief		形状から図形を解除 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	01:18	T.Aoyama	作成
//------------------------------------------------------------------------------
void PhysXScene::DetachShape(PxRigidActor* _pActor, PxShape* _shape)
{
	std::lock_guard< std::mutex > lock(this->mutex);
	_pActor->detachShape(*_shape);
}


//------------------------------------------------------------------------------
/**	@brief		物体をシーンに追加 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	01:23	T.Aoyama	作成
//------------------------------------------------------------------------------
void PhysXScene::AddActor(PxActor& _actor)
{
	if (this->pPhysicsScene == nullptr)
		return;

	std::lock_guard< std::mutex > lock(this->mutex);
	this->pPhysicsScene->addActor(_actor);
	this->pActorList.push_back(&_actor);
}


//------------------------------------------------------------------------------
/**	@brief		物体をシーンから削除 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	01:24	T.Aoyama	作成
//------------------------------------------------------------------------------
void PhysXScene::RemoveActor(PxActor& _actor)
{
	if (this->pPhysicsScene == nullptr)
		return;

	std::lock_guard< std::mutex > lock(this->mutex);
	this->pPhysicsScene->removeActor(_actor);
	this->pActorList.remove(&_actor);
}


//------------------------------------------------------------------------------
/**	@brief		シーン全ての物体を削除 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	01:24	T.Aoyama	作成
//------------------------------------------------------------------------------
void PhysXScene::FlushActor()
{
	if (this->pPhysicsScene == nullptr || this->pActorList.empty())
		return;

	for (auto& it : this->pActorList)
		this->pPhysicsScene->removeActor(*it);
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	13:37	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT PhysXScene::Initialize(
	PxPhysics* _pPxSdk,
	PhysXSimulationEventCallback* _pCallback,
	const float _gx,
	const float _gy,
	const float _gz)
{
	// create PhysX Scene
	PxSceneDesc sceneDesc(_pPxSdk->getTolerancesScale());
	sceneDesc.gravity = PxVec3(_gx, _gy, _gz);
	if (sceneDesc.cpuDispatcher == nullptr){
		this->pCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		if (this->pCpuDispatcher == nullptr){
			::MessageBox(NULL, _T("\nFailed Create PhysX CpuDispatcher"), _T("Error"), MB_OK);
			return E_FAIL;
		}
		sceneDesc.cpuDispatcher = this->pCpuDispatcher;
	}
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	this->pPhysicsScene = _pPxSdk->createScene(sceneDesc);
	if (this->pPhysicsScene == nullptr){
		::MessageBox(nullptr, _T("\nFailed Create PhysX Scene"), _T("Error"), MB_OK);
		return E_FAIL;
	}

	// set simulation event callback
	this->pPhysicsScene->setSimulationEventCallback(_pCallback);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		終了処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	13:38	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT PhysXScene::Finalize()
{
	this->FlushActor();

	if (this->pCpuDispatcher != nullptr){
		this->pCpuDispatcher->release();
		this->pCpuDispatcher = nullptr;
	}

	if (this->pPhysicsScene != nullptr){
		this->pPhysicsScene->release();
		this->pPhysicsScene = nullptr;
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	13:52	T.Aoyama	作成
//------------------------------------------------------------------------------
void PhysXScene::Simulate()
{
	this->pPhysicsScene->simulate(gSimulationTime);
	this->pPhysicsScene->fetchResults(true);
}


//------------------------------------------------------------------------------
/**	@brief		一番近いオブジェクトと衝突判定を行う */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/11/08	15:13	T.Aoyama	作成
//------------------------------------------------------------------------------
BOOL PhysXScene::Raycast(
	const XMFLOAT3& _origin,
	const XMFLOAT3& _unitDir,
	const float _maxDistance,
	RaycastHit* _pOut)
{
	PxRaycastBuffer hit;
	PxVec3 origin(_origin.x, _origin.y, _origin.z);
	PxVec3 unitDir(_unitDir.x, _unitDir.y, _unitDir.z);
	this->pPhysicsScene->raycast(origin, unitDir, _maxDistance, hit);

	//内部からのレイかどうか判定
	DWORD insideFlag = 0;
	insideFlag |= ((hit.block.distance == 0.0f) << 0);
	insideFlag |= ((hit.block.normal.x == 0 - _unitDir.x) << 1);
	insideFlag |= ((hit.block.normal.y == 0 - _unitDir.y) << 2);
	insideFlag |= ((hit.block.normal.z == 0 - _unitDir.z) << 3);
	insideFlag |= ((hit.block.position.x == 0.0f) << 4);
	insideFlag |= ((hit.block.position.y == 0.0f) << 5);
	insideFlag |= ((hit.block.position.z == 0.0f) << 6);
	if (insideFlag == 0x7F)
		return FALSE;

	if (_pOut != nullptr){
		_pOut->distance = static_cast<float>(hit.block.distance);
		_pOut->point.x = static_cast<float>(hit.block.position.x);
		_pOut->point.y = static_cast<float>(hit.block.position.y);
		_pOut->point.z = static_cast<float>(hit.block.position.z);
		if( hit.block.actor )
			_pOut->userData = hit.block.actor->userData;
	}

	return static_cast<BOOL>(hit.hasBlock);
}