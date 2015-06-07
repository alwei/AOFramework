/*************************************************************
*	@file   	BaseCamera.hpp
*	@brief  	BaseCameraクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/04/29
*************************************************************/

#ifndef _Include_BaseCamera_hpp_	// インクルードガード
#define _Include_BaseCamera_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<DirectXMath.h>
#include"TaskObject.hpp"
#include"../Math/MathDefine.hpp"
#include"../Math/GameArithmetic.hpp"

using namespace DirectX;


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BaseCamera
	//	@brief  カメラ基底クラス
	//	@author	Tatsunori Aoyama
	//	@date	2015/04/29
	------------------------------------------------------------*/
	class BaseCamera : public TaskObject
	{
		AO_CLASS_DEFINE(TaskObject);
	public:
		/// コンストラクタ
		BaseCamera() : TaskObject(_T("BaseCamera")){};
		BaseCamera(AOLPSTR _name) : TaskObject(_name){};

		/// デストラクタ
		virtual ~BaseCamera()override{};

		/*!-----------------------------------------------------------
		//	@brief		ビューパラメーター初期化
		//  @param[in]	_lookAtDir		注視方向
		//  @param[in]	_position		設定座標
		//  @param[in]  _upDir			上ベクトル
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual	void Initialize(const float3& _lookAtDir,const float3& _position,const float3& _upDir);

		/*!-----------------------------------------------------------
		//	@brief		ビューパラメーター初期化
		//  @param[in]  _poseMtx	姿勢行列
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual	void Initialize(const XMFLOAT4X4& _poseMtx);

		/*!-----------------------------------------------------------
		//	@brief		ビューパラメーター初期化
		//  @param[in]  _transform	姿勢
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual	void Initialize(const Transform& _transform);

		/*!-----------------------------------------------------------
		//	@brief		ビューパラメーター初期化
		//  @param[in]  _position	座標
		//  @param[in]  _rotation	回転
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual	void Initialize(const float3& _position,const float4& _rotation);

		/*!-----------------------------------------------------------
		//	@brief		計算処理
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual	void Update();

		/*!-----------------------------------------------------------
		//	@brief		現在の座標取得
		//	@param[out]	_pOut	出力する視点座標のベクトルポインタ
		//	@return		出力する視点座標のベクトルポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual const float3& GetEyePosition() const
		{
			return this->commitPosition;
		}

		/*!-----------------------------------------------------------
		//	@brief		視点方向ベクトルを取得
		//	@param[out]	_pOut	出力する視点方向ベクトルポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/15
		------------------------------------------------------------*/
		virtual const float3& GetLookAtDirection() const
		{
			return this->commitLookAtDir;
		}

		/*!-----------------------------------------------------------
		//	@brief		ビュー行列の取得
		//	@param[out]	_pOut	出力するビュー行列ポインタ[省略可]
		//	@return		出力するポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual const XMFLOAT4X4& GetViewMatrix() const
		{
			return this->viewMatrix;
		}

	protected:
		float3		commitPosition;
		float3		commitLookAtDir;
		Transform	transform;
		XMFLOAT4X4	viewMatrix;
	};
}

#endif // _Include_BaseCamera_hpp_