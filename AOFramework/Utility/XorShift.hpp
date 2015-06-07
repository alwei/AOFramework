/*************************************************************
*	@file   	xorShift.hpp
*	@brief  	xorShift�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2013/05/19
*************************************************************/

#ifndef _Include_XorShift_hpp_	// �C���N���[�h�K�[�h
#define _Include_XorShift_hpp_


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  XorShift
	//	@brief  Xorshif���g�p���������N���X
	//	@author	Tatsunori Aoyama
	//	@date	2013/05/19
	------------------------------------------------------------*/
	class XorShift
	{
	public:
		~XorShift(){};

		/*!-----------------------------------------------------------
		//	@brief		�C���X�^���X�擾
		//	@return		���g�̎Q��
		//	@author		Tatsunori Aoyama
		//	@date		2013/05/19
		------------------------------------------------------------*/
		static	XorShift*	GetInstance(){
			static	XorShift singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		�����̃V�[�h�l�ݒ�
		//	@param[in]	setSeed �ݒ肷��V�[�h�l
		//	@author		Tatsunori Aoyama
		//	@date		2013/05/19
		------------------------------------------------------------*/
		void	InitSeed(_int64 setSeed){
			this->seed = setSeed;
			this->z ^= this->seed;
			this->z ^= this->z >> 21;
			this->z ^= this->z << 35;
			this->z ^= this->z >> 4;
			this->w ^= this->z << 2;
			this->w *= 2685821692745602874LL;
			this->z *= 2685821657736338717LL;
		}

		/*!-----------------------------------------------------------
		//	@brief		�����̎擾
		//	@return		�擾���闐��
		//	@author		Tatsunori Aoyama
		//	@date		2013/05/19
		------------------------------------------------------------*/
		_int64 GetRandom(){
			_int64 t = (this->x ^ (this->x << 11));
			this->x = this->y;
			this->y = this->z;
			this->z = this->w;
			return (this->w = (this->w ^ (this->w >> 19)) ^ (t ^ (t >> 8)));
		}

		/*!-----------------------------------------------------------
		//	@brief		�͈͗����̎擾
		//	@param[in]	min �ŏ��l�i�͈͂Ɋ܂܂��j
		//	@param[in]	max �ő�l�i�͈͂Ɋ܂܂��j
		//	@return		�擾���闐��
		//	@author		Tatsunori Aoyama
		//	@date		2013/05/19
		------------------------------------------------------------*/
		_int64 GetRandom(int min, int max){
			return (this->GetRandom() % (max + 1 - min) + min);
		}

	private:
		_int64 x;
		_int64 y;
		_int64 z;
		_int64 w;
		_int64 seed;
	private:
		XorShift()
		{
			this->x = 123456789;
			this->y = 362436069;
			this->z = 521288629;
			this->w = 88675123;
			this->seed = 1;
		};
	};
}


#endif // _Include_XorShift_hpp_