/*************************************************************
*	@file   	PhysX.hpp
*	@brief  	PhysXクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/07/08
*************************************************************/

#ifndef _Include_PhysX_hpp_	// インクルードガード
#define _Include_PhysX_hpp_

//------------------------------------------------------------
// インクルード
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
	//	@brief  PhysX制御クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/08
	------------------------------------------------------------*/
	class PhysX
	{
	public:
		~PhysX();

		/*!-----------------------------------------------------------
		//	@brief		インスタンス取得
		//	@return		自身のインスタンス
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/08
		------------------------------------------------------------*/
		static PhysX* GetInstance(){
			static PhysX singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		シーンの生成
		//  @param[in]  _sceneName	シーン名[省略可]
		//	@param[in]  _pCallback	コールバック関数
		//	@param[in]	_gx			重力要素X
		//	@param[in]	_gy			重力要素Y
		//	@param[in]	_gz			重力要素Z
		//	@return		生成されたシーンポインタ
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
		//	@brief		シーンの削除
		//	@param[in]	_pScene	シーンポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/11
		------------------------------------------------------------*/
		void	DeleteScene(PhysXScene* _pScene);

		/*!-----------------------------------------------------------
		//	@brief		材質の作成
		//	@param[in]	_staticFriction		静摩擦係数(0~1)
		//	@param[in]  _dynamicFriction	動摩擦係数(0~1)
		//	@param[in]  _restitution		反発係数(0~1)
		//	@return		作成されたマテリアルポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		physx::PxMaterial* CreateMaterial(
			float _staticFriction,
			float _dynamicFriction,
			float _restitution);

		/*!-----------------------------------------------------------
		//	@brief		図形作成
		//	@param[in]	_radius			半径[省略可]
		//	@param[in]	_sFriction		静摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_dFriction		動摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_restitution	反発係数(0.0f-1.0f)[省略可]
		//	@return		作成された図形
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
		//	@brief		コンストラクタ
		//	@param[in]	_halfWidth		幅の半分[省略可]
		//	@param[in]	_halfDepth		奥行きの半分[省略可]
		//	@param[in]	_sFriction		静摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_dFriction		動摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_restitution	反発係数(0.0f-1.0f)[省略可]
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
		//	@brief		図形作成
		//	@param[in]	_radius			半径[省略可]
		//	@param[in]	_sFriction		静摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_dFriction		動摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_restitution	反発係数(0.0f-1.0f)[省略可]
		//	@return		作成された図形
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/31
		------------------------------------------------------------*/
		SphereCollider*	CreateSphereCollider(
			float _radius = 0.5f,
			float _sFriction = 1.0f,
			float _dFriction = 1.0f,
			float _restitution = 0.0f);

		/*!-----------------------------------------------------------
		//	@brief		図形作成
		//	@param[in]	_radius			半径[省略可]
		//	@param[in]	_halfHeight		高さの半分[省略可]
		//	@param[in]	_sFriction		静摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_dFriction		動摩擦係数(0.0f-1.0f)[省略可]
		//	@param[in]	_restitution	反発係数(0.0f-1.0f)[省略可]
		//	@return		作成された図形
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
		//	@brief		静的形状を作成
		//	@param[in]	_pose	姿勢
		//	@return		作成された静的形状ポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		physx::PxRigidStatic* CreateRigidStatic(const physx::PxTransform& _pose);

		/*!-----------------------------------------------------------
		//	@brief		剛体を作成
		//	@param[in]	_pose	姿勢
		//	@return		作成された剛体ポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		Rigidbody* CreateRigidbody(const physx::PxTransform& _pose);

		/*!-----------------------------------------------------------
		//	@brief		図形を作成
		//	@param[in]	_geometry	ジオメトリ
		//	@param[in]  _material	マテリアル
		//	@return		作成した形状ポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		physx::PxShape* CreateShape(
			const physx::PxGeometry& _geometry,
			const physx::PxMaterial& _material);

	public:
		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/08
		------------------------------------------------------------*/
		HRESULT		Initialize();

		/*!-----------------------------------------------------------
		//	@brief		終了処理
		//	@return		S_OK：終了処理成功　それ以外：終了処理失敗
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
