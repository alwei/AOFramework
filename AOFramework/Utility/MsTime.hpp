/*************************************************************
*	@file   	MsTime.hpp
*	@brief  	MsTime�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/01/01
*************************************************************/

#ifndef _Include_MsTime_hpp_	// �C���N���[�h�K�[�h
#define _Include_MsTime_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"BezierCurve.hpp"
#include"../System/FpsController.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  MsTime
	//	@brief  ���ԕ�ԃN���X
	//	@author	Tatsunori Aoyama
	//	@date	2015/01/01
	------------------------------------------------------------*/
	class MsTime
	{
	public:
		MsTime() :
			nowTime(0),
			maxTime(1),
			normalizeTime(0){};

		~MsTime(){};

		/*!-----------------------------------------------------------
		//	@brief		�p�����[�^�[�ݒ�
		//	@param[in]	_setTime	�ݒ�^�C��
		//	@param[in]	_initTime	����������[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/01
		------------------------------------------------------------*/
		void SetParameter(float _setTime,float _initTime=0)
		{
			this->nowTime = _initTime;
			this->maxTime = _setTime;
			this->normalizeTime = this->nowTime / this->maxTime;
		}

		/*!-----------------------------------------------------------
		//	@brief		�X�V
		//	@param[in]	_limitDelta	�f���^�^�C���̐���l(2.0f=30F���E�j
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/01
		------------------------------------------------------------*/
		void Update(float _limitDelta = 0.032f)
		{
			float d = min(_limitDelta, ao::FpsController::GetInstance()->GetDeltaTime());
			this->nowTime = min(this->nowTime + d, this->maxTime);
			this->normalizeTime = this->nowTime / this->maxTime;
		}

		/*!-----------------------------------------------------------
		//	@brief		���K�����ꂽ���Ԏ擾
		//	@return		���K�����ꂽ���ԁi0~1.0f�j
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/01
		------------------------------------------------------------*/
		const float GetNormalizedTime() const
		{
			return this->normalizeTime;
		}

		/*!-----------------------------------------------------------
		//	@brief		�I���������ǂ���
		//	@return		TRUE�F�I���@FALSE�F�I�����Ă��Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/01
		------------------------------------------------------------*/
		const BOOL IsEnd() const
		{
			return (this->nowTime >= this->maxTime);
		}

		/*!-----------------------------------------------------------
		//	@brief		���Z�b�g
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/01
		------------------------------------------------------------*/
		void Reset()
		{
			this->nowTime = 0;
			this->normalizeTime = 0;
		}

	public:
		float nowTime;
		float maxTime;
		float normalizeTime;
	};
}

#endif // _Include_MsTime_hpp_