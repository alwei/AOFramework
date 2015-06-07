/*************************************************************
*	@file   	iUiObjectFactory.hpp
*	@brief  	iUiObjectFactory�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/03/29
*************************************************************/

#ifndef _Include_iUiObjectFactory_hpp_	// �C���N���[�h�K�[�h
#define _Include_iUiObjectFactory_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<list>
#include<Windows.h>
#include"../Define/SystemDefine.hpp"
#include"../Api/DirectX11Resource.hpp"


namespace ao
{
	class iUiObject;
	class iUiObjectExecutor;
	class float2;
	struct UiInstance;

	/*!-----------------------------------------------------------
	//	@class  iUiObjectFactory
	//	@note	��̃A�g���X�̂ݑΉ�
	//	@brief  UI�C���X�^���X�t�@�N�g��
	//	@author	Tatsunori Aoyama
	//	@date	2015/03/29
	------------------------------------------------------------*/
	class iUiObjectFactory
	{
	public:
		iUiObjectFactory();
		~iUiObjectFactory();

		/*!-----------------------------------------------------------
		//	@brief		�C���X�^���X�쐬
		//	@param[in]	_filePath	�ǂݍ��ރt�@�C���p�X
		//	@param[in]	_numAlloc	�m�ۂ���o�b�t�@��
		//	@return		�����_�����O���s�N���X
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		iUiObjectExecutor* CreateInstance(
			const string _filePath,
			const UINT _numAlloc = 100,
			const SamplerState _sampState = SamplerState::LINEAR);

		/*!-----------------------------------------------------------
		//	@brief		UI�C���X�^���X�I�u�W�F�N�g�쐬
		//	@param[in]	_leftTop		����UV���W�i�s�N�Z���P�ʁj
		//	@param[in]	_rightTop		�E��UV���W�i�s�N�Z���P�ʁj
		//	@param[in]	_leftBottom		����UV���W�i�s�N�Z���P�ʁj
		//	@param[in]	_rightBottom	�E��UV���W�i�s�N�Z���P�ʁj
		//	@return		�������ꂽUI�C���X�^���X�I�u�W�F�N�g
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		iUiObject* CreateObject(
			const float2& _leftTop,
			const float2& _rightTop,
			const float2& _leftBottom,
			const float2& _rightBottom);

		/*!-----------------------------------------------------------
		//	@brief		UI�C���X�^���X�I�u�W�F�N�g�쐬
		//	@param[in]	_ppOutArray		�o�͂���UI�I�u�W�F�N�g�z��
		//	@param[in]	_xNum			���̕�����
		//	@param[in]	_yNum			�c�̕�����
		//	@param[in]	_xSize			�ꖇ���̉��T�C�Y
		//	@param[in]	_ySize			�ꖇ���̏c�T�C�Y
		//	@param[in]	_atlasWidth		�A�g���X�̉���
		//	@param[in]	_atlasHeight	�A�g���X�̏c��
		//	@return		�������ꂽUI�C���X�^���X�I�u�W�F�N�g
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		HRESULT CreateObjectArray(
			iUiObject** const _ppOutArray,
			const int _xNum, const int _yNum,
			const int _xSize, const int _ySize,
			const int _atlasWidth, const int _atlasHeight);

	private:
		UiInstance* pInstance;
		iUiObjectExecutor* pExecutor;
		std::list<iUiObject*> pBufferList;

	private:
		/*!-----------------------------------------------------------
		//	@brief		�o�b�t�@���
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		inline void DeleteBuffer();
	};
}

#endif // _Include_iUiObjectFactory_hpp_