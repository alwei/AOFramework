#include"stdafx.hpp"
#include"PhysX.hpp"
#include"RigidBody.hpp"


using namespace ao;
using namespace physx;


PhysX::PhysX()
{

#if defined(DEBUG) || defined(_DEBUG)
	this->pPvdConnection = nullptr;
#endif

}


PhysX::~PhysX()
{
	this->Finalize();
}

//------------------------------------------------------------------------------
/**	@brief		シーン作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	14:11	T.Aoyama	作成
//------------------------------------------------------------------------------
ao::PhysXScene* PhysX::CreateScene(
	ao::string _sceneName,
	PhysXSimulationEventCallback* _pCallback,
	const float _gx,
	const float _gy,
	const float _gz)
{
	if (this->pPhysicsSDK == nullptr){
		MessageBox(nullptr, _T("PhysX SDK has not been initialized."), _T("ERROR"), MB_ICONERROR);
		return nullptr;
	}

	std::lock_guard< std::mutex > lock(this->mutex);

	PhysXScene * p = new PhysXScene();
	if (!_sceneName.empty()){
		p->SetSceneName(_sceneName);
	}
	p->Initialize(this->pPhysicsSDK, _pCallback, _gx, _gy, _gz);
	this->pSceneList.push_back(p);

	return p;
}


//------------------------------------------------------------------------------
/**	@brief		シーンの削除 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/11	03:29	T.Aoyama	作成
//------------------------------------------------------------------------------
void ao::PhysX::DeleteScene(PhysXScene* _pScene)
{
	this->pSceneList.remove(_pScene);
	delete _pScene;
}

//------------------------------------------------------------------------------
/**	@brief		材質の作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	01:18	T.Aoyama	作成
//------------------------------------------------------------------------------
PxMaterial* PhysX::CreateMaterial(
	float _staticFriction,
	float _dynamicFriction,
	float _restitution)
{
	if (this->pPhysicsSDK == nullptr){
		MessageBox(nullptr, _T("PhysX SDK has not been initialized."), _T("ERROR"), MB_ICONERROR);
		return nullptr;
	}

	PxMaterial* material = this->pPhysicsSDK->createMaterial(_staticFriction, _dynamicFriction, _restitution);
	return material;
}


//------------------------------------------------------------------------------
/**	@brief		ボックスコライダー作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/31	21:49	T.Aoyama	作成
//------------------------------------------------------------------------------
BoxCollider* PhysX::CreateBoxCollider(
	float _halfWidth,
	float _halfHeight,
	float _halfDepth,
	float _sFriction,
	float _dFriction,
	float _restitution)
{
	std::lock_guard< std::mutex > lock(this->mutex);
	BoxCollider* p = new BoxCollider(this, _halfWidth, _halfHeight, _halfDepth, _sFriction, _dFriction, _restitution);
	this->pColliderList.push_back(p);
	return p;
}


//------------------------------------------------------------------------------
/**	@brief		プレーンコライダー作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/31	21:57	T.Aoyama	作成
//------------------------------------------------------------------------------
PlaneCollider* PhysX::CreatePlaneCollider(
	float _halfWidth,
	float _halfDepth,
	float _sFriction,
	float _dFriction,
	float _restitution)
{
	std::lock_guard< std::mutex > lock(this->mutex);
	PlaneCollider* p = new PlaneCollider(this, _halfWidth, _halfDepth, _sFriction, _dFriction, _restitution);
	this->pColliderList.push_back(p);
	return p;
}


//------------------------------------------------------------------------------
/**	@brief		スフィアコライダー作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/31	21:57	T.Aoyama	作成
//------------------------------------------------------------------------------
SphereCollider* PhysX::CreateSphereCollider(
	float _radius,
	float _sFriction,
	float _dFriction,
	float _restitution)
{
	std::lock_guard< std::mutex > lock(this->mutex);
	SphereCollider* p = new SphereCollider(this, _radius, _sFriction, _dFriction, _restitution);
	this->pColliderList.push_back(p);
	return p;
}


//------------------------------------------------------------------------------
/**	@brief		カプセルコライダー作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/31	21:57	T.Aoyama	作成
//------------------------------------------------------------------------------
CapsuleCollider* PhysX::CreateCapsuleCollider(
	float _radius,
	float _halfHeight,
	float _sFriction,
	float _dFriction,
	float _restitution)
{
	std::lock_guard< std::mutex > lock(this->mutex);
	CapsuleCollider* p = new CapsuleCollider(this, _radius, _halfHeight, _sFriction, _dFriction, _restitution);
	this->pColliderList.push_back(p);
	return p;
}


//------------------------------------------------------------------------------
/**	@brief		剛体の作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	01:18	T.Aoyama	作成
//------------------------------------------------------------------------------
ao::Rigidbody* PhysX::CreateRigidbody(const PxTransform& _pose)
{
	if (this->pPhysicsSDK == nullptr){
		MessageBox(nullptr, _T("PhysX SDK has not been initialized."), _T("ERROR"), MB_ICONERROR);
		return nullptr;
	}

	std::lock_guard< std::mutex > lock(this->mutex);
	ao::Rigidbody* p = new ao::Rigidbody(this->pPhysicsSDK, _pose);
	this->pRigidbodyList.push_back(p);
	return p;
}

//------------------------------------------------------------------------------
/**	@brief		図形の作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/09	01:18	T.Aoyama	作成
//------------------------------------------------------------------------------
PxShape* PhysX::CreateShape(
	const PxGeometry& _geometry,
	const PxMaterial& _material)
{
	if (this->pPhysicsSDK == nullptr){
		MessageBox(nullptr, _T("PhysX SDK has not been initialized."), _T("ERROR"), MB_ICONERROR);
		return nullptr;
	}

	PxShape* shape = this->pPhysicsSDK->createShape(_geometry, _material);
	return shape;
}

//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/08	17:03	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT PhysX::Initialize()
{
	// create foundation
	this->pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, this->allocator, this->errorCallback);
	if (this->pFoundation == nullptr){
		::MessageBox(nullptr, _T("\nFailed Create PhysX Foundation"), _T("Error"), MB_OK);
		return E_FAIL;
	}

	// create PhysX SDK
	this->pPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *this->pFoundation, PxTolerancesScale());
	if (this->pPhysicsSDK == nullptr){
		::MessageBox(NULL, _T("\nFailed Create PhysX SDK"), _T("Error"), MB_OK);
		return E_FAIL;
	}

#ifdef _DEBUG
	// Connect PhysX Visual Debugger
	if (this->pPhysicsSDK->getPvdConnectionManager() != nullptr){
		this->pPvdConnection = PxVisualDebuggerExt::createConnection(
			this->pPhysicsSDK->getPvdConnectionManager(),
			"127.0.0.1",	// local host ip
			5425,			// port
			100,			// timeout millisec
			PxVisualDebuggerExt::getAllConnectionFlags());
	}
#endif

	// success
	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		終了処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/08	17:04	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT PhysX::Finalize()
{
	for (auto& it : this->pSceneList)
		delete it;

	for (auto& it : this->pColliderList)
		delete it;

	for (auto& it : this->pRigidbodyList)
		delete it;

	this->pColliderList.clear();
	this->pRigidbodyList.clear();
	this->pSceneList.clear();

#if defined(_DEBUG) || defined(DEBUG)
	if (this->pPvdConnection != nullptr){
		this->pPvdConnection->release();
		this->pPvdConnection = nullptr;
	}
#endif

	if (this->pPhysicsSDK != nullptr){
		this->pPhysicsSDK->release();
		this->pPhysicsSDK = nullptr;
	}

	if (this->pFoundation != nullptr){
		this->pFoundation->release();
		this->pFoundation = nullptr;
	}

	return S_OK;
}