/*************************************************************
*	@file   	SceneLoadThread.hpp
*	@brief  	SceneLoadThread�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneLoadThread_hpp_	// �C���N���[�h�K�[�h
#define _Include_SceneLoadThread_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include"SceneCtrlProxyComposite.hpp"

namespace ao
{
	class SceneControlProxy;
	class ISceneBase;

	/*!-----------------------------------------------------------
	//	@class  SceneLoadThread
	//	@brief  Scene���[�h�p�̃X���b�h�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SceneLoadThread : public SceneCtrlProxyComposite
	{
	public:
		SceneLoadThread(SceneControlProxy* _pSceneCtrlProxy);
		~SceneLoadThread();

		/*!-----------------------------------------------------------
		//	@brief		���[�h�X���b�h�����J�n
		//	@param[in]	_pISceneBase	�V�[���|�C���^
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		HRESULT BeginLoadThread(ISceneBase* _pISceneBase);

		/*!-----------------------------------------------------------
		//	@brief		���[�h�����̃X���b�h�ҋ@
		//	@return		S_OK�F����I���@����ȊO�F�ُ�I��
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/01
		------------------------------------------------------------*/
		HRESULT WaitForLoadThread();

	private:
		HANDLE hLoadThread;

	private:
		/*!-----------------------------------------------------------
		//	@brief		���[�h�p�̃v���V�[�W��
		//	@param[in]	_argList	�o�^�|�C���^
		//	@return		0�F�ǂݍ��ݐ���
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		static	inline	unsigned WINAPI LoadThread(void* _argList);
	};
}

#endif // _Include_SceneLoadThread_hpp_