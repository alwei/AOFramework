/*************************************************************
*	@file   	Collider.hpp
*	@brief  	Colliderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/22
*************************************************************/

#ifndef _Include_Collider_hpp_	// インクルードガード
#define _Include_Collider_hpp_

//------------------------------------------------------------
// インクルード
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
	//	@brief  コライダークラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/22
	------------------------------------------------------------*/
	class Collider
	{
	public:
		Collider(
			PhysX* _pPhysx,						///< フィジックス管理クラス
			const physx::PxGeometry& _geometry,	///< 形状
			float _staticFriction,				///< 静摩擦係数(0.0f~1.0f)
			float _dynamicFriction,				///< 動摩擦係数(0.0f~1.0f)
			float _restitution);				///< 反発係数(0.0f~1.0f)

		~Collider();

		/*!-----------------------------------------------------------
		//	@brief		図形取得
		//	@return		図形の種類
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/22
		------------------------------------------------------------*/
		virtual	ColliderType GetColliderType() const = 0;

		/*!-----------------------------------------------------------
		//	@brief		登録したPhysXの図形取得
		//	@return		登録した図形
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/22
		------------------------------------------------------------*/
		physx::PxShape* GetPxShape(){
			return this->pShape;
		}

		/*!-----------------------------------------------------------
		//	@brief		相対姿勢を設定
		//	@param[in]	_x	X座標
		//	@param[in]	_y	Y座標
		//	@param[in]	_z	Z座標
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/22
		------------------------------------------------------------*/
		void SetLocalPose(float _x, float _y, float _z){
			this->pShape->setLocalPose(physx::PxTransform(physx::PxVec3(_x, _y, _z)));
		}

		/*!-----------------------------------------------------------
		//	@brief		相対姿勢を設定
		//	@param[in]	_xPos	X座標
		//	@param[in]	_yPos	Y座標
		//	@param[in]	_zPos	Z座標
		//	@param[in]	_xRot	X回転量
		//	@param[in]	_yRot	Y回転量
		//	@param[in]	_zRot	Z回転量
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
		//	@brief		トリガー図形の設定
		//	@param[in]	_isEnable	トリガーの有無[省略可]
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
	//	@brief  プレーンコライダークラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/24
	------------------------------------------------------------*/
	class PlaneCollider : public Collider
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		コンストラクタ
		//	@param[in]	_pPhysx			フィジックス管理クラス
		//	@param[in]	_halfWidth		幅の半分[省略可]
		//	@param[in]	_halfDepth		奥行きの半分[省略可]
		//	@param[in]	_sFriction		静摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_dFriction		動摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_restitution	反発係数(0.0f-1.0f)[省略可]
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
		//	@brief		図形取得
		//	@return		図形の種類
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/24
		------------------------------------------------------------*/
		ColliderType GetColliderType() const final {
			return ColliderType::Plane;
		}
	};

	/*!-----------------------------------------------------------
	//	@class  BoxCollider
	//	@brief  ボックスコライダークラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/24
	------------------------------------------------------------*/
	class BoxCollider : public Collider
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		コンストラクタ
		//	@param[in]	_pPhysx			フィジックス管理クラス
		//	@param[in]	_radius			半径[省略可]
		//	@param[in]	_sFriction		静摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_dFriction		動摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_restitution	反発係数(0.0f-1.0f)[省略可]
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
		//	@brief		図形取得
		//	@return		図形の種類
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
	//	@brief  スフィアコライダークラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/24
	------------------------------------------------------------*/
	class SphereCollider : public Collider
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		コンストラクタ
		//	@param[in]	_pPhysx			フィジックス管理クラス
		//	@param[in]	_radius			半径[省略可]
		//	@param[in]	_sFriction		静摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_dFriction		動摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_restitution	反発係数(0.0f-1.0f)[省略可]
		------------------------------------------------------------*/
		SphereCollider(
			PhysX* _pPhysx,
			float _radius = 0.5f,
			float _sFriction = 1.0f,
			float _dFriction = 1.0f,
			float _restitution = 0.0f);

		//----------------------------------------------------------------------
		/**	@brief		図形の種類を取得
		//	@return		図形の種類 */
		//----------------------------------------------------------------------
		ColliderType GetColliderType() const final {
			return ColliderType::Sphere;
		}
	private:
		float radius;
	};

	/*!-----------------------------------------------------------
	//	@class  CapsuleCollider
	//	@brief  カプセルコライダークラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/24
	------------------------------------------------------------*/
	class CapsuleCollider : public Collider
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		コンストラクタ
		//	@param[in]	_pPhysx			フィジックス管理クラス
		//	@param[in]	_radius			半径[省略可]
		//	@param[in]	_halfHeight		高さの半分[省略可]
		//	@param[in]	_sFriction		静摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_dFriction		動摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_restitution	反発係数(0.0f-1.0f)[省略可]
		------------------------------------------------------------*/
		CapsuleCollider(
			PhysX* _pPhysx,
			float _radius = 0.5f,
			float _halfHeight = 0.5f,
			float _sFriction = 1.0f,
			float _dFriction = 1.0f,
			float _restitution = 0.0f);

		//----------------------------------------------------------------------
		/**	@brief		図形の種類を取得
		//	@return		図形の種類 */
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