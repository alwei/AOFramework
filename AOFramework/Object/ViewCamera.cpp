#include"stdafx.hpp"
#include"ViewCamera.hpp"
#include"../Math/Math.hpp"


using namespace DirectX;
using namespace ao;


ViewCamera::ViewCamera()
{
	//���ʏ�̈ʒu�x�N�g��������
	this->localPos.x = 0;
	this->localPos.y = 0;
	this->localPos.z = 0;

	//�J�����̎p���s�񏉊���
	XMMATRIX identity = XMMatrixIdentity();
	XMStoreFloat4x4(&this->localMatrix, identity);
	memcpy(this->localMatrix.m[0], &XMFLOAT3(1, 0, 0), sizeof(XMFLOAT3));
	memcpy(this->localMatrix.m[1], &XMFLOAT3(0, 1, 0), sizeof(XMFLOAT3));
	memcpy(this->localMatrix.m[2], &XMFLOAT3(0, 0, 1), sizeof(XMFLOAT3));
	memcpy(this->localMatrix.m[3], &this->localPos, sizeof(XMFLOAT3));

	//�����_���W������
	this->lookAt.x = 0;
	this->lookAt.y = 0;
	this->lookAt.z = 0;

	//�p�x�ۑ�
	this->localRotation.x = 20;
	this->localRotation.y = 10;

	//�z�C�[������������
	this->wheelZ = 0;
}


ViewCamera::~ViewCamera()
{

}


//------------------------------------------------------------------------------
/**	@brief		�J�������������\�b�h */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/12/23	03:04	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void	ViewCamera::Initialize(const XMFLOAT3& _lookAt, float _distance, float _xRot, float _yRot)
{
	this->localPos.z = -_distance;

	this->lookAt.x = _lookAt.x;
	this->lookAt.y = _lookAt.y;
	this->lookAt.z = _lookAt.z;

	this->localRotation.x = _xRot;
	this->localRotation.y = _yRot;


	//Y��]�p�x����
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
/**	@brief		�����_�̈ړ����͏��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/05/20	15:23	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void ViewCamera::InputMoveLookAt(MouseCode::Press _mouseCode)
{
	POINT	relativePos = { 0, 0 };

	ao::Mouse::GetInstance()->GetRelativeCursorPos(&relativePos);

	XMFLOAT2 move;
	move.x = 0 - (float)relativePos.x / 2.0f;
	move.y = 0 + (float)relativePos.y / 2.0f;

	//���N���b�N���̂ݒ����_�ړ�
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
/**	@brief		�����_����J�����̋����������͏��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/05/20	15:23	T.Aoyama	�쐬
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
/**	@brief		�J�����̉q����]���͏��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/05/20	15:23	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void ViewCamera::InputRotation(MouseCode::Press _mouseCode)
{
	POINT	relativePos = { 0, 0 };

	ao::Mouse::GetInstance()->GetRelativeCursorPos(&relativePos);

	XMFLOAT2 move;
	move.x = (float)relativePos.x / 2.0f;
	move.y = (float)relativePos.y / 2.0f;

	//�E�N���b�N���̂݉�]����
	if (ao::Mouse::GetInstance()->GetPressButton(_mouseCode))
	{
		this->localRotation.x += move.x;
		this->localRotation.y += move.y;
	}

	//Y��]�p�x����
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
/**	@brief		�����_���Z�b�g���͏��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/05/20	15:33	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void ViewCamera::InputResetLookAt(const KeyCode& _keyCode)
{
	//SPACE�����Œ����_���Z�b�g
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
/**	@brief		���͏��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/05/20	15:33	T.Aoyama	�쐬
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
/**	@brief		�J�����̃p�����[�^�[�X�V���\�b�h */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2013/12/23	03:04	T.Aoyama	�쐬
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

	//Z���X�V�i��]���Z�o�j
	XMFLOAT3	yRotAxis;
	ao::Vector3Cross(&yRotAxis, worldAxis, *(XMFLOAT3*)this->localMatrix.m[2]);

	//X����]�̍s��쐬
	ao::MatrixRotationY(&xRotMtx, ao::ToRadian(this->localRotation.x));

	//Y��]�s��쐬
	ao::MatrixRotationX(&yRotMtx, ao::ToRadian(this->localRotation.y));

	//��]�s��̍���
	ao::MatrixMultiply(&rotMtx, yRotMtx, xRotMtx);

	//�I�t�Z�b�g�����ʉ�]�s��ŕϊ�
	ao::Vector3TransformCoord(&worldPos, this->localPos.xm, rotMtx);

	//�p���X�V
	XMFLOAT3	xAxis;
	XMFLOAT3	yAxis;
	XMFLOAT3	zAxis(-worldPos.x, -worldPos.y, -worldPos.z);
	ao::Vector3Normalize(&zAxis, zAxis);

	//XY���Z�o
	ao::Vector3Cross(&xAxis, XMFLOAT3(0, 1, 0), zAxis);
	ao::Vector3Cross(&yAxis, zAxis, xAxis);

	//�����s��ɃZ�b�g
	ao::Vector3Normalize(&xAxis, xAxis);
	ao::Vector3Normalize(&yAxis, yAxis);
	ao::Vector3Normalize(&zAxis, zAxis);
	::CopyMemory(this->localMatrix.m[0], &xAxis, sizeof(float) * 3);
	::CopyMemory(this->localMatrix.m[1], &yAxis, sizeof(float) * 3);
	::CopyMemory(this->localMatrix.m[2], &zAxis, sizeof(float) * 3);

	//���s�������iZ)�̃I�t�Z�b�g�X�V
	if (this->localPos.z + this->wheelZ < 0)
		this->localPos.z += this->wheelZ;


	//�r���[�I�t�Z�b�g�X�V�i�����_�ɘA���j
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

	//�r���[�s��ɕϊ��i�t�s�񉻁j
	ao::MatrixInverse(&this->viewMatrix, this->worldMatrix);
}


//------------------------------------------------------------------------------
/**	@brief		�����_���W��ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/11/05	17:59	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void ViewCamera::SetLookAtPosition(const float _x,const float _y,const float _z)
{
	this->lookAt.x = _x;
	this->lookAt.y = _y;
	this->lookAt.z = _z;
}


//------------------------------------------------------------------------------
/**	@brief		���[�J���s��擾 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/08/15	02:49	T.Aoyama	�쐬
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