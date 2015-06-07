/*************************************************************
*	@file   	SceneLoadThread.hpp
*	@brief  	SceneLoadThreadクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneLoadThread_hpp_	// インクルードガード
#define _Include_SceneLoadThread_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include"SceneCtrlProxyComposite.hpp"

namespace ao
{
	class SceneControlProxy;
	class ISceneBase;

	/*!-----------------------------------------------------------
	//	@class  SceneLoadThread
	//	@brief  Sceneロード用のスレッドクラス
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SceneLoadThread : public SceneCtrlProxyComposite
	{
	public:
		SceneLoadThread(SceneControlProxy* _pSceneCtrlProxy);
		~SceneLoadThread();

		/*!-----------------------------------------------------------
		//	@brief		ロードスレッド処理開始
		//	@param[in]	_pISceneBase	シーンポインタ
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		HRESULT BeginLoadThread(ISceneBase* _pISceneBase);

		/*!-----------------------------------------------------------
		//	@brief		ロード処理のスレッド待機
		//	@return		S_OK：正常終了　それ以外：異常終了
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/01
		------------------------------------------------------------*/
		HRESULT WaitForLoadThread();

	private:
		HANDLE hLoadThread;

	private:
		/*!-----------------------------------------------------------
		//	@brief		ロード用のプロシージャ
		//	@param[in]	_argList	登録ポインタ
		//	@return		0：読み込み成功
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		static	inline	unsigned WINAPI LoadThread(void* _argList);
	};
}

#endif // _Include_SceneLoadThread_hpp_