/*************************************************************
*	@file   	Collider.hpp
*	@brief  	Collider�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/22
*************************************************************/

#ifndef _Include_Collider_hpp_	// �C���N���[�h�K�[�h
#define _Include_Collider_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include"PhysXHeader.hpp"
#include"PhysX.hpp"
#include"../Math/Math.hpp"


using namespace DirectX;

namespace ao
{
	class PhysX;

	enum class ColliderType : int{
		Plane,
		Box,
		Sphere,
		Capsule,
		Max
	};

	/*!-----------------------------------------------------------
	//	@class  Collider
	//	@brief  �R���C�_�[�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/22
	------------------------------------------------------------*/
	class Collider
	{
	public:
		Collider(
			PhysX* _pPhysx,						///< �t�B�W�b�N�X�Ǘ��N���X
			const physx::PxGeometry& _geometry,	///< �`��
			float _staticFriction,				///< �Ö��C�W��(0.0f~1.0f)
			float _dynamicFriction,				///< �����C�W��(0.0f~1.0f)
			float _restitution);				///< �����W��(0.0f~1.0f)

		~Collider();

		/*!-----------------------------------------------------------
		//	@brief		�}�`�擾
		//	@return		�}�`�̎��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/22
		------------------------------------------------------------*/
		virtual	ColliderType GetColliderType() const = 0;

		/*!-----------------------------------------------------------
		//	@brief		�o�^����PhysX�̐}�`�擾
		//	@return		�o�^�����}�`
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/22
		------------------------------------------------------------*/
		physx::PxShape* GetPxShape(){
			return this->pShape;
		}

		/*!-----------------------------------------------------------
		//	@brief		���Ύp����ݒ�
		//	@param[in]	_x	X���W
		//	@param[in]	_y	Y���W
		//	@param[in]	_z	Z���W
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/22
		------------------------------------------------------------*/
		void SetLocalPose(float _x, float _y, float _z){
			this->pShape->setLocalPose(physx::PxTransform(physx::PxVec3(_x, _y, _z)));
		}

		/*!-----------------------------------------------------------
		//	@brief		���Ύp����ݒ�
		//	@param[in]	_xPos	X���W
		//	@param[in]	_yPos	Y���W
		//	@param[in]	_zPos	Z���W
		//	@param[in]	_xRot	X��]��
		//	@param[in]	_yRot	Y��]��
		//	@param[in]	_zRot	Z��]��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/22
		------------------------------------------------------------*/
		void SetLocalPose(float _xPos, float _yPos, float _zPos, float _xRot, float _yRot, float _zRot){
			XMFLOAT4X4 mtx;
			ao::MatrixIdentity(&mtx);
			ao::MatrixRollPitchYaw(&mtx, _xRot, _yRot, _zRot);
			ao::MatrixSetOffset(&mtx, _xPos, _yPos, _zPos);
			this->pShape->setLocalPose(physx::PxTransform(physx::PxMat44((float*)&mtx)));
		}

		/*!-----------------------------------------------------------
		//	@brief		�g���K�[�}�`�̐ݒ�
		//	@param[in]	_isEnable	�g���K�[�̗L��[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/22
		------------------------------------------------------------*/
		void	EnableTriggerShape(bool _isEnable = false){
			if (_isEnable){
				this->pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
				this->pShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
			}
			else{
				this->pShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
				this->pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
			}
		}

	protected:
		physx::PxMaterial* pMaterial;
		physx::PxShape* pShape;
	};

	/*!-----------------------------------------------------------
	//	@class  PlaneCollider
	//	@brief  �v���[���R���C�_�[�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/24
	------------------------------------------------------------*/
	class PlaneCollider : public Collider
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		�R���X�g���N�^
		//	@param[in]	_pPhysx			�t�B�W�b�N�X�Ǘ��N���X
		//	@param[in]	_halfWidth		���̔���[�ȗ���]
		//	@param[in]	_halfDepth		���s���̔���[�ȗ���]
		//	@param[in]	_sFriction		�Ö��C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_dFriction		�����C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_restitution	�����W��(0.0f-1.0f)[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/22
		------------------------------------------------------------*/
		PlaneCollider(
			PhysX* _pPhysx,
			float _halfWidth = 0.5f,
			float _halfDepth = 0.5f,
			float _sFriction = 1.0f,
			float _dFriction = 1.0f,
			float _restitution = 0.0f);

		/*!-----------------------------------------------------------
		//	@brief		�}�`�擾
		//	@return		�}�`�̎��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/24
		------------------------------------------------------------*/
		ColliderType GetColliderType() const final {
			return ColliderType::Plane;
		}
	};

	/*!-----------------------------------------------------------
	//	@class  BoxCollider
	//	@brief  �{�b�N�X�R���C�_�[�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/24
	------------------------------------------------------------*/
	class BoxCollider : public Collider
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		�R���X�g���N�^
		//	@param[in]	_pPhysx			�t�B�W�b�N�X�Ǘ��N���X
		//	@param[in]	_radius			���a[�ȗ���]
		//	@param[in]	_sFriction		�Ö��C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_dFriction		�����C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_restitution	�����W��(0.0f-1.0f)[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/24
		------------------------------------------------------------*/
		BoxCollider(
			PhysX* _pPhysx,
			float _halfWidth = 0.5f,
			float _halfHeight = 0.5f,
			float _halfDepth = 0.5f,
			float _sFriction = 1.0f,
			float _dFriction = 1.0f,
			float _restitution = 0.0f);

		/*!-----------------------------------------------------------
		//	@brief		�}�`�擾
		//	@return		�}�`�̎��
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/24
		------------------------------------------------------------*/
		ColliderType GetColliderType() const final {
			return ColliderType::Box;
		}
	private:
		float halfWidth;
		float halfHeight;
		float halfDepth;
	};

	/*!-----------------------------------------------------------
	//	@class  SphereCollider
	//	@brief  �X�t�B�A�R���C�_�[�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/24
	------------------------------------------------------------*/
	class SphereCollider : public Collider
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		�R���X�g���N�^
		//	@param[in]	_pPhysx			�t�B�W�b�N�X�Ǘ��N���X
		//	@param[in]	_radius			���a[�ȗ���]
		//	@param[in]	_sFriction		�Ö��C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_dFriction		�����C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_restitution	�����W��(0.0f-1.0f)[�ȗ���]
		------------------------------------------------------------*/
		SphereCollider(
			PhysX* _pPhysx,
			float _radius = 0.5f,
			float _sFriction = 1.0f,
			float _dFriction = 1.0f,
			float _restitution = 0.0f);

		//----------------------------------------------------------------------
		/**	@brief		�}�`�̎�ނ��擾
		//	@return		�}�`�̎�� */
		//----------------------------------------------------------------------
		ColliderType GetColliderType() const final {
			return ColliderType::Sphere;
		}
	private:
		float radius;
	};

	/*!-----------------------------------------------------------
	//	@class  CapsuleCollider
	//	@brief  �J�v�Z���R���C�_�[�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/24
	------------------------------------------------------------*/
	class CapsuleCollider : public Collider
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		�R���X�g���N�^
		//	@param[in]	_pPhysx			�t�B�W�b�N�X�Ǘ��N���X
		//	@param[in]	_radius			���a[�ȗ���]
		//	@param[in]	_halfHeight		�����̔���[�ȗ���]
		//	@param[in]	_sFriction		�Ö��C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_dFriction		�����C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_restitution	�����W��(0.0f-1.0f)[�ȗ���]
		------------------------------------------------------------*/
		CapsuleCollider(
			PhysX* _pPhysx,
			float _radius = 0.5f,
			float _halfHeight = 0.5f,
			float _sFriction = 1.0f,
			float _dFriction = 1.0f,
			float _restitution = 0.0f);

		//----------------------------------------------------------------------
		/**	@brief		�}�`�̎�ނ��擾
		//	@return		�}�`�̎�� */
		//----------------------------------------------------------------------
		ColliderType GetColliderType() const final {
			return ColliderType::Capsule;
		}
	private:
		float radius;
		float halfHeight;
	};
}

#endif // _Include_Collider_hpp_