/*************************************************************
*	@file   	Rigidbody.hpp
*	@brief  	Rigidbodyクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/07/10
*************************************************************/

#ifndef _Include_Rigidbody_hpp_	// インクルードガード
#define _Include_Rigidbody_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"PhysXHeader.hpp"
#include"PhysX.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  Rigidbody
	//	@brief  剛体クラス
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
		//	@brief		アクティブ設定
		//	@param[in]	_isActive	有効化設定
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	SetActive(bool _isActive = true){
			this->active = _isActive;
			this->pRigidbody->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !_isActive);
		}

		/*!-----------------------------------------------------------
		//	@brief		アクティブ状態の取得
		//	@return		true：有効　false：無効
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		bool	IsActive(){
			return this->active;
		}

		/*!-----------------------------------------------------------
		//	@brief		重力の設定
		//	@param[in]	_isUseGravity	重力の設定
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	EnableGravity(bool _useGravity = true){
			this->useGravity = _useGravity;
			this->pRigidbody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !_useGravity);
		}

		/*!-----------------------------------------------------------
		//	@brief		重力の有効状態取得
		//	@return		true：重力有効　false：重力無効
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		bool	IsUseGravity(){
			return this->useGravity;
		}

		/*!-----------------------------------------------------------
		//	@brief		外力の影響設定
		//	@param[in]	_isKinematic	外力の影響設定（true:受けない / false:受ける）
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	EnableKinematic(bool _isKinematic = false){
			this->isKinematic = _isKinematic;
			this->pRigidbody->setRigidBodyFlag(physx::PxRigidDynamicFlag::eKINEMATIC, _isKinematic);
		}

		/*!-----------------------------------------------------------
		//	@brief		外力影響の取得
		//	@return		true：影響を受けない　false：影響を受ける
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		bool	IsKinematic(){
			return this->isKinematic;
		}

		/*!-----------------------------------------------------------
		//	@brief		速度を設定
		//	@param[in]	_x	速度X
		//	@param[in]  _y	速度Y
		//	@param[in]  _z	速度Z
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	SetLinearVelocity(float _x, float _y, float _z){
			this->pRigidbody->setLinearVelocity(physx::PxVec3(_x, _y, _z));
		}

		/*!-----------------------------------------------------------
		//	@brief		速度を取得
		//	@param[out]	pVelocity	出力するベクトルポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	GetLinearVelocity(physx::PxVec3* pVelocity){
			(*pVelocity) = this->pRigidbody->getLinearVelocity();
		}

		/*!-----------------------------------------------------------
		//	@brief		回転速度を設定
		//	@param[in]	_x	回転速度X
		//	@param[in]  _y	回転速度Y
		//	@param[in]  _z	回転速度Z
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	SetAngularVelocity(float _x, float _y, float _z){
			this->pRigidbody->setAngularVelocity(physx::PxVec3(_x, _y, _z));
		}

		/*!-----------------------------------------------------------
		//	@brief		回転速度を取得
		//	@param[out]	pAngularVelocity	出力する回転速度ベクトルポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	GetAngularVelocity(physx::PxVec3* pAngularVelocity){
			(*pAngularVelocity) = this->pRigidbody->getAngularVelocity();
		}

		/*!-----------------------------------------------------------
		//	@brief		姿勢を設定
		//	@param[in]	_xPos	X座標
		//	@param[in]  _yPos	Y座標
		//	@param[in]  _zPos	Z座標
		//	@param[in]	_xQtn	クォータニオン要素X
		//	@param[in]	_yQtn	クォータニオン要素Y
		//	@param[in]	_zQtn	クォータニオン要素Z
		//	@param[in]	_wQtn	クォータニオン要素W
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
		//	@brief		姿勢を設定
		//  @param[in]  _pose	姿勢
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	SetTransform(physx::PxTransform& _pose){
			this->pRigidbody->setGlobalPose(_pose);
		}

		/*!-----------------------------------------------------------
		//	@brief		姿勢を取得
		//	@param[out]	pTransfrom	出力する姿勢
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	GetTransform(physx::PxTransform* pTransform){
			(*pTransform) = this->pRigidbody->getGlobalPose();
		}

		/*!-----------------------------------------------------------
		//	@brief		合力を加算
		//	@param[in]	_x	X要素
		//	@param[in]  _y	Y要素
		//	@param[in]  _z	Z要素
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	AddForce(float _x, float _y, float _z){
			this->pRigidbody->addForce(physx::PxVec3(_x, _y, _z));
		}

		/*!-----------------------------------------------------------
		//	@brief		トルクを加算
		//	@param[in]	_x	X要素
		//	@param[in]  _y	Y要素
		//	@param[in]  _z	Z要素
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	AddTorque(float _x, float _y, float _z){
			this->pRigidbody->addTorque(physx::PxVec3(_x, _y, _z));
		}


		/*!-----------------------------------------------------------
		//	@brief		剛体取得
		//  @return     動的剛体取得
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/01
		------------------------------------------------------------*/
		physx::PxRigidDynamic* GetRigidbody(){
			return this->pRigidbody;
		}

		/*!-----------------------------------------------------------
		//	@brief		合力をリセット
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	ClearForce(){
			this->pRigidbody->clearForce();
		}

		/*!-----------------------------------------------------------
		//	@brief		トルクをリセット
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/10
		------------------------------------------------------------*/
		void	ClearTorque(){
			this->pRigidbody->clearTorque();
		}

		/*!-----------------------------------------------------------
		//	@brief		ユーザーデータの設定
		//	@param[in]	_userData	ユーザーデータ
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
		bool active;						///< アクティブフラグ
		bool useGravity;					///< 重力の有効化
		bool isKinematic;					///< 外力影響の設定
		physx::PxRigidDynamic* pRigidbody;	///< 剛体オブジェクト（動的）
	};
}

#endif // _Include_Rigidbody_hpp_
