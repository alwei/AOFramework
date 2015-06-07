/*************************************************************
*	@file   	BezierCurve.hpp
*	@brief  	BezierCurve�N���X�w�b�_�[
*	@note		Takumi Yamada���̃N���X������
*	@author		Takumi Yamada
*	@date		2013/09/01
*************************************************************/

#ifndef _Include_BezierCurve_hpp_	// �C���N���[�h�K�[�h
#define _Include_BezierCurve_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include <math.h>


//------------------------------------------------------------
// �萔�A�\���̒�`
//------------------------------------------------------------
const static int nPasTgl[12][12] =
{
	{ 1 },
	{ 1, 1 },
	{ 1, 2, 1 },
	{ 1, 3, 3, 1 },
	{ 1, 4, 6, 4, 1 },
	{ 1, 5, 10, 10, 5, 1 },
	{ 1, 6, 15, 20, 15, 6, 1 },
	{ 1, 7, 21, 35, 35, 21, 7, 1 },
	{ 1, 8, 28, 56, 70, 56, 28, 8, 1 },
	{ 1, 9, 36, 84, 126, 126, 84, 36, 9, 1 },
	{ 1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1 },
	{ 1, 11, 55, 165, 330, 464, 464, 330, 165, 55, 11, 1 }
};

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BezierCurve
	//	@brief  �x�W�F�Ȑ��N���X
	//	@author	Takumi Yamada
	//	@date	2013/09/01
	------------------------------------------------------------*/
	class BezierCurve
	{
	public:

		/*!-----------------------------------------------------------
		//	@brief		�V���O���g���ɂ��Ăяo��
		//	@return		���g�̃C���X�^���X�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		static	BezierCurve* GetInstance()
		{
			static BezierCurve singleton;
			return &singleton;
		}

		//Get�̈����Ɏg�p����p�����[�^
		enum ePrm_t : int
		{					// Prm1                   / Prm2
			eSlow_Lv5,      ///< ��������蓮���n�߂� / ������蓮���I����
			eSlow_Lv4,      ///< ��������蓮���n�߂� / ������蓮���I����
			eSlow_Lv3,      ///< ������蓮���n�߂� / ������蓮���I����
			eSlow_Lv2,      ///< ��������蓮���n�߂� / ������蓮���I����
			eSlow_Lv1,      ///< �������蓮���n�߂� / ������蓮���I����
			eNoAccel,       ///< �����I�ȓ���������
			eRapid_Lv1,     ///< ��}�ɓ����n�߂�      / �}�ɓ����I����
			eRapid_Lv2,     ///< ���}�ɓ����n�߂�      / �}�ɓ����I����
			eRapid_Lv3,     ///< �}�ɓ����n�߂�      / �}�ɓ����I����
			eRapid_Lv4,     ///< ���}�ɓ����n�߂�      / �}�ɓ����I����
			eRapid_Lv5,     ///< ���}�ɓ����n�߂�      / �}�ɓ����I����
		};

		/*
		/*!-----------------------------------------------------------
		//	@brief		�p�����[�^�[�ˑ��̋Ȑ���̒l��Ԃ�
		//	@param[in]	ePrm1        �����n�߂̃p�����[�^(ePrm_t�̒�`�Q��)
		//	@param[in]	ePrm2        �����I��̃p�����[�^(ePrm_t�̒�`�Q��)
		//	@param[in]	fRate        �ω���������������0�`1�œn��
		//	@return		�p�����[�^�[�Ɉˑ������Ȑ��l
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		float Get(ePrm_t ePrm1, ePrm_t ePrm2, float fRate)
		{
			int n = 3;                //n�����w��
			float y1, y2;
			switch (ePrm1)
			{
			case eSlow_Lv5: y1 = 0;                       n = 11; break;//11����
			case eSlow_Lv4: y1 = 0;                       n = 9; break;//9����
			case eSlow_Lv3: y1 = 0;                       n = 7; break;//7����
			case eSlow_Lv2: y1 = 0;                       n = 5; break;//5����
			case eSlow_Lv1: y1 = 0;                       n = 3; break;//3����
			case eNoAccel: y1 = 0.333333f;               n = 3; break;//�����̏ꍇ��3������1/3�̓_
			case eRapid_Lv1: y1 = 1;                       n = 3; break;//3����
			case eRapid_Lv2: y1 = 1;                       n = 5; break;//5����
			case eRapid_Lv3: y1 = 1;                       n = 7; break;//7����
			case eRapid_Lv4: y1 = 1;                       n = 9; break;//9����
			case eRapid_Lv5: y1 = 1;                       n = 11; break;//11����
			}
			switch (ePrm2)
			{
			case eSlow_Lv5: y2 = 1;                       n = 11; break;//11����
			case eSlow_Lv4: y2 = 1;                       n = 9; break;//9����
			case eSlow_Lv3: y2 = 1;                       n = 7; break;//7����
			case eSlow_Lv2: y2 = 1;                       n = 5; break;//5����
			case eSlow_Lv1: y2 = 1;                       n = 3; break;//3����
			case eNoAccel: y2 = 0.6666667f;              n = 3; break;//�����̏ꍇ��3������2/3�̓_
			case eRapid_Lv1: y2 = 0;                       n = 3; break;//3����
			case eRapid_Lv2: y2 = 0;                       n = 5; break;//5����
			case eRapid_Lv3: y2 = 0;                       n = 7; break;//7����
			case eRapid_Lv4: y2 = 0;                       n = 9; break;//9����
			case eRapid_Lv5: y2 = 0;                       n = 11; break;//11����
			}
			return Get(y1, y2, fRate, n);
		}

	private:
		inline float Get(const float &y1, const float &y2, const float &t, const int &n)
		{
			float b = t > 1 ? 1 : (t < 0 ? 0 : t);
			float a = 1 - b;
			float ay = 0;
			float y[4] = { 0, y1, y2, 1 };
			int m;
			for (int i = 0; i <= n; i++){
				m = i == 0 ? 0 : (i == n ? 3 : (i <= n / 2 ? 1 : 2));//y�̓Y��������
				ay += nPasTgl[n][i] * pow(a, n - i)*pow(b, i)*y[m];//��1
			}
			return ay;
		}
	};
}

#endif // _Include_BezierCurve_hpp_
