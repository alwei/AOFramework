/*************************************************************
*	@file   	FpsController.hpp
*	@brief  	FpsController�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2013/07/28
*************************************************************/

#ifndef _Include_FpsController_hpp_	// �C���N���[�h�K�[�h
#define _Include_FpsController_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include<process.h>
#include"CountTime.hpp"
#include"../Utility/NonCopyable.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  FpsController
	//	@brief  fps�Ǘ��N���X
	//	@author	Tatsunori Aoyama
	//	@date	2013/07/28
	------------------------------------------------------------*/
	class FpsController : private NonCopyable
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		�V���O���g���ɂ��Ăэ���
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		static	FpsController*	GetInstance(){
			static FpsController singleton;
			return &singleton;
		}

		~FpsController();

		/*!-----------------------------------------------------------
		//	@brief		fps�p�̃T�u�X���b�h�쐬
		//	@param[in]  _frameRate	�ݒ肷��t���[�����[�g[�ȗ���]
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		HRESULT	Create(DWORD _frameRate = 60);

		/*!-----------------------------------------------------------
		//	@brief		�T�u�X���b�h�̔j������
		//	@return		S_OK�F����I��	����ȊO�F�s���I��
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		HRESULT	Destroy();

		/*!-----------------------------------------------------------
		//	@brief		�v���J�n
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		void	Begin();

		/*!-----------------------------------------------------------
		//	@brief		�v���I��
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		void	End();

		/*!-----------------------------------------------------------
		//	@brief		�]��~���b�̃��Z�b�g
		//	@author		Tatsunori Aoyama
		//	@date		2013/11/23
		------------------------------------------------------------*/
		void	ResetOverTime(){
			this->overTime = 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		fps�l�̕��ς��擾
		//	@return		fps���ϒl
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		template<class T=float>
		const T	GetFps(){
			return static_cast<T>(this->aveFps);
		}

		/*!-----------------------------------------------------------
		//	@brief		���������ɂ��]��̃~���b���擾
		//	@return		�������������]��̃~���b
		//	@author		Tatsunori Aoyama
		//	@date		2013/11/22
		------------------------------------------------------------*/
		template<class T=float>
		T	GetOverTime(){
			return static_cast<T>(this->overTime);
		}

		/*!-----------------------------------------------------------
		//	@brief		�f���^�^�C���擾
		//	@return		�f���^�^�C��(60FPS=0.016f)
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/02
		------------------------------------------------------------*/
		template<class T=float>
		const T	GetDeltaTime(){
			return static_cast<T>(this->deltaTime);
		}

		/*!-----------------------------------------------------------
		//	@brief		�f���^�C���l�擾
		//	@return		���K�����ꂽ�f���^�^�C��
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/02
		------------------------------------------------------------*/
		template<class T = float>
		const T	GetDeltaFix(){
			return max(1, static_cast<T>(this->deltaFix));
		}

		/*!-----------------------------------------------------------
		//	@brief		�X�V�\���ǂ����t���O���擾����
		//	@return		true�F�X�V�\false�F�X�V�s��
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		const bool	IsUpdatable() const {
			return this->bUpdate;
		}

	protected:
		HANDLE	hThread;		///< �X���b�h�n���h��
		UINT	threadId;		///< �X���b�hID
		DWORD	frameRate;		///< �t���[�����[�g
		DWORD	frameCount;		///< �t���[���J�E���g
		DOUBLE	ms1Frame;		///< 1�t���[��������̃~���b
		DOUBLE	overTime;		///< �����̃~���b
		DOUBLE	deltaFix;		///< �f���^�C���l
		DOUBLE	deltaTime;		///< �f���^�^�C��
		DOUBLE	aveFps;			///< fps���ϒl
		CountTime _1FTimer;		///< 1F�^�C�}�[
		CountTime loopTimer;	///< ���[�v�^�C�}�[
		bool	isKilled;		///< �I���t���O
		bool	bUpdate;		///< �X�V�\�t���O

		/*!-----------------------------------------------------------
		//	@brief		fps�J�E���g���\�b�h
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		inline	void	CountFps();

		/*!-----------------------------------------------------------
		//	@brief		fps���䃁�\�b�h
		//	@return		0�F�X�V�I��
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		inline	unsigned Process();

	private:
		/*!-----------------------------------------------------------
		//	@brief		�T�u�X���b�h�ɏ�������v���V�[�W��
		//	@param[in]	argList	�n���|�C���^
		//	@return		0�F�X�V�I��
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		static unsigned WINAPI CallProc(void* argList);

		FpsController();
	};
}

#endif // _Include_FpsController_hpp_