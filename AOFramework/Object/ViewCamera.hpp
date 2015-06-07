/*************************************************************
*	@file   	ViewCamera.hpp
*	@brief  	ViewCameraクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/04/15
*************************************************************/

#ifndef _Include_ViewCamera_hpp_	// インクルードガード
#define _Include_ViewCamera_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<DirectXMath.h>
#include"BaseCamera.hpp"
#include"../Input/Mouse.hpp"
#include"../Input/Keyboard.hpp"


using namespace DirectX;


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  ViewCamera
	//	@brief  ビューワーカメラ
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/15
	------------------------------------------------------------*/
	class ViewCamera : public BaseCamera
	{
	public:
		/// コンストラクタ
		ViewCamera();

		/// デストラクタ
		virtual ~ViewCamera()override;

		/*!-----------------------------------------------------------
		//	@brief		ビューパラメーター初期化
		//	@param[in]	vAt			注視点
		//	@param[in]	distance	距離
		//	@param[in]	xRot		X軸回転角度
		//	@param[in]	yRot		Y軸回転角度
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/15
		------------------------------------------------------------*/
		virtual	void	Initialize(const DirectX::XMFLOAT3& _lookAt, float _distance, float _xRot, float _yRot);

		/*!-----------------------------------------------------------
		//	@brief		注視点の移動入力処理
		//	@param[in]  _mouseCode	マウスコード[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/15
		------------------------------------------------------------*/
		virtual	void	InputMoveLookAt(MouseCode::Press _mouseCode = MouseCode::Press::Middle);

		/*!-----------------------------------------------------------
		//	@brief		注視点からカメラの距離調整入力処理
		//	@param[in]	_moveCode	ズームインアウトの入力コード
		//	@author		Tatsunori Aoyama
		//	@date		2014/05/20
		------------------------------------------------------------*/
		virtual	void	InputDistance(MouseCode::Press _moveCode = MouseCode::Press::Right);

		/*!-----------------------------------------------------------
		//	@brief		カメラの衛星回転入力処理
		//	@param[in]  _mouseCode	マウスコード[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/05/20
		------------------------------------------------------------*/
		virtual	void	InputRotation(MouseCode::Press _mouseCode = MouseCode::Press::Left);

		/*!-----------------------------------------------------------
		//	@brief		注視点リセット入力処理
		//	@param[in]  _keyCode	キーコード[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/05/20
		------------------------------------------------------------*/
		virtual	void	InputResetLookAt(const KeyCode& _keyCode = KeyCode::SPACE);

		/*!-----------------------------------------------------------
		//	@brief		入力処理
		//	@note		全てまとめたもの
		//  @param[in]  _mouseLockKey		マウス入力ロックキー
		//	@param[in]  _moveMouseCode		注視点移動入力時のマウスコード
		//	@param[in]  _rotationMouseCode	回転入力時のマウスコード
		//	@param[in]	_zoomMouseCode		ズームインアウトのマウスコード
		//	@param[in]  _resetLookAt		注視点をリセットするキーコード
		//	@author		Tatsunori Aoyama
		//	@date		2014/05/20
		------------------------------------------------------------*/
		virtual	void	Input(
			const KeyCode& _mouseLockKey = KeyCode::LMENU,
			MouseCode::Press _moveMouseCode = MouseCode::Press::Middle,
			MouseCode::Press _rotationMouseCode = MouseCode::Press::Left,
			MouseCode::Press _zoomMouseCode = MouseCode::Press::Right,
			const KeyCode& _resetLookAt = KeyCode::SPACE);

		/*!-----------------------------------------------------------
		//	@brief		計算処理
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/15
		------------------------------------------------------------*/
		virtual	void	Update();

		/*!-----------------------------------------------------------
		//	@brief		注視点座標を設定
		//	@param[in]	_x	X座標
		//	@param[in]	_y	Y座標
		//	@param[in]	_z	Z座標
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/05
		------------------------------------------------------------*/
		virtual void SetLookAtPosition(const float _x, const float _y, const float _z);

		/*!-----------------------------------------------------------
		//	@brief		現在の注視点座標取得
		//	@param[in]	_pOut	出力する注視点座標のベクトルポインタ
		//	@return		出力する注視点座標のベクトルポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/08
		------------------------------------------------------------*/
		virtual inline const float3& GetLookAtPosition()
		{
			return this->lookAt;
		}

		/*!-----------------------------------------------------------
		//	@brief		回転角度取得
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/15
		--------------inline ----------------------------------------------*/
		virtual	inline const float2& GetRotation()
		{
			return this->localRotation;
		}

		/*!-----------------------------------------------------------
		//	@brief		注視点からの距離を算出
		//	@return		注視点からカメラまでの距離
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/15
		------------------------------------------------------------*/
		virtual float	GetLookAtDistance() const 
		{
			return this->localPos.z;
		}

		/*!-----------------------------------------------------------
		//	@brief		ローカル行列取得
		//	@param[out]	_pOut	出力するローカル行列ポインタ
		//	@return		出力するローカル行列ポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/15
		------------------------------------------------------------*/
		virtual const XMFLOAT4X4* GetLocalMatrix(XMFLOAT4X4* _pOut);

	protected:
		FLOAT		wheelZ;
		XMFLOAT4X4	localMatrix;
		XMFLOAT4X4	worldMatrix;
		float2	localRotation;
		float3	localPos;
		float3	lookAt;
	};
}

#endif // _Include_ViewCamera_hpp_