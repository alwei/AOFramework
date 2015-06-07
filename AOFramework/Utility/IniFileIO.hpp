/*************************************************************
*	@file   	IniFileIO.hpp
*	@brief  	IniFileIO�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/12/16
*************************************************************/

#ifndef _Include_IniFileIO_hpp_	// �C���N���[�h�K�[�h
#define _Include_IniFileIO_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include<stdio.h>
#include<tchar.h>
#include<array>
#include"../Define/SystemDefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  IniFileIO
	//	@brief  ini�t�@�C����ǂݍ��ރN���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/12/16
	------------------------------------------------------------*/
	class IniFileIO
	{
	public:
		IniFileIO();
		IniFileIO(const ao::string& _filePath);
		~IniFileIO();

		/*!-----------------------------------------------------------
		//	@brief		�t�@�C���I�[�v��
		//	@param[in]	_filePath	�t�@�C���p�X
		//  @return     �����F�t�@�C���p�X�������@���s�F-1
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/16
		------------------------------------------------------------*/
		int Open(const ao::string& _filePath);

		/*!-----------------------------------------------------------
		//	@brief		�w��Z�N�V�����̏��擾
		//	@param[in]	_sectionName  �Z�N�V�����l�[��
		//	@param[in]	_keyName      �L�[��
		//	@param[in]	_defaultParam ����ɐݒ肷��l[�ȗ���]
		//	@return		�w�肵���l
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/16
		------------------------------------------------------------*/
		const float GetFloat(
			const ao::string& _sectionName,
			const ao::string& _keyName,
			const float _defaultParam = 0);

		/*!-----------------------------------------------------------
		//	@brief		�w��Z�N�V�����̏��擾
		//	@param[in]	_sectionName  �Z�N�V�����l�[��
		//	@param[in]	_keyName      �L�[��
		//	@param[in]	_defaultParam ����ɐݒ肷��l[�ȗ���]
		//	@return		�w�肵���l
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/16
		------------------------------------------------------------*/
		const int	GetInt(
			const ao::string& _sectionName,
			const ao::string& _keyName,
			const int _defaultParam = 0);

		/*!-----------------------------------------------------------
		//	@brief		�w��Z�N�V�����̏��擾
		//  @param[out] _pOut			�o�͂���o�b�t�@
		//  @param[in]  _size			�o�͐�̃o�b�t�@�T�C�Y
		//	@param[in]	sectionName		�Z�N�V�����l�[��
		//	@param[in]	keyName			�L�[��
		//	@param[in]	defaultParam	����ɐݒ肷��l[�ȗ���]
		//	@return		�w�肵���l
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/16
		------------------------------------------------------------*/
		const AOCHAR* GetString(
			AOCHAR* _pOut,
			const size_t _size,
			const ao::string& _sectionName,
			const ao::string& _keyName,
			const ao::string& _defaultParam = _T(""));


		/*!-----------------------------------------------------------
		//	@brief		�w��Z�N�V�����̏��擾
		//	@param[in]	sectionName		�Z�N�V�����l�[��
		//	@param[in]	keyName			�L�[��
		//	@param[in]	defaultParam	����ɐݒ肷��l[�ȗ���]
		//  @return     TRUE�F������TRUE�@FALSE�F������FALSE
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/16
		------------------------------------------------------------*/
		const BOOL GetBool(
			const ao::string& _sectionName,
			const ao::string& _keyName,
			const BOOL _defaultParam = FALSE);

	private:
		enum Buffer
		{
			Temp = 128,
			Path = 512
		};

	private:
		std::array<AOCHAR, Buffer::Path> filePath;
		std::array<AOCHAR, Buffer::Temp> buffer;
	};
}

#endif // _Include_IniFileIO_hpp_