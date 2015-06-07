/*************************************************************
*	@file   	SceneControlProxy.hpp
*	@brief  	SceneControlProxy�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneControlProxy_hpp_	// �C���N���[�h�K�[�h
#define _Include_SceneControlProxy_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<list>
#include<Windows.h>
#include"SystemComposite.hpp"

namespace ao
{
	class ISceneBase;
	class SceneBase;
	class SystemScene;
	class SystemComposite;
	class SceneDatabase;
	class SceneExecutor;
	class SceneChanger;
	class SceneDeleter;
	class SceneLoadThread;
	class SceneCtrlProxyComposite;

	/*!-----------------------------------------------------------
	//	@class  SceneControlProxy
	//	@brief  �V�[������v���L�V
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SceneControlProxy : public SystemComposite
	{
	public:
		SceneControlProxy(SystemScene* _pSystemScene);
		~SceneControlProxy();

		/*!-----------------------------------------------------------
		//	@brief		�ŏ��̃V�[���ݒ�
		//	@param[in]	_pFirstScene	�ŏ��ɐݒ肷��V�[��
		//	@return		S_OK�F�ݒ萬���@����ȊO�F�ݒ莸�s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		HRESULT	SetFirstScene(ISceneBase* _pFirstScene);

		/*!-----------------------------------------------------------
		//	@brief		�V�[���̎��s
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/01
		------------------------------------------------------------*/
		void Execute();

		/*!-----------------------------------------------------------
		//	@brief		�I������
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		void Finalize();

		/*!-----------------------------------------------------------
		//	@brief		�V�����V�[���̓ǂݍ���
		//	@param[in]	_pNewScene	�V�����V�[��
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/01
		------------------------------------------------------------*/
		void LoadLevel(SceneBase* _pNewScene);

	private:
		SceneExecutor* pSceneExecutor;
		SceneChanger* pSceneChanger;
		SceneDeleter* pSceneDeleter;
		SceneLoadThread* pSceneLoadThread;
		SceneDatabase* pSceneDatabase;
		std::list< SceneCtrlProxyComposite* > compositeList;

	private:
		/*!-----------------------------------------------------------
		//	@brief		���[�h�̏�����
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		FORCEINLINE HRESULT InitLoad();

		/*!-----------------------------------------------------------
		//	@brief		�V�[������
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		FORCEINLINE HRESULT ExchangeScene();
	};
}

#endif // _Include_SceneControlProxy_hpp_