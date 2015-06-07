/*************************************************************
*	@file   	SceneControlProxy.hpp
*	@brief  	SceneControlProxyクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_SceneControlProxy_hpp_	// インクルードガード
#define _Include_SceneControlProxy_hpp_

//------------------------------------------------------------
// インクルード
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
	//	@brief  シーン制御プロキシ
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class SceneControlProxy : public SystemComposite
	{
	public:
		SceneControlProxy(SystemScene* _pSystemScene);
		~SceneControlProxy();

		/*!-----------------------------------------------------------
		//	@brief		最初のシーン設定
		//	@param[in]	_pFirstScene	最初に設定するシーン
		//	@return		S_OK：設定成功　それ以外：設定失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		HRESULT	SetFirstScene(ISceneBase* _pFirstScene);

		/*!-----------------------------------------------------------
		//	@brief		シーンの実行
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/01
		------------------------------------------------------------*/
		void Execute();

		/*!-----------------------------------------------------------
		//	@brief		終了処理
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		void Finalize();

		/*!-----------------------------------------------------------
		//	@brief		新しいシーンの読み込み
		//	@param[in]	_pNewScene	新しいシーン
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
		//	@brief		ロードの初期化
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		FORCEINLINE HRESULT InitLoad();

		/*!-----------------------------------------------------------
		//	@brief		シーン交換
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		FORCEINLINE HRESULT ExchangeScene();
	};
}

#endif // _Include_SceneControlProxy_hpp_