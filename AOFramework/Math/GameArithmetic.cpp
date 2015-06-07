#include"stdafx.hpp"
#include"../Define/SystemDefine.hpp"
#include"GameArithmetic.hpp"


using namespace ao;





Transform::Transform()
{
	::SecureZeroMemory(&this->localPosition, sizeof(XMFLOAT3));
	::SecureZeroMemory(&this->worldPosition, sizeof(XMFLOAT3));

	this->localRotation.x = 0.0f;
	this->localRotation.y = 0.0f;
	this->localRotation.z = 0.0f;
	this->localRotation.w = 1.0f;
	this->worldRotation.x = 0.0f;
	this->worldRotation.y = 0.0f;
	this->worldRotation.z = 0.0f;
	this->worldRotation.w = 1.0f;
	this->localScale.x = 1.0f;
	this->localScale.y = 1.0f;
	this->localScale.z = 1.0f;
}


Transform::Transform(
	float _lx, float _ly, float _lz, float _wx, float _wy, float _wz,
	float _lxDir, float _lyDir, float _lzDir, float _lRad,
	float _wxDir, float _wyDir, float _wzDir, float _wRad,
	float _sx, float _sy, float _sz)
{
	this->localPosition.x = _lx;
	this->localPosition.y = _ly;
	this->localPosition.z = _lz;
	this->worldPosition.x = _wx;
	this->worldPosition.y = _wy;
	this->worldPosition.z = _wz;
	this->localRotation.x = _lxDir;
	this->localRotation.y = _lyDir;
	this->localRotation.z = _lzDir;
	this->localRotation.w = _lRad;
	this->worldRotation.x = _wxDir;
	this->worldRotation.y = _wyDir;
	this->worldRotation.z = _wzDir;
	this->worldRotation.w = _wRad;
	this->localScale.x = _sx;
	this->localScale.y = _sy;
	this->localScale.z = _sz;
}


Transform::Transform(const Transform& _inst)
{
	this->localScale = _inst.localPosition;
	this->localRotation = _inst.localRotation;
	this->worldRotation = _inst.worldRotation;
	this->localPosition = _inst.localPosition;
	this->worldPosition = _inst.worldPosition;
}


//------------------------------------------------------------------------------
/**	@brief		•ûŒüŽw’è‰ñ“] */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/01/04	03:07	T.Aoyama	ì¬
//------------------------------------------------------------------------------
void Transform::LookAtDirection(const XMFLOAT3& _dir)
{
	XMFLOAT3 cross;
	ao::Vector3Cross(&cross, ao::float3::back.xm, _dir);
	float rad = ::acosf(ao::Vector3Dot(float3::back.xm, _dir));
	ao::QuaternionRotationAxis(&this->localRotation, cross, rad);
}



//------------------------------------------------------------------------------
/**	@brief		s—ñ‚É‚µ‚ÄŽæ“¾ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/01/04	03:07	T.Aoyama	ì¬
//------------------------------------------------------------------------------
const XMFLOAT4X4* Transform::GetMatrix(XMFLOAT4X4* const _pOut)
{
	TRANSFORM_NULL_CHECK(_pOut);

	XMFLOAT4X4 scale, lRot, wRot, trans;

	ao::MatrixScale(&scale, this->localScale);
	ao::QuaternionToMatrix(&lRot, this->localRotation);
	ao::QuaternionToMatrix(&wRot, this->worldRotation);
	ao::MatrixTranslate(&trans, this->localPosition);

	ao::MatrixMultiply(_pOut, scale, lRot);
	ao::MatrixMultiply(_pOut, *_pOut, trans);
	ao::MatrixMultiply(_pOut, *_pOut, wRot);

	_pOut->_41 += this->worldPosition.x;
	_pOut->_42 += this->worldPosition.y;
	_pOut->_43 += this->worldPosition.z;

	return _pOut;
}