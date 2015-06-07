/*************************************************************
*	@file   	iUiObject.hpp
*	@brief  	iUiObject�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/03/29
*************************************************************/

#ifndef _Include_iUiObject_hpp_	// �C���N���[�h�K�[�h
#define _Include_iUiObject_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"../Object/GameObject.hpp"
#include"../Math/MathDefine.hpp"
#include"../Api/DirectX11Resource.hpp"


namespace ao
{
	class float2;
	class float4;
	struct UiInstance;

	/*!-----------------------------------------------------------
	//	@class  iUiObject
	//	@brief  UI�̃C���X�^���X�����_�����O�p�I�u�W�F�N�g
	//	@author	Tatsunori Aoyama
	//	@date	2015/03/29
	------------------------------------------------------------*/
	class iUiObject : public GameObject
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		�t�@�N�g�����琶������
		//	@param[in]	_pUiInstance	UI�C���X�^���X
		//	@param[in]	_leftTop		�����UV���W
		//	@param[in]	_rightTop		�E���UV���W
		//	@param[in]	_leftBottom		������UV���W
		//	@param[in]	_rightBottom	�E����UV���W
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		iUiObject(
			ao::UiInstance* _pInstance,
			const ao::float2& _leftTop,
			const ao::float2& _rightTop,
			const ao::float2& _leftBottom,
			const ao::float2& _rightBottom);

		~iUiObject();

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_x		X���W
		//  @param[in]  _y		Y���W
		//  @param[in]  _w		����
		//  @param[in]  _h		�c��
		//	@param[in]	_degree	��]�p�x(�f�O���[�l)[�ȗ���]
		//	@param[in]	_r		�ԐF[�ȗ���]
		//	@param[in]	_g		�ΐF[�ȗ���]
		//	@param[in]	_b		�F[�ȗ���]
		//	@param[in]	_a		���ߗ�[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		virtual void Render(
			const int _x, const int _y,
			const float _w, const float _h,
			const float _degree,
			const float _r, const float _g, const float _b, const float _a);

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@param[in]	_x		X���W
		//  @param[in]  _y		Y���W
		//  @param[in]  _w		����
		//  @param[in]  _h		�c��
		//	@param[in]	_degree	��]�p�x(�f�O���[�l)[�ȗ���]
		//	@param[in]	_r		�ԐF[�ȗ���]
		//	@param[in]	_g		�ΐF[�ȗ���]
		//	@param[in]	_b		�F[�ȗ���]
		//	@param[in]	_a		���ߗ�[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		virtual void Render(
			const int _x, const int _y,
			const float _w, const float _h,
			const float _degree = 0,
			const float4& _color = float4::one);

	private:
		struct CutUV
		{
			float2 lt;
			float2 rt;
			float2 lb;
			float2 rb;
			CutUV(
				const float2& _lt, const float2& _rt,
				const float2& _lb, const float2& _rb)
			{
				this->lt = _lt;
				this->rt = _rt;
				this->lb = _lb;
				this->rb = _rb;
			}
		};

	private:
		CutUV uv;
		ao::UiInstance* pInstance;
	};
}

#endif // _Include_iUiObject_hpp_