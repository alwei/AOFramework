/*************************************************************
*	@file   	MtRandom.hpp
*	@brief  	MtRandom�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/09/03
*************************************************************/

#ifndef _Include_MtRandom_hpp_	// �C���N���[�h�K�[�h
#define _Include_MtRandom_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<random>

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  MtRandom
	//	@brief  �����Z���k�E�c�C�X�^�[�ɂ��C++11����
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/03
	------------------------------------------------------------*/
	class MtRandom
	{
	public:
		/// �R���X�g���N�^
		MtRandom()
		{
			this->mt.seed(std::random_device()());
		};


		/*!-----------------------------------------------------------
		//	@brief		���g�̌Ăяo��
		//  @return     ���g�̃C���X�^���X�Ăяo��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		static	MtRandom* GetInstance()
		{
			static MtRandom singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		���������擾
		//	@param[in]	_min	�ŏ��l[�ȗ���]
		//  @param[in]  _max	�ő�l[�ȗ���]
		//  @return     �������ꂽ����
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/03
		------------------------------------------------------------*/
		int	GetIntRandom(int _min = INT_MIN, int _max = INT_MAX)
		{
			std::uniform_int_distribution<int> dist(_min, _max);
			return dist(this->mt);
		}

		/*!-----------------------------------------------------------
		//	@brief		�������������擾
		//	@param[in]	_min	�ŏ��l[�ȗ���]
		//  @param[in]  _max	�ő�l[�ȗ���]
		//  @return     �������ꂽ����
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		float GetFloatRandom(float _min = -1.0f, float _max = 1.0f)
		{
			std::uniform_real_distribution<float> dist(_min, _max);
			return dist(this->mt);
		}

		/// �f�X�g���N�^
		~MtRandom(){};

	private:
		std::mt19937 mt;
	};
}

#endif // _Include_MtRandom_hpp_