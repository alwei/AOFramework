#include"stdafx.hpp"
#include"ViewCamera.hpp"
#include"../Math/Math.hpp"


using namespace DirectX;
using namespace ao;


ViewCamera::ViewCamera()
{
	//球面上の位置ベクトル初期化
	this->localPos.x = 0;
	this->localPos.y = 0;
	this->localPos.z = 0;

	//カメラの姿勢行列初期化
	XMMATRIX identity = XMMatrixIdentity();
	XMStoreFloat4x4(&this->localMatrix, identity);
	memcpy(this->localMatrix.m[0], &XMFLOAT3(1, 0, 0), sizeof(XMFLOAT3));
	memcpy(this->localMatrix.m[1], &XMFLOAT3(0, 1, 0), sizeof(XMFLOAT3));
	memcpy(this->localMatrix.m[2], &XMFLOAT3(0, 0, 1), sizeof(XMFLOAT3));
	memcpy(this->localMatrix.m[3], &this->localPos, sizeof(XMFLOAT3));

	//注視点座標初期化
	this->lookAt.x = 0;
	this->lookAt.y = 0;
	this->lookAt.z = 0;

	//角度保存
	this->localRotation.x = 20;
	this->localRotation.y = 10;

	//ホイール成分初期化
	this->wheelZ = 0;
}


ViewCamera::~ViewCamera()
{

}


//------------------------------------------------------------------------------
/**	@brief		カメラ初期化メソッド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/12/23	03:04	T.Aoyama	作成
//------------------------------------------------------------------------------
void	ViewCamera::Initialize(const XMFLOAT3& _lookAt, float _distance, float _xRot, float _yRot)
{
	this->localPos.z = -_distance;

	this->lookAt.x = _lookAt.x;
	this->lookAt.y = _lookAt.y;
	this->lookAt.z = _lookAt.z;

	this->localRotation.x = _xRot;
	this->localRotation.y = _yRot;


	//Y回転角度制限
	if (this->localRotation.y >= 90)
	{
		this->localRotation.y = 89.99f;
	}
	else if (this->localRotation.y <= -90)
	{
		this->localRotation.y = -89.99f;
	}

	this->Update();
}


//------------------------------------------------------------------------------
/**	@brief		注視点の移動入力処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/05/20	15:23	T.Aoyama	作成
//------------------------------------------------------------------------------
void ViewCamera::InputMoveLookAt(MouseCode::Press _mouseCode)
{
	POINT	relativePos = { 0, 0 };

	ao::Mouse::GetInstance()->GetRelativeCursorPos(&relativePos);

	XMFLOAT2 move;
	move.x = 0 - (float)relativePos.x / 2.0f;
	move.y = 0 + (float)relativePos.y / 2.0f;

	//左クリック時のみ注視点移動
	if (ao::Mouse::GetInstance()->GetPressButton(_mouseCode))
	{
		XMFLOAT3 atMove(move.x, move.y, 0);
		ao::Vector3TransformNormal(&atMove, atMove, this->localMatrix);

		this->lookAt.x += atMove.x / 10.0f;
		this->lookAt.y += atMove.y / 10.0f;
		this->lookAt.z += atMove.z / 10.0f;
	}
}


//------------------------------------------------------------------------------
/**	@brief		注視点からカメラの距離調整入力処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/05/20	15:23	T.Aoyama	作成
//------------------------------------------------------------------------------
void ViewCamera::InputDistance(MouseCode::Press _moveCode)
{
	if (!ao::Mouse::GetInstance()->GetPressButton(_moveCode))
	{
		this->wheelZ = 0;
		return;
	}

	POINT pos;
	ao::Mouse::GetInstance()->GetRelativeCursorPos(&pos);
	this->wheelZ = (float)pos.x;
}


//------------------------------------------------------------------------------
/**	@brief		カメラの衛星回転入力処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/05/20	15:23	T.Aoyama	作成
//------------------------------------------------------------------------------
void ViewCamera::InputRotation(MouseCode::Press _mouseCode)
{
	POINT	relativePos = { 0, 0 };

	ao::Mouse::GetInstance()->GetRelativeCursorPos(&relativePos);

	XMFLOAT2 move;
	move.x = (float)relativePos.x / 2.0f;
	move.y = (float)relativePos.y / 2.0f;

	//右クリック時のみ回転処理
	if (ao::Mouse::GetInstance()->GetPressButton(_mouseCode))
	{
		this->localRotation.x += move.x;
		this->localRotation.y += move.y;
	}

	//Y回転角度制限
	if (this->localRotation.y >= 90)
	{
		this->localRotation.y = 89.99f;
	}
	else if (this->localRotation.y <= -90)
	{
		this->localRotation.y = -89.99f;
	}
}


//------------------------------------------------------------------------------
/**	@brief		注視点リセット入力処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/05/20	15:33	T.Aoyama	作成
//------------------------------------------------------------------------------
void ViewCamera::InputResetLookAt(const KeyCode& _keyCode)
{
	//SPACE押下で注視点リセット
	if (ao::Keyboard::GetInstance()->GetTrgKey(_keyCode))
	{
		this->lookAt.x = 0;
		this->lookAt.y = 0;
		this->lookAt.z = 0;
	}

	if (ao::Keyboard::GetInstance()->GetTrgKey(KeyCode::F1))
	{
		this->localRotation.x = -90.0f;
		this->localRotation.y = 0;
		this->lookAt.x = 0;
		this->lookAt.y = 0;
		this->lookAt.z = 0;
	}

	if (ao::Keyboard::GetInstance()->GetTrgKey(KeyCode::F2))
	{
		this->localRotation.x = 0;
		this->localRotation.y = 89.99f;
		this->lookAt.x = 0;
		this->lookAt.y = 0;
		this->lookAt.z = 0;
	}

	if (ao::Keyboard::GetInstance()->GetTrgKey(KeyCode::F3))
	{
		this->localRotation.x = 180;
		this->localRotation.y = 0;
		this->lookAt.x = 0;
		this->lookAt.y = 0;
		this->lookAt.z = 0;
	}
}


//------------------------------------------------------------------------------
/**	@brief		入力処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/05/20	15:33	T.Aoyama	作成
//------------------------------------------------------------------------------
void ViewCamera::Input(
	const KeyCode& _mouseLockKey,
	MouseCode::Press _moveMouseCode,
	MouseCode::Press _rotationMouseCode,
	MouseCode::Press _zoomMouseCode,
	const KeyCode& _resetLookAt)
{
	this->InputMoveLookAt(_moveMouseCode);

	if (!ao::Keyboard::GetInstance()->GetPrsKey(_mouseLockKey))
	{
		return;
	}

	this->InputDistance(_zoomMouseCode);
	this->InputRotation(_rotationMouseCode);
	this->InputResetLookAt(_resetLookAt);
}


//------------------------------------------------------------------------------
/**	@brief		カメラのパラメーター更新メソッド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/12/23	03:04	T.Aoyama	作成
//------------------------------------------------------------------------------
void	ViewCamera::Update()
{
	XMFLOAT4X4	xRotMtx;
	XMFLOAT4X4	yRotMtx;
	XMFLOAT4X4	rotMtx;
	XMFLOAT3	worldAxis;
	XMFLOAT3	worldPos;

	ao::MatrixIdentity(&xRotMtx);
	ao::MatrixIdentity(&yRotMtx);
	ao::Vector3TransformNormal(&worldAxis, XMFLOAT3(1, 0, 0), this->localMatrix);

	//Z仮更新（回転軸算出）
	XMFLOAT3	yRotAxis;
	ao::Vector3Cross(&yRotAxis, worldAxis, *(XMFLOAT3*)this->localMatrix.m[2]);

	//X軸回転の行列作成
	ao::MatrixRotationY(&xRotMtx, ao::ToRadian(this->localRotation.x));

	//Y回転行列作成
	ao::MatrixRotationX(&yRotMtx, ao::ToRadian(this->localRotation.y));

	//回転行列の合成
	ao::MatrixMultiply(&rotMtx, yRotMtx, xRotMtx);

	//オフセットを球面回転行列で変換
	ao::Vector3TransformCoord(&worldPos, this->localPos.xm, rotMtx);

	//姿勢更新
	XMFLOAT3	xAxis;
	XMFLOAT3	yAxis;
	XMFLOAT3	zAxis(-worldPos.x, -worldPos.y, -worldPos.z);
	ao::Vector3Normalize(&zAxis, zAxis);

	//XY軸算出
	ao::Vector3Cross(&xAxis, XMFLOAT3(0, 1, 0), zAxis);
	ao::Vector3Cross(&yAxis, zAxis, xAxis);

	//軸を行列にセット
	ao::Vector3Normalize(&xAxis, xAxis);
	ao::Vector3Normalize(&yAxis, yAxis);
	ao::Vector3Normalize(&zAxis, zAxis);
	::CopyMemory(this->localMatrix.m[0], &xAxis, sizeof(float) * 3);
	::CopyMemory(this->localMatrix.m[1], &yAxis, sizeof(float) * 3);
	::CopyMemory(this->localMatrix.m[2], &zAxis, sizeof(float) * 3);

	//奥行き方向（Z)のオフセット更新
	if (this->localPos.z + this->wheelZ < 0)
		this->localPos.z += this->wheelZ;


	//ビューオフセット更新（注視点に連動）
	XMFLOAT3 worldTransPos(0, 0, 0);
	worldTransPos.x = worldPos.x + this->lookAt.x;
	worldTransPos.y = worldPos.y + this->lookAt.y;
	worldTransPos.z = worldPos.z + this->lookAt.z;
	::CopyMemory(&this->worldMatrix, &this->localMatrix, sizeof(float) * 12);
	::CopyMemory(this->worldMatrix.m[3], &worldTransPos, sizeof(XMFLOAT3));
	this->worldMatrix._44 = 1.0f;

	this->transform = this->worldMatrix;
	this->commitPosition = worldTransPos;
	Vector3TransformNormal(&this->commitLookAtDir.xm, float3::foward.xm, rotMtx);

	//ビュー行列に変換（逆行列化）
	ao::MatrixInverse(&this->viewMatrix, this->worldMatrix);
}


//------------------------------------------------------------------------------
/**	@brief		注視点座標を設定 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/11/05	17:59	T.Aoyama	作成
//------------------------------------------------------------------------------
void ViewCamera::SetLookAtPosition(const float _x,const float _y,const float _z)
{
	this->lookAt.x = _x;
	this->lookAt.y = _y;
	this->lookAt.z = _z;
}


//------------------------------------------------------------------------------
/**	@brief		ローカル行列取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/15	02:49	T.Aoyama	作成
//------------------------------------------------------------------------------
const XMFLOAT4X4* ViewCamera::GetLocalMatrix(XMFLOAT4X4* _pOut)
{
#ifdef __AO_DEBUG__
	if (_pOut == nullptr)
	{
		return nullptr;
	}
#endif

	::CopyMemory(_pOut, &this->localMatrix, sizeof(XMFLOAT4X4));

	return _pOut;
}