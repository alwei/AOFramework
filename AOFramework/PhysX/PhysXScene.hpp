/*************************************************************
*	@file   	PhysXScene.hpp
*	@brief  	PhysXScene�N���X�w�b�_�[
*	@note		���ɂȂ�
*	@author		Tatsunori Aoyama
*	@date		2014/07/09
*************************************************************/

#ifndef _Include_PhysXScene_hpp_	// �C���N���[�h�K�[�h
#define _Include_PhysXScene_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<tchar.h>
#include<Windows.h>
#include<mutex>
#include<list>
#include"../Define/SystemDefine.hpp"
#include"PhysXSimulationEventCallback.hpp"
#include"PhysXHeader.hpp"
#include"Collider.hpp"
#include"../Math/MathDefine.hpp"
#include"../Math/GameArithmetic.hpp"


namespace ao
{
	struct	RaycastHit{
		float3 point;
		float distance;
		void* userData;
		RaycastHit() : distance(0),userData(nullptr){}
		
		template<class T>
		T* GetUserData(){
			return macro::pointer_cast<T*>(this->userData);
		}
	};

	/*!-----------------------------------------------------------
	//	@class  PhysXScene
	//	@brief  �����V�[��
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/09
	------------------------------------------------------------*/
	class PhysXScene
	{
	public:
		PhysXScene();
		~PhysXScene();

		/*!-----------------------------------------------------------
		//	@brief		�`��ɐ}�`���A�^�b�`
		//	@param[in]	pActor	�`��A�N�^�[
		//	@param[in]  shape	�A�^�b�`����`��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void AttachShape(physx::PxRigidActor* _pActor, physx::PxShape* _shape);

		/*!-----------------------------------------------------------
		//	@brief		�`�󂩂�}�`������
		//	@param[in]	pActor	�`��A�N�^�[
		//	@param[in]  shape	��������`��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void DetachShape(physx::PxRigidActor* _pActor, physx::PxShape* _shape);

		/*!-----------------------------------------------------------
		//	@brief		���̂��V�[���ɒǉ�
		//	@param[in]	actor	�ǉ����镨��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void AddActor(physx::PxActor& _actor);

		/*!-----------------------------------------------------------
		//	@brief		���̂��V�[������폜
		//	@param[in]	actor	�폜���镨��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void RemoveActor(physx::PxActor& _actor);

		/*!-----------------------------------------------------------
		//	@brief		�V�[���̑S�Ă̕��̂��폜
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void FlushActor();

	public:
		/*!-----------------------------------------------------------
		//	@brief		���ݒ�
		//	@param[in]	_sceneName		�V�[����
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void	SetSceneName(ao::string _sceneName){
			this->sceneName = _sceneName;
		}

		/*!-----------------------------------------------------------
		//	@brief		������
		//  @param[in]  _pPxSdk		PhysXSDK�|�C���^
		//  @param[in]  _pCallback	�R�[���o�b�N�N���X
		//	@param[in]	_gx			�d�͗v�fX
		//	@param[in]	_gy			�d�͗v�fY
		//	@param[in]	_gz			�d�͗v�fZ
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		HRESULT Initialize(
			physx::PxPhysics* _pPxSdk,
			PhysXSimulationEventCallback* _pCallback,
			const float _gx = 0,
			const float _gy = -9.81f,
			const float _gz = 0);

		/*!-----------------------------------------------------------
		//	@brief		�I������
		//	@return		S_OK�F���������@����ȊO�F�������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		HRESULT Finalize();

		/*!-----------------------------------------------------------
		//	@brief		�X�V
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void	Simulate();

		/*!-----------------------------------------------------------
		//	@brief		��ԋ߂��I�u�W�F�N�g�ƏՓ˔�����s��
		//	@param[in]	_origin			�Ǝˍ��W	
		//	@param[in]	_unitDir		���K�����ꂽ�����x�N�g��
		//	@param[in]	_maxDistance	�x�N�g���̒���[�ȗ���]
		//	@param[in]	_pOut			�o�͂���Փˏ��[�ȗ���]
		//	@return		TRUE�F�Փ˂����@FALSE�F�Փ˂��Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/08
		------------------------------------------------------------*/
		BOOL	Raycast(
			const XMFLOAT3& _origin,
			const XMFLOAT3& _unitDir,
			float _maxDistance = 1,
			RaycastHit* _pOut=nullptr);

	private:
		std::list<physx::PxActor*> pActorList;
		physx::PxDefaultCpuDispatcher*	pCpuDispatcher;
		physx::PxScene*					pPhysicsScene;
		ao::string		sceneName;
		std::mutex		mutex;
	};
}

#endif // _Include_PhysXScene_hpp_