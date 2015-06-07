/*************************************************************
*	@file   	Rigidbody.hpp
*	@brief  	Rigidbody�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/07/10
*************************************************************/

#ifndef _Include_Rigidbody_hpp_	// �C���N���[�h�K�[�h
#define _Include_Rigidbody_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"PhysXHeader.hpp"
#include"PhysX.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  Rigidbody
	//	@brief  ���̃N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/10
	------------------------------------------------------------*/
	class Rigidbody
	{
	public:
		Rigidbody(physx::PxPhysics* _pSDK, physx::PxTransform _pose){
			this->pRigidbody = _pSDK->createRigidDynamic(_pose);
			this->pRigidbody->userData = nullptr;
		}
		~Rigidbody(){};

		/*!-----------------------------------------------------------
		//	@brief		�A�N�e�B�u�ݒ�
		//	@param[in]	_isActive	�L�����ݒ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	SetActive(bool _isActive = true){
			this->active = _isActive;
			this->pRigidbody->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !_isActive);
		}

		/*!-----------------------------------------------------------
		//	@brief		�A�N�e�B�u��Ԃ̎擾
		//	@return		true�F�L���@false�F����
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		bool	IsActive(){
			return this->active;
		}

		/*!-----------------------------------------------------------
		//	@brief		�d�͂̐ݒ�
		//	@param[in]	_isUseGravity	�d�͂̐ݒ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	EnableGravity(bool _useGravity = true){
			this->useGravity = _useGravity;
			this->pRigidbody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !_useGravity);
		}

		/*!-----------------------------------------------------------
		//	@brief		�d�̗͂L����Ԏ擾
		//	@return		true�F�d�͗L���@false�F�d�͖���
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		bool	IsUseGravity(){
			return this->useGravity;
		}

		/*!-----------------------------------------------------------
		//	@brief		�O�͂̉e���ݒ�
		//	@param[in]	_isKinematic	�O�͂̉e���ݒ�itrue:�󂯂Ȃ� / false:�󂯂�j
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	EnableKinematic(bool _isKinematic = false){
			this->isKinematic = _isKinematic;
			this->pRigidbody->setRigidBodyFlag(physx::PxRigidDynamicFlag::eKINEMATIC, _isKinematic);
		}

		/*!-----------------------------------------------------------
		//	@brief		�O�͉e���̎擾
		//	@return		true�F�e�����󂯂Ȃ��@false�F�e�����󂯂�
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		bool	IsKinematic(){
			return this->isKinematic;
		}

		/*!-----------------------------------------------------------
		//	@brief		���x��ݒ�
		//	@param[in]	_x	���xX
		//	@param[in]  _y	���xY
		//	@param[in]  _z	���xZ
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	SetLinearVelocity(float _x, float _y, float _z){
			this->pRigidbody->setLinearVelocity(physx::PxVec3(_x, _y, _z));
		}

		/*!-----------------------------------------------------------
		//	@brief		���x���擾
		//	@param[out]	pVelocity	�o�͂���x�N�g���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	GetLinearVelocity(physx::PxVec3* pVelocity){
			(*pVelocity) = this->pRigidbody->getLinearVelocity();
		}

		/*!-----------------------------------------------------------
		//	@brief		��]���x��ݒ�
		//	@param[in]	_x	��]���xX
		//	@param[in]  _y	��]���xY
		//	@param[in]  _z	��]���xZ
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	SetAngularVelocity(float _x, float _y, float _z){
			this->pRigidbody->setAngularVelocity(physx::PxVec3(_x, _y, _z));
		}

		/*!-----------------------------------------------------------
		//	@brief		��]���x���擾
		//	@param[out]	pAngularVelocity	�o�͂����]���x�x�N�g���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	GetAngularVelocity(physx::PxVec3* pAngularVelocity){
			(*pAngularVelocity) = this->pRigidbody->getAngularVelocity();
		}

		/*!-----------------------------------------------------------
		//	@brief		�p����ݒ�
		//	@param[in]	_xPos	X���W
		//	@param[in]  _yPos	Y���W
		//	@param[in]  _zPos	Z���W
		//	@param[in]	_xQtn	�N�H�[�^�j�I���v�fX
		//	@param[in]	_yQtn	�N�H�[�^�j�I���v�fY
		//	@param[in]	_zQtn	�N�H�[�^�j�I���v�fZ
		//	@param[in]	_wQtn	�N�H�[�^�j�I���v�fW
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	SetTransform(
			float _xPos, float _yPos, float _zPos,
			float _xQtn, float _yQtn, float _zQtn, float _wQtn)
		{
			physx::PxVec3 pos(_xPos, _yPos, _zPos);
			physx::PxQuat rot(_xQtn, _yQtn, _zQtn, _wQtn);
			this->pRigidbody->setGlobalPose(physx::PxTransform(pos, rot));
		}

		/*!-----------------------------------------------------------
		//	@brief		�p����ݒ�
		//  @param[in]  _pose	�p��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	SetTransform(physx::PxTransform& _pose){
			this->pRigidbody->setGlobalPose(_pose);
		}

		/*!-----------------------------------------------------------
		//	@brief		�p�����擾
		//	@param[out]	pTransfrom	�o�͂���p��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	GetTransform(physx::PxTransform* pTransform){
			(*pTransform) = this->pRigidbody->getGlobalPose();
		}

		/*!-----------------------------------------------------------
		//	@brief		���͂����Z
		//	@param[in]	_x	X�v�f
		//	@param[in]  _y	Y�v�f
		//	@param[in]  _z	Z�v�f
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	AddForce(float _x, float _y, float _z){
			this->pRigidbody->addForce(physx::PxVec3(_x, _y, _z));
		}

		/*!-----------------------------------------------------------
		//	@brief		�g���N�����Z
		//	@param[in]	_x	X�v�f
		//	@param[in]  _y	Y�v�f
		//	@param[in]  _z	Z�v�f
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	AddTorque(float _x, float _y, float _z){
			this->pRigidbody->addTorque(physx::PxVec3(_x, _y, _z));
		}


		/*!-----------------------------------------------------------
		//	@brief		���̎擾
		//  @return     ���I���̎擾
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/01
		------------------------------------------------------------*/
		physx::PxRigidDynamic* GetRigidbody(){
			return this->pRigidbody;
		}

		/*!-----------------------------------------------------------
		//	@brief		���͂����Z�b�g
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	ClearForce(){
			this->pRigidbody->clearForce();
		}

		/*!-----------------------------------------------------------
		//	@brief		�g���N�����Z�b�g
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	ClearTorque(){
			this->pRigidbody->clearTorque();
		}

		/*!-----------------------------------------------------------
		//	@brief		���[�U�[�f�[�^�̐ݒ�
		//	@param[in]	_userData	���[�U�[�f�[�^
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		template<class T>
		void	SetUserData(T* _userData){
			this->pRigidbody->userData = static_cast<void*>(_userData);
		}

	private:
		ao::Rigidbody(const ao::Rigidbody&){};
		void operator=(const ao::Rigidbody&){};

	private:
		bool active;						///< �A�N�e�B�u�t���O
		bool useGravity;					///< �d�̗͂L����
		bool isKinematic;					///< �O�͉e���̐ݒ�
		physx::PxRigidDynamic* pRigidbody;	///< ���̃I�u�W�F�N�g�i���I�j
	};
}

#endif // _Include_Rigidbody_hpp_
