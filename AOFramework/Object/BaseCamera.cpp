#include"stdafx.hpp"
#include"BaseCamera.hpp"
#include"../Math/Math.hpp"


using namespace DirectX;
using namespace ao;


AO_CLASS_IMPLEMENT(BaseCamera);


//------------------------------------------------------------------------------
/**	@brief		カメラ初期化メソッド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/04/29	18:13	T.Aoyama	作成
//------------------------------------------------------------------------------
void	BaseCamera::Initialize(const float3& _lookAtDir,const float3& _position,const float3& _upDir)
{
	float3 front(_lookAtDir),up(_upDir),right(_upDir);
	
	right.Cross(front);
	front.Normalize();
	up.Normalize();
	right.Normalize();

	XMFLOAT4X4 mtx;
	MatrixIdentity(&mtx);

	mtx._11 = right.x;
	mtx._12 = right.y;
	mtx._13 = right.z;

	mtx._21 = up.x;
	mtx._22 = up.y;
	mtx._23 = up.z;

	mtx._31 = front.x;
	mtx._32 = front.y;
	mtx._33 = front.z;

	mtx._41 = _position.x;
	mtx._42 = _position.y;
	mtx._43 = _position.z;

	this->transform = mtx;
}


//------------------------------------------------------------------------------
/**	@brief		カメラ初期化メソッド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/04/29	21:29	T.Aoyama	作成
//------------------------------------------------------------------------------
void	BaseCamera::Initialize(const XMFLOAT4X4& _poseMtx)
{
	this->transform = _poseMtx;
}


//------------------------------------------------------------------------------
/**	@brief		カメラ初期化メソッド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/04/29	18:13	T.Aoyama	作成
//------------------------------------------------------------------------------
void	BaseCamera::Initialize(const Transform& _transform)
{
	this->transform = _transform;
}


//------------------------------------------------------------------------------
/**	@brief		カメラ初期化メソッド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/04/29	21:29	T.Aoyama	作成
//------------------------------------------------------------------------------
void	BaseCamera::Initialize(const float3& _position,const float4& _rotation)
{
	this->transform.SetLocalPosition(_position.xm);
	this->transform.SetWorldPosition(float3::zero.xm);
	this->transform.SetLocalRotation(_rotation.xm);
	this->transform.SetWorldRotation(float4::zero_1.xm);
}


//------------------------------------------------------------------------------
/**	@brief		カメラのパラメーター更新メソッド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/04/29	21:29	T.Aoyama	作成
//------------------------------------------------------------------------------
void	BaseCamera::Update()
{
	XMFLOAT4X4 mtx;
	this->transform.GetMatrix(&mtx);

	this->commitPosition.x = mtx._41;
	this->commitPosition.y = mtx._42;
	this->commitPosition.z = mtx._43;

	Vector3Normalize(&this->commitLookAtDir.xm, *reinterpret_cast<XMFLOAT3*>(mtx.m[2]));

	MatrixInverse(&this->viewMatrix, mtx);
}