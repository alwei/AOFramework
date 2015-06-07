/*************************************************************
*	@file   	GameArithmetic.hpp
*	@brief  	GameArithmetic�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/20
*************************************************************/

#ifndef _Include_GameArithmetic_hpp_	// �C���N���[�h�K�[�h
#define _Include_GameArithmetic_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include<DirectXMath.h>
#include"Math.hpp"
#include"MathDefine.hpp"


using namespace DirectX;

#if defined(__AO_DEBUG__)
#define	TRANSFORM_NULL_CHECK(_ptr)	\
	if(_ptr==nullptr)				\
	{								\
		return nullptr;				\
	}								
#else
#define TRANSFORM_NULL_CHECK(_ptr)
#endif


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  Transform
	//	@brief  �I�u�W�F�N�g��STR�̐���N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/20
	------------------------------------------------------------*/
	class Transform final
	{
	public:
		Transform();

		Transform(
			float _lx, float _ly, float _lz, float _wx = 0, float _wy = 0, float _wz = 0,
			float _lxDir = 0, float _lyDir = 0, float _lzDir = 0, float _lRad = 1,
			float _wxDir = 0, float _wyDir = 0, float _wzDir = 0, float _wRad = 1,
			float _sx = 1, float _sy = 1, float _sz = 1);

		Transform(const Transform& _inst);

		~Transform(){}

		/*!-----------------------------------------------------------
		//	@brief		�X�P�[���l�ݒ�
		//	@param[in]	_x	X�v�f
		//  @param[in]  _y	Y�v�f
		//  @param[in]  _z	Z�v�f
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/01
		------------------------------------------------------------*/
		inline void	SetScale(float _x, float _y, float _z)
		{
			this->localScale.x = _x;
			this->localScale.y = _y;
			this->localScale.z = _z;
		}

		/*!-----------------------------------------------------------
		//	@brief		�X�P�[���l�ݒ�
		//	@param[in]	_scale	�g��x�N�g��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/01
		------------------------------------------------------------*/
		inline void	SetScale(const XMFLOAT3& _scale)
		{
			this->localScale.x = _scale.x;
			this->localScale.y = _scale.y;
			this->localScale.z = _scale.z;
		}

		/*!-----------------------------------------------------------
		//	@brief		���[�J����]�N�H�[�^�j�I���ݒ�
		//	@param[in]	_qRot	��]�N�H�[�^�j�I��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/01
		------------------------------------------------------------*/
		inline void	SetLocalRotation(const XMFLOAT4& _qRot)
		{
			this->localRotation.x = _qRot.x;
			this->localRotation.y = _qRot.y;
			this->localRotation.z = _qRot.z;
			this->localRotation.w = _qRot.w;
		}

		/*!-----------------------------------------------------------
		//	@brief		��]�N�H�[�^�j�I���ݒ�
		//	@param[in]	_x		X�v�f
		//  @param[in]  _y		Y�v�f
		//  @param[in]  _z		Z�v�f
		//  @param[in]  _w		W�v�f
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/01
		------------------------------------------------------------*/
		inline void	SetLocalRotation(float _x, float _y, float _z, float _w)
		{
			this->localRotation.x = _x;
			this->localRotation.y = _y;
			this->localRotation.z = _z;
			this->localRotation.w = _w;
		}

		/*!-----------------------------------------------------------
		//	@brief		��]�N�H�[�^�j�I���ݒ�
		//  @param[in]  _axis	��]��
		//  @param[in]  _radian	���W�A���l
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/01
		------------------------------------------------------------*/
		inline void	SetLocalRotation(const XMFLOAT3& _axis, float _radian)
		{
			ao::QuaternionRotationAxis(&this->localRotation, _axis, _radian);
		}

		/*!-----------------------------------------------------------
		//	@brief		���[���h��]�N�H�[�^�j�I���ݒ�
		//	@param[in]	_qRot	��]�N�H�[�^�j�I��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		inline void	SetWorldRotation(const XMFLOAT4& _qRot)
		{
			this->worldRotation.x = _qRot.x;
			this->worldRotation.y = _qRot.y;
			this->worldRotation.z = _qRot.z;
			this->worldRotation.w = _qRot.w;
		}

		/*!-----------------------------------------------------------
		//	@brief		��]�N�H�[�^�j�I���ݒ�
		//	@param[in]	_x		X�v�f
		//  @param[in]  _y		Y�v�f
		//  @param[in]  _z		Z�v�f
		//  @param[in]  _w		W�v�f
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		inline void	SetWorldRotation(float _x, float _y, float _z, float _w)
		{
			this->worldRotation.x = _x;
			this->worldRotation.y = _y;
			this->worldRotation.z = _z;
			this->worldRotation.w = _w;
		}

		/*!-----------------------------------------------------------
		//	@brief		��]�N�H�[�^�j�I���ݒ�
		//  @param[in]  _axis	��]��
		//  @param[in]  _radian	���W�A���l
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		inline void	SetWorldRotation(const XMFLOAT3& _axis, float _radian)
		{
			ao::QuaternionRotationAxis(&this->worldRotation, _axis, _radian);
		}

		/*!-----------------------------------------------------------
		//	@brief		���[�J�����W�ݒ�
		//	@param[in]	_x	X���W
		//  @param[in]  _y	Y���W
		//  @param[in]  _z	Z���W
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/01
		------------------------------------------------------------*/
		inline void	SetLocalPosition(float _x, float _y, float _z)
		{
			this->localPosition.x = _x;
			this->localPosition.y = _y;
			this->localPosition.z = _z;
		}

		/*!-----------------------------------------------------------
		//	@brief		���[�J�����W�ݒ�
		//	@param[in]	_pos	���W
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/01
		------------------------------------------------------------*/
		inline void	SetLocalPosition(const XMFLOAT3& _pos)
		{
			this->localPosition.x = _pos.x;
			this->localPosition.y = _pos.y;
			this->localPosition.z = _pos.z;
		}

		/*!-----------------------------------------------------------
		//	@brief		���[���h���W�ݒ�
		//	@param[in]	_x	X���W
		//  @param[in]  _y	Y���W
		//  @param[in]  _z	Z���W
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/17
		------------------------------------------------------------*/
		inline void	SetWorldPosition(float _x, float _y, float _z)
		{
			this->worldPosition.x = _x;
			this->worldPosition.y = _y;
			this->worldPosition.z = _z;
		}

		/*!-----------------------------------------------------------
		//	@brief		���[���h���W�ݒ�
		//	@param[in]	_pos	���W
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/17
		------------------------------------------------------------*/
		inline void	SetWorldPosition(const XMFLOAT3& _pos)
		{
			this->worldPosition.x = _pos.x;
			this->worldPosition.y = _pos.y;
			this->worldPosition.z = _pos.z;
		}

		/*!-----------------------------------------------------------
		//	@brief		�����w���]
		//	@param[in]	_dir	��������������
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/30
		------------------------------------------------------------*/
		void	LookAtDirection(const XMFLOAT3& _dir);

		/*!-----------------------------------------------------------
		//	@brief		�s��ɂ��Ď擾
		//  @param[out]	_pOut	�o�͂���s��|�C���^
		//  @return     �o�͂���s��|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/01
		------------------------------------------------------------*/
		const XMFLOAT4X4* GetMatrix(XMFLOAT4X4* const _pOut);

		/*!-----------------------------------------------------------
		//	@brief		���ʃx�N�g���擾
		//	@param[out]	_pOut	�o�͂���x�N�g��[�ȗ���]
		//	@return		�o�̓x�N�g��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		inline const XMFLOAT3* GetFoward(XMFLOAT3* _pOut)
		{
			TRANSFORM_NULL_CHECK(_pOut);
			ao::Vector3Rotate(_pOut, ao::float3::foward.xm, this->localRotation);
			return _pOut;
		}

		/*!-----------------------------------------------------------
		//	@brief		��x�N�g���擾
		//	@param[out]	_pOut	�o�͂���x�N�g��[�ȗ���]
		//	@return		�o�̓x�N�g��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		inline const XMFLOAT3* GetUp(XMFLOAT3* _pOut)
		{
			TRANSFORM_NULL_CHECK(_pOut);
			ao::Vector3Rotate(_pOut, ao::float3::up.xm, this->localRotation);
			return _pOut;
		}

		/*!-----------------------------------------------------------
		//	@brief		�E�x�N�g���擾
		//	@param[out]	_pOut	�o�͂���x�N�g��[�ȗ���]
		//	@return		�o�̓x�N�g��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		inline const XMFLOAT3* GetRight(XMFLOAT3* _pOut)
		{
			TRANSFORM_NULL_CHECK(_pOut);
			ao::Vector3Rotate(_pOut, ao::float3::right.xm, this->localRotation);
			return _pOut;
		}

		/*!-----------------------------------------------------------
		//	@brief		������Z�q�̃I�[�o�[���[�h
		//	@param[in]	_inst	�E�Ӓl
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		inline void operator=(const Transform& _inst)
		{
			this->localScale = _inst.localScale;
			this->localRotation = _inst.localRotation;
			this->worldRotation = _inst.worldRotation;
			this->localPosition = _inst.localPosition;
			this->worldPosition = _inst.worldPosition;
		}

		/*!-----------------------------------------------------------
		//	@brief		������Z�q�̃I�[�o�[���[�h
		//	@param[in]	_inst	�E�Ӓl
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		inline void operator=(const XMFLOAT4X4& _mtx)
		{
			this->localScale.x = _mtx._11;
			this->localScale.y = _mtx._22;
			this->localScale.z = _mtx._33;
			MatrixToQuaternion(&this->localRotation, _mtx);
			this->worldRotation = float4::zero_1.xm;
			this->localPosition.x = _mtx._41;
			this->localPosition.y = _mtx._42;
			this->localPosition.z = _mtx._43;
			this->worldPosition.x = 0;
			this->worldPosition.y = 0;
			this->worldPosition.z = 0;
		}

	public:
		XMFLOAT3 localScale;
		XMFLOAT4 localRotation;
		XMFLOAT4 worldRotation;
		XMFLOAT3 localPosition;
		XMFLOAT3 worldPosition;
	};

	/*!-----------------------------------------------------------
	//	@brief		�e�q�p���̐ώZ
	//	@param[out]	_pOut		�o�͂���p��
	//	@param[in]	_trans1		�q���p��
	//	@param[in]	_trans2		�e�p��
	//	@return		�o�͂���|�C���^
	//	@author		Tatsunori Aoyama
	//	@date		2014/10/18
	------------------------------------------------------------*/
	inline Transform* TransformMultiply(Transform* _pOut, const Transform& _trans1, const Transform& _trans2)
	{
		_pOut->localPosition = _trans1.localPosition;
		_pOut->localRotation = _trans1.localRotation;
		ao::Vector3Rotate(&_pOut->worldPosition, _trans2.localPosition, _trans2.worldRotation);

		_pOut->worldPosition.x += _trans1.worldPosition.x + _trans2.worldPosition.x;
		_pOut->worldPosition.y += _trans1.worldPosition.y + _trans2.worldPosition.y;
		_pOut->worldPosition.z += _trans1.worldPosition.z + _trans2.worldPosition.z;
		QuaternionMultiply(&_pOut->worldRotation, _trans1.worldRotation, _trans2.localRotation);
		QuaternionMultiply(&_pOut->worldRotation, _pOut->worldRotation, _trans2.worldRotation);

		_pOut->localScale.x = _trans1.localScale.x * _trans2.localScale.x;
		_pOut->localScale.y = _trans1.localScale.y * _trans2.localScale.y;
		_pOut->localScale.z = _trans1.localScale.z * _trans2.localScale.z;

		return _pOut;
	}
}

#endif // _Include_GameArithmetic_hpp_