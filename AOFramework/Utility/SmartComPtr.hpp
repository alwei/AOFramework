/*************************************************************
*	@file   	SmartComPtr.hpp
*	@brief  	SmartComPtr�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2013/06/13
*************************************************************/

#ifndef _Include_SmartComPtr_hpp_	// �C���N���[�h�K�[�h
#define _Include_SmartComPtr_hpp_


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  SmartComPtr
	//	@brief  COM��p�̃X�}�[�g�|�C���^
	//	@author	Tatsunori Aoyama
	//	@date	2013/03/10
	------------------------------------------------------------*/
	template<class T>
	class	SmartComPtr
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		�������R���X�g���N�^
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		SmartComPtr()
		{
			this->pInterface = nullptr;	//NULL���
			this->refCount = 0;		//�Q�ƃJ�E���^������
		}

		/*!-----------------------------------------------------------
		//	@brief		�Öق̌^�ϊ��֎~�ɂ��R���X�g���N�^
		//	@note		�����Ŏ󂯎��Ƃ��ɃX�}�[�g�|�C���^�̑����h��
		//	@param[in]	pInterface COM�Ɨ\�z�����|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		explicit	SmartComPtr(T* pInterface)
		{
			if (pInterface)
				this->refCount = pInterface->AddRef();

			this->pInterface = pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		�R�s�[�R���X�g���N�^
		//	@note		�Q�ƃJ�E���g���C���N�������g����
		//	@param[in]	src �R�s�[����COM��p�X�}�[�g�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		SmartComPtr(SmartComPtr& src)
		{
			if (src.pInterface)
				this->refCount = src.pInterface->AddRef();

			this->pInterface = src.pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		�f�X�g���N�^�ɂ��COM�|�C���^�̉��
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		~SmartComPtr()
		{
			if (this->pInterface)
				this->refCount = this->pInterface->Release();

			if (this->refCount == 0)
				this->pInterface = nullptr;
		}

		/*!-----------------------------------------------------------
		//	@brief		�����I�ȑ�����Z�q�ɂ��R�s�[
		//	@note		SmartComPtr�̃R�s�[�ɂȂ�
		//	@param[in]	src �E�ӂ̎��g�Ɠ����^
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		SmartComPtr	operator=(SmartComPtr& src)
		{
			//�R�s�[��̎Q�ƃJ�E���g++
			if (src.pInterface)
				this->refCount = src.pInterface->AddRef();

			//���g�̃|�C���^�͑���ɂ���Ĕj�������̂�
			//�Ō�ɎQ�ƃJ�E���g�����炵�Č�͑��Ő��䂵�Ă���COM�ɔC����
			if (this->pInterface)
				this->refCount = this->pInterface->Release();

			if (this->refCount == 0)
				this->pInterface = nullptr;


			this->pInterface = src.pInterface;

			return	(*this);
		}

		/*!-----------------------------------------------------------
		//	@brief		������Z�ɂ��C���^�[�t�F�[�X�o�^
		//	@param[in]	pInterface COM�|�C���^�ɂ�铧�ߓI�ȑ��
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		void	operator=(T* pInterface)
		{
			//���g��COM�|�C���^��������A
			//��͑��Ő��䂵�Ă���COM�ɔC����
			if (this->pInterface)
				this->refCount = this->pInterface->Release();

			this->pInterface = pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		��r���Z�q==
		//	@note		�A�h���X�l�̔�r
		//	@param[in]	adr �A�h���X�l
		//	@return		�����ƈ�v���Ă�����TRUE�A��v���Ă��Ȃ����FALSE
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		bool	operator==(int adr)
		{
			if (adr == (int)this->pInterface)
				return	true;
			nullptr
				return	false;
		}

		/*!-----------------------------------------------------------
		//	@brief		��r���Z�q!=
		//	@note		�A�h���X�l�̔�r
		//	@param[in]	adr �A�h���X�l
		//	@return		�����ƈ�v���Ă�����TRUE�A��v���Ă��Ȃ����FALSE
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		bool	operator!=(int adr)
		{
			if (adr != (int)this->pInterface)
				return	true;

			return	false;
		}

		/*!-----------------------------------------------------------
		//	@brief		��r���Z�q==
		//	@note		�A�h���X�l�̔�r
		//	@param[in]	adr �A�h���X�l
		//	@return		�����ƈ�v���Ă�����TRUE�A��v���Ă��Ȃ����FALSE
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		bool	operator==(void* adr)
		{
			if ((int)adr == (int)this->pInterface)
				return	true;
			else
				return	false;
		}

		/*!-----------------------------------------------------------
		//	@brief		��r���Z�q!=
		//	@note		�A�h���X�l�̔�r
		//	@param[in]	adr �A�h���X�l
		//	@return		�����ƈ�v���Ă�����TRUE�A��v���Ă��Ȃ����FALSE
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		bool	operator!=(void* adr)
		{
			if ((int)adr != (int)this->pInterface)
				return	true;

			return	false;
		}

		/*!-----------------------------------------------------------
		//	@brief		�A���[���Z�q�̃I�[�o�[���[�h
		//	@note		COM�C���^�[�t�F�C�X�̓��ߐ����グ��
		//	@return		�C���^�[�t�F�[�X�̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T*	operator->(){
			return	this->pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		�ԐڎQ�Ɖ��Z�q�̃I�[�o�[���[�h
		//	@note		�|�C���^�A�N�Z�X�ɂ��COM�C���^�[�t�F�[�X�̓��ߐ����グ��
		//	@return		COM�̃G�C���A�X
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T&	operator*(){
			return	(*this->pInterface);
		}

		/*!-----------------------------------------------------------
		//	@brief		�z��Q�Ɖ��Z�q�̃I�[�o�[���[�h
		//	@note		�C���f�b�N�X���t�w��ɂ��Q�Ƃ�COM�C���^�[�t�F�[�X�̓��ߐ����グ��
		//	@param[in]	�C���f�b�N�X�ԍ��i�Y�����j
		//	@return		COM�̃G�C���A�X
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T&	operator[](int n){
			return	this->pInterface[n];
		}

		/*!-----------------------------------------------------------
		//	@brief		&���Z�q���I�[�o�[���[�h
		//	@note		�|�C���^�擾�̓��ߐ����グ��
		//	@return		COM�|�C���^�̃A�h���X
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T**	operator&(){
			return	&this->pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		���݂̎Q�ƃJ�E���^�擾
		//	@return		���݂�COM�C���^�[�t�F�[�X�̎Q�ƃJ�E���g
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		unsigned int	GetNowRefCount(){
			return this->refCount;
		}

		/*!-----------------------------------------------------------
		//	@brief		COM�C���^�[�t�F�[�X�|�C���^�擾
		//	@note		�����I�ɕ�����₷���悤�֐���p��
		//	@return		COM�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T*	GetComPtr(){
			return this->pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		COM�|�C���^�̃A�h���X�擾
		//	@note		�g��Ȃ���������Ȃ����Ǘp�ӂ��Ă���
		//	@return		COM�|�C���^�̃A�h���X
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T**	GetComPtrPtr(){
			return &this->pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		COM�C���^�[�t�F�[�X�������ɓn����p�֐�
		//	@note		�����I�ɂ��邱�Ƃɂ��A�g�p�p�r���c���ł���
		//	@return		COM�|�C���^�̃A�h���X
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/10
		------------------------------------------------------------*/
		T**	ToCreator()
		{
			this->refCount = 1;

			//���g��COM��������A
			//��͑��Ő��䂵�Ă���COM�ɔC����
			if (this->pInterface)
				this->refCount = this->pInterface->Release();
			if (this->refCount == 0)
				this->pInterface = nullptr;

			return	&this->pInterface;
		}

		/*!-----------------------------------------------------------
		//	@brief		COM�C���^�[�t�F�[�X�̖����I���
		//	@note		����͂��܂�g��Ȃ��łق���
		//	@author		Tatsunori Aoyama
		//	@date		2013/06/13
		------------------------------------------------------------*/
		int ReleaseComPtr()
		{
			if (this->pInterface)
				this->refCount = this->pInterface->Release();

			if (this->refCount == 0)
				this->pInterface = nullptr;

			return this->refCount;
		}

	private:
		T*				pInterface;		///< �C���^�[�t�F�[�X�̃e���v���[�g
		unsigned int	refCount;		///< �Q�Ɛ�
	};
}


#endif // _Include_SmartComPtr_hpp_