/*************************************************************
*	@file   	PhysX.hpp
*	@brief  	PhysX�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/07/08
*************************************************************/

#ifndef _Include_PhysX_hpp_	// �C���N���[�h�K�[�h
#define _Include_PhysX_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<tchar.h>
#include<Windows.h>
#include<mutex>
#include"PhysXHeader.hpp"
#include"Collider.hpp"
#include"PhysXScene.hpp"
#include"PhysXSimulationEventCallback.hpp"
#include"../Define/SystemDefine.hpp"


namespace ao
{
	class PhysXScene;
	class Rigidbody;
	class Collider;
	class BoxCollider;
	class PlaneCollider;
	class SphereCollider;
	class CapsuleCollider;

	/*!-----------------------------------------------------------
	//	@class  PhysX
	//	@brief  PhysX����N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/08
	------------------------------------------------------------*/
	class PhysX
	{
	public:
		~PhysX();

		/*!-----------------------------------------------------------
		//	@brief		�C���X�^���X�擾
		//	@return		���g�̃C���X�^���X
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/08
		------------------------------------------------------------*/
		static PhysX* GetInstance(){
			static PhysX singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		�V�[���̐���
		//  @param[in]  _sceneName	�V�[����[�ȗ���]
		//	@param[in]  _pCallback	�R�[���o�b�N�֐�
		//	@param[in]	_gx			�d�͗v�fX
		//	@param[in]	_gy			�d�͗v�fY
		//	@param[in]	_gz			�d�͗v�fZ
		//	@return		�������ꂽ�V�[���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		PhysXScene*	CreateScene(
			ao::string _sceneName,
			PhysXSimulationEventCallback* _pCallback,
			const float _gx = 0,
			const float _gy = -9.81f,
			const float _gz = 0);

		/*!-----------------------------------------------------------
		//	@brief		�V�[���̍폜
		//	@param[in]	_pScene	�V�[���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/11
		------------------------------------------------------------*/
		void	DeleteScene(PhysXScene* _pScene);

		/*!-----------------------------------------------------------
		//	@brief		�ގ��̍쐬
		//	@param[in]	_staticFriction		�Ö��C�W��(0~1)
		//	@param[in]  _dynamicFriction	�����C�W��(0~1)
		//	@param[in]  _restitution		�����W��(0~1)
		//	@return		�쐬���ꂽ�}�e���A���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		physx::PxMaterial* CreateMaterial(
			float _staticFriction,
			float _dynamicFriction,
			float _restitution);

		/*!-----------------------------------------------------------
		//	@brief		�}�`�쐬
		//	@param[in]	_radius			���a[�ȗ���]
		//	@param[in]	_sFriction		�Ö��C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_dFriction		�����C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_restitution	�����W��(0.0f-1.0f)[�ȗ���]
		//	@return		�쐬���ꂽ�}�`
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/31
		------------------------------------------------------------*/
		BoxCollider* CreateBoxCollider(
			float _halfWidth = 0.5f,
			float _halfHeight = 0.5f,
			float _halfDepth = 0.5f,
			float _sFriction = 1.0f,
			float _dFriction = 1.0f,
			float _restitution = 0.0f);

		/*!-----------------------------------------------------------
		//	@brief		�R���X�g���N�^
		//	@param[in]	_halfWidth		���̔���[�ȗ���]
		//	@param[in]	_halfDepth		���s���̔���[�ȗ���]
		//	@param[in]	_sFriction		�Ö��C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_dFriction		�����C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_restitution	�����W��(0.0f-1.0f)[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/31
		------------------------------------------------------------*/
		PlaneCollider* CreatePlaneCollider(
			float _halfWidth = 0.5f,
			float _halfDepth = 0.5f,
			float _sFriction = 1.0f,
			float _dFriction = 1.0f,
			float _restitution = 0.0f);

		/*!-----------------------------------------------------------
		//	@brief		�}�`�쐬
		//	@param[in]	_radius			���a[�ȗ���]
		//	@param[in]	_sFriction		�Ö��C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_dFriction		�����C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_restitution	�����W��(0.0f-1.0f)[�ȗ���]
		//	@return		�쐬���ꂽ�}�`
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/31
		------------------------------------------------------------*/
		SphereCollider*	CreateSphereCollider(
			float _radius = 0.5f,
			float _sFriction = 1.0f,
			float _dFriction = 1.0f,
			float _restitution = 0.0f);

		/*!-----------------------------------------------------------
		//	@brief		�}�`�쐬
		//	@param[in]	_radius			���a[�ȗ���]
		//	@param[in]	_halfHeight		�����̔���[�ȗ���]
		//	@param[in]	_sFriction		�Ö��C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_dFriction		�����C�W��(0.0f-1.0f)[�ȗ���]
		//	@param[in]	_restitution	�����W��(0.0f-1.0f)[�ȗ���]
		//	@return		�쐬���ꂽ�}�`
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/31
		------------------------------------------------------------*/
		CapsuleCollider* CreateCapsuleCollider(
			float _radius = 0.5f,
			float _halfHeight = 0.5f,
			float _sFriction = 1.0f,
			float _dFriction = 1.0f,
			float _restitution = 0.0f);

		/*!-----------------------------------------------------------
		//	@brief		�ÓI�`����쐬
		//	@param[in]	_pose	�p��
		//	@return		�쐬���ꂽ�ÓI�`��|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		physx::PxRigidStatic* CreateRigidStatic(const physx::PxTransform& _pose);

		/*!-----------------------------------------------------------
		//	@brief		���̂��쐬
		//	@param[in]	_pose	�p��
		//	@return		�쐬���ꂽ���̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		Rigidbody* CreateRigidbody(const physx::PxTransform& _pose);

		/*!-----------------------------------------------------------
		//	@brief		�}�`���쐬
		//	@param[in]	_geometry	�W�I���g��
		//	@param[in]  _material	�}�e���A��
		//	@return		�쐬�����`��|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		physx::PxShape* CreateShape(
			const physx::PxGeometry& _geometry,
			const physx::PxMaterial& _material);

	public:
		/*!-----------------------------------------------------------
		//	@brief		������
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/08
		------------------------------------------------------------*/
		HRESULT		Initialize();

		/*!-----------------------------------------------------------
		//	@brief		�I������
		//	@return		S_OK�F�I�����������@����ȊO�F�I���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/08
		------------------------------------------------------------*/
		HRESULT		Finalize();

	private:
		physx::PxFoundation*			pFoundation;
		physx::PxDefaultAllocator		allocator;
		physx::PxDefaultErrorCallback	errorCallback;
		physx::PxPhysics*				pPhysicsSDK;
		std::list< PhysXScene* >		pSceneList;
		std::list< Rigidbody* >			pRigidbodyList;
		std::list< Collider* >			pColliderList;
		std::mutex	mutex;
#if defined(_DEBUG) || defined(DEBUG)
		physx::PxVisualDebuggerConnection* pPvdConnection;
#endif
	private:
		PhysX();
		PhysX(const PhysX&){};
		void operator=(const PhysX&){};
	};
}

#endif // _Include_PhysX_hpp_
