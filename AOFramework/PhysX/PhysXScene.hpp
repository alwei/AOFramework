/*************************************************************
*	@file   	PhysXScene.hpp
*	@brief  	PhysXSceneクラスヘッダー
*	@note		特になし
*	@author		Tatsunori Aoyama
*	@date		2014/07/09
*************************************************************/

#ifndef _Include_PhysXScene_hpp_	// インクルードガード
#define _Include_PhysXScene_hpp_

//------------------------------------------------------------
// インクルード
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
	//	@brief  物理シーン
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/09
	------------------------------------------------------------*/
	class PhysXScene
	{
	public:
		PhysXScene();
		~PhysXScene();

		/*!-----------------------------------------------------------
		//	@brief		形状に図形をアタッチ
		//	@param[in]	pActor	形状アクター
		//	@param[in]  shape	アタッチする形状
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void AttachShape(physx::PxRigidActor* _pActor, physx::PxShape* _shape);

		/*!-----------------------------------------------------------
		//	@brief		形状から図形を解除
		//	@param[in]	pActor	形状アクター
		//	@param[in]  shape	解除する形状
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void DetachShape(physx::PxRigidActor* _pActor, physx::PxShape* _shape);

		/*!-----------------------------------------------------------
		//	@brief		物体をシーンに追加
		//	@param[in]	actor	追加する物体
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void AddActor(physx::PxActor& _actor);

		/*!-----------------------------------------------------------
		//	@brief		物体をシーンから削除
		//	@param[in]	actor	削除する物体
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void RemoveActor(physx::PxActor& _actor);

		/*!-----------------------------------------------------------
		//	@brief		シーンの全ての物体を削除
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void FlushActor();

	public:
		/*!-----------------------------------------------------------
		//	@brief		情報設定
		//	@param[in]	_sceneName		シーン名
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void	SetSceneName(ao::string _sceneName){
			this->sceneName = _sceneName;
		}

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//  @param[in]  _pPxSdk		PhysXSDKポインタ
		//  @param[in]  _pCallback	コールバッククラス
		//	@param[in]	_gx			重力要素X
		//	@param[in]	_gy			重力要素Y
		//	@param[in]	_gz			重力要素Z
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
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
		//	@brief		終了処理
		//	@return		S_OK：処理成功　それ以外：処理失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		HRESULT Finalize();

		/*!-----------------------------------------------------------
		//	@brief		更新
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/09
		------------------------------------------------------------*/
		void	Simulate();

		/*!-----------------------------------------------------------
		//	@brief		一番近いオブジェクトと衝突判定を行う
		//	@param[in]	_origin			照射座標	
		//	@param[in]	_unitDir		正規化された方向ベクトル
		//	@param[in]	_maxDistance	ベクトルの長さ[省略可]
		//	@param[in]	_pOut			出力する衝突情報[省略可]
		//	@return		TRUE：衝突した　FALSE：衝突していない
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