#ifndef __DEBUG_SYSTEM_HPP__
#define	__DEBUG_SYSTEM_HPP__

#include<tchar.h>
#include"DebugConsole.hpp"

#ifdef __AO_DEBUG__
#include<crtdbg.h>
#endif

namespace ao
{
	namespace debug
	{
		/*!-----------------------------------------------------------
		//	@brief		�������_���v�̏�����
		//	@return		_crtDbgFlag �̈ȑO�̏��
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/26
		------------------------------------------------------------*/
		inline	int	InitMemDump(int _flags = _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
		{
#ifdef __AO_DEBUG__
			return _CrtSetDbgFlag(_flags);
#endif
			return 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		���������[�N���o
		//	@param[in]	_breakAlloc	�u���[�N�|�C���g��ݒ肷�銄�蓖�ď����ԍ�
		//	@return		�u���[�N�|�C���g���ݒ肳��Ă����A�ȑO�̃I�u�W�F�N�g���蓖�ď����ԍ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/26
		------------------------------------------------------------*/
		inline	int	BreakAlloc(long _breakAlloc){
#ifdef __AO_DEBUG__
			return _CrtSetBreakAlloc(_breakAlloc);
#endif
			return 0;
		}

		static const int DUMMY_ZERO = 0;

#if defined(_DEBUG) || defined(DEBUG) || defined(RELEASE_DEBUG)
		/*!-----------------------------------------------------------
		//	@brief		�f�o�b�O�R���\�[���̏�����
		//	@author		Tatsunori Aoyama
		//	@date		2013/04/27
		------------------------------------------------------------*/
		inline	void	InitDebugConsole(void)
		{
			DebugConsole::GetInstance()->RemoveClose();
		}

		/*!-----------------------------------------------------------
		//	@brief		���O�쐬������
		//	@param[in]	fileName	�������ރt�@�C����
		//	@author		Tatsunori Aoyama
		//	@date		2013/10/05
		------------------------------------------------------------*/
		inline	void	InitDebugLog(AOLPCSTR _fileName)
		{
			DebugConsole::GetInstance()->CreateDebugLog(_fileName);
		}

		/*!-----------------------------------------------------------
		//	@brief		�~�{�^���𖳌���
		//	@author		Tatsunori Aoyama
		//	@date		2013/12/04
		------------------------------------------------------------*/
		inline	void	RemoveClose()
		{
			DebugConsole::GetInstance()->RemoveClose();
		}

		/*!-----------------------------------------------------------
		//	@brief		������̕`��
		//	@param[in]	_str	������
		//	@param[in]	...		�ψ���
		//	@author		Tatsunori Aoyama
		//	@date		2013/04/27
		------------------------------------------------------------*/
		void Printf(CONST AOLPSTR _str, ...);

		/*!-----------------------------------------------------------
		//	@brief		�w��F�ŕ�����̕`��
		//	@param[in]	_textColor �����F
		//	@param[in]	_str       ������
		//	@param[in]	...       �ψ���
		//	@author		Tatsunori Aoyama
		//	@date		2013/04/27
		------------------------------------------------------------*/
		void PrintfColor(ConsoleColor::Enum _textColor, CONST AOLPSTR _str, ...);

		/*!-----------------------------------------------------------
		//	@brief		������̕`��
		//	@note		���O�o�͔�
		//	@param[in]	_str	������
		//	@param[in]	...		�ψ���
		//	@author		Tatsunori Aoyama
		//	@date		2013/05/20
		------------------------------------------------------------*/
		void PrintfLog(CONST AOLPSTR _str, ...);

		/*!-----------------------------------------------------------
		//	@brief		�w��F�ŕ�����̕`��
		//	@note		���O�o�͔�
		//	@param[in]	_textColor	�����F
		//	@param[in]	_str		������
		//	@param[in]	...			�ψ���
		//	@author		Tatsunori Aoyama
		//	@date		2013/05/20
		------------------------------------------------------------*/
		void PrintfColorLog(ConsoleColor::Enum _textColor, CONST AOLPSTR _str, ...);

#else
#define InitDebugConsole()					DUMMY_ZERO
#define InitDebugLog(fileName)				DUMMY_ZERO
#define RemoveClose()						DUMMY_ZERO
#define Printf(str, ... )					DUMMY_ZERO
#define PrintfLog(str,... )					DUMMY_ZERO
#define PrintfColor(fontColor,str, ... )	DUMMY_ZERO
#define PrintfColorLog(textColor,str,...)	DUMMY_ZERO

#endif
	}	
}

#endif