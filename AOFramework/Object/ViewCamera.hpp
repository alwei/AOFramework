/*************************************************************
*	@file   	ViewCamera.hpp
*	@brief  	ViewCamera�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/04/15
*************************************************************/

#ifndef _Include_ViewCamera_hpp_	// �C���N���[�h�K�[�h
#define _Include_ViewCamera_hpp_

//------------------------------------------------------------
// �C���N���[�h
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
	//	@brief  �r���[���[�J����
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/15
	------------------------------------------------------------*/
	class ViewCamera : public BaseCamera
	{
	public:
		/// �R���X�g���N�^
		ViewCamera();

		/// �f�X�g���N�^
		virtual ~ViewCamera()override;

		/*!-----------------------------------------------------------
		//	@brief		�r���[�p�����[�^�[������
		//	@param[in]	vAt			�����_
		//	@param[in]	distance	����
		//	@param[in]	xRot		X����]�p�x
		//	@param[in]	yRot		Y����]�p�x
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/15
		------------------------------------------------------------*/
		virtual	void	Initialize(const DirectX::XMFLOAT3& _lookAt, float _distance, float _xRot, float _yRot);

		/*!-----------------------------------------------------------
		//	@brief		�����_�̈ړ����͏���
		//	@param[in]  _mouseCode	�}�E�X�R�[�h[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/15
		------------------------------------------------------------*/
		virtual	void	InputMoveLookAt(MouseCode::Press _mouseCode = MouseCode::Press::Middle);

		/*!-----------------------------------------------------------
		//	@brief		�����_����J�����̋����������͏���
		//	@param[in]	_moveCode	�Y�[���C���A�E�g�̓��̓R�[�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/05/20
		------------------------------------------------------------*/
		virtual	void	InputDistance(MouseCode::Press _moveCode = MouseCode::Press::Right);

		/*!-----------------------------------------------------------
		//	@brief		�J�����̉q����]���͏���
		//	@param[in]  _mouseCode	�}�E�X�R�[�h[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/05/20
		------------------------------------------------------------*/
		virtual	void	InputRotation(MouseCode::Press _mouseCode = MouseCode::Press::Left);

		/*!-----------------------------------------------------------
		//	@brief		�����_���Z�b�g���͏���
		//	@param[in]  _keyCode	�L�[�R�[�h[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/05/20
		------------------------------------------------------------*/
		virtual	void	InputResetLookAt(const KeyCode& _keyCode = KeyCode::SPACE);

		/*!-----------------------------------------------------------
		//	@brief		���͏���
		//	@note		�S�Ă܂Ƃ߂�����
		//  @param[in]  _mouseLockKey		�}�E�X���̓��b�N�L�[
		//	@param[in]  _moveMouseCode		�����_�ړ����͎��̃}�E�X�R�[�h
		//	@param[in]  _rotationMouseCode	��]���͎��̃}�E�X�R�[�h
		//	@param[in]	_zoomMouseCode		�Y�[���C���A�E�g�̃}�E�X�R�[�h
		//	@param[in]  _resetLookAt		�����_�����Z�b�g����L�[�R�[�h
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
		//	@brief		�v�Z����
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/15
		------------------------------------------------------------*/
		virtual	void	Update();

		/*!-----------------------------------------------------------
		//	@brief		�����_���W��ݒ�
		//	@param[in]	_x	X���W
		//	@param[in]	_y	Y���W
		//	@param[in]	_z	Z���W
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/05
		------------------------------------------------------------*/
		virtual void SetLookAtPosition(const float _x, const float _y, const float _z);

		/*!-----------------------------------------------------------
		//	@brief		���݂̒����_���W�擾
		//	@param[in]	_pOut	�o�͂��钍���_���W�̃x�N�g���|�C���^
		//	@return		�o�͂��钍���_���W�̃x�N�g���|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/08
		------------------------------------------------------------*/
		virtual inline const float3& GetLookAtPosition()
		{
			return this->lookAt;
		}

		/*!-----------------------------------------------------------
		//	@brief		��]�p�x�擾
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/15
		--------------inline ----------------------------------------------*/
		virtual	inline const float2& GetRotation()
		{
			return this->localRotation;
		}

		/*!-----------------------------------------------------------
		//	@brief		�����_����̋������Z�o
		//	@return		�����_����J�����܂ł̋���
		//	@author		Tatsunori Aoyama
		//	@date		2014/04/15
		------------------------------------------------------------*/
		virtual float	GetLookAtDistance() const 
		{
			return this->localPos.z;
		}

		/*!-----------------------------------------------------------
		//	@brief		���[�J���s��擾
		//	@param[out]	_pOut	�o�͂��郍�[�J���s��|�C���^
		//	@return		�o�͂��郍�[�J���s��|�C���^
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