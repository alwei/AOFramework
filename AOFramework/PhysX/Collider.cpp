#include"stdafx.hpp"
#include"Collider.hpp"
#include"../Debug/DebugSystem.hpp"


using namespace ao;


Collider::Collider(
	PhysX* _pPhysX,
	const physx::PxGeometry& _geometry,
	float _staticFriction,
	float _dynamicFriction,
	float _restitution)
{
	this->pMaterial = _pPhysX->CreateMaterial(_staticFriction, _dynamicFriction, _restitution);
#ifdef __AO_DEBUG__
	if (this->pMaterial == nullptr)
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[Collider::Collider]Failed create material\n"));
#endif
	this->pShape = _pPhysX->CreateShape(_geometry, *this->pMaterial);
	this->SetLocalPose(0,0,0);
}


Collider::~Collider()
{

}


PlaneCollider::PlaneCollider(
	PhysX* _pPhysx,
	float _halfWidth,
	float _halfDepth,
	float _sFriction,
	float _dFriction,
	float _restitution) : 
	Collider(_pPhysx, physx::PxBoxGeometry(_halfDepth, 0.000001f, _halfDepth), _sFriction, _dFriction, _restitution)
{

}


BoxCollider::BoxCollider(
	PhysX* _pPhysx,
	float _halfWidth,
	float _halfHeight,
	float _halfDepth,
	float _sFriction,
	float _dFriction,
	float _restitution) :
	Collider(_pPhysx, physx::PxBoxGeometry(_halfWidth, _halfHeight, _halfDepth), _sFriction, _dFriction, _restitution)
{
	this->halfWidth = _halfWidth;
	this->halfHeight = _halfHeight;
	this->halfDepth = _halfDepth;
}


SphereCollider::SphereCollider(
	PhysX* _pPhysx,
	float _radius,
	float _sFriction,
	float _dFriction,
	float _restitution) :
	Collider(_pPhysx, physx::PxSphereGeometry(_radius), _sFriction, _dFriction, _restitution)
{
	this->radius = _radius;
}


CapsuleCollider::CapsuleCollider(
	PhysX* _pPhysx,
	float _radius,
	float _halfHeight,
	float _sFriction,
	float _dFriction,
	float _restitution) :
	Collider(_pPhysx, physx::PxCapsuleGeometry(_radius, _halfHeight), _sFriction, _dFriction, _restitution)
{
	this->radius = _radius;
	this->halfHeight = _halfHeight;
}
