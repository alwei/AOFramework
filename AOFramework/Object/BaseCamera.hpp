/*************************************************************
*	@file   	BaseCamera.hpp
*	@brief  	BaseCamera�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/04/29
*************************************************************/

#ifndef _Include_BaseCamera_hpp_	// �C���N���[�h�K�[�h
#define _Include_BaseCamera_hpp_

//------------------------------------------------------------
// �C���N���[�h
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
	//	@brief  �J�������N���X
	//	@author	Tatsunori Aoyama
	//	@date	2015/04/29
	------------------------------------------------------------*/
	class BaseCamera : public TaskObject
	{
		AO_CLASS_DEFINE(TaskObject);
	public:
		/// �R���X�g���N�^
		BaseCamera() : TaskObject(_T("BaseCamera")){};
		BaseCamera(AOLPSTR _name) : TaskObject(_name){};

		/// �f�X�g���N�^
		virtual ~BaseCamera()override{};

		/*!-----------------------------------------------------------
		//	@brief		�r���[�p�����[�^�[������
		//  @param[in]	_lookAtDir		��������
		//  @param[in]	_position		�ݒ���W
		//  @param[in]  _upDir			��x�N�g��
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual	void Initialize(const float3& _lookAtDir,const float3& _position,const float3& _upDir);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�p�����[�^�[������
		//  @param[in]  _poseMtx	�p���s��
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual	void Initialize(const XMFLOAT4X4& _poseMtx);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�p�����[�^�[������
		//  @param[in]  _transform	�p��
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual	void Initialize(const Transform& _transform);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�p�����[�^�[������
		//  @param[in]  _position	���W
		//  @param[in]  _rotation	��]
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual	void Initialize(const float3& _position,const float4& _rotation);

		/*!-----------------------------------------------------------
		//	@brief		�v�Z����
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual	void Update();

		/*!-----------------------------------------------------------
		//	@brief		���݂̍��W�擾
		//	@param[out]	_pOut	�o�͂��鎋�_���W�̃x�N�g���|�C���^
		//	@return		�o�͂��鎋�_���W�̃x�N�g���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/29
		------------------------------------------------------------*/
		virtual const float3& GetEyePosition() const
		{
			return this->commitPosition;
		}

		/*!-----------------------------------------------------------
		//	@brief		���_�����x�N�g�����擾
		//	@param[out]	_pOut	�o�͂��鎋�_�����x�N�g���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/15
		------------------------------------------------------------*/
		virtual const float3& GetLookAtDirection() const
		{
			return this->commitLookAtDir;
		}

		/*!-----------------------------------------------------------
		//	@brief		�r���[�s��̎擾
		//	@param[out]	_pOut	�o�͂���r���[�s��|�C���^[�ȗ���]
		//	@return		�o�͂���|�C���^
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