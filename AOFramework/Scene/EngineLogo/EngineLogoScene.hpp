/*************************************************************
*	@file   	EngineLogoScene.hpp
*	@brief  	EngineLogoSceneクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/10/15
*************************************************************/

#ifndef _Include_EngineLogoScene_hpp_	// インクルードガード
#define _Include_EngineLogoScene_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../AoScene.hpp"
#include"../../Resource/Sprite.hpp"
#include"../../Utility/MsTime.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  EngineLogoScene
	//	@brief  エンジンロゴシーン
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/15
	------------------------------------------------------------*/
	class EngineLogoScene : public ao::SceneBase
	{
	public:
		/// コンストラクタ
		EngineLogoScene();

		/// デストラクタ
		~EngineLogoScene();


		/*!-----------------------------------------------------------
		//	@brief		セットアップ
		//	@param[in]	_pMainScene	ユーザー指定のメインシーン
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/15
		------------------------------------------------------------*/
		void Setup(SceneBase* _pMainScene)
		{
			this->pMainScene = _pMainScene;
		}

		/*!-----------------------------------------------------------
		//	@brief		ロード前に実行する
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/15
		------------------------------------------------------------*/
		void	InitPreLoad();

		/*!-----------------------------------------------------------
		//	@brief		ロードスレッド
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/15
		------------------------------------------------------------*/
		void	LoadThread();

		/*!-----------------------------------------------------------
		//	@brief		シーン実行前にコールされる
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/15
		------------------------------------------------------------*/
		void	OnStartCall();

	protected:
		/*!-----------------------------------------------------------
		//	@brief		更新
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/15
		------------------------------------------------------------*/
		void	OnUpdate();

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/15
		------------------------------------------------------------*/
		void	OnRender();

	private:
		enum class State : int
		{
			FadeStandby = 0,
			FadeIn,
			FadeNow,
			FadeOut,
			FadeEnd
		};

		BOOL isSkip;
		State nowState;
		ao::Sprite* engineLogo;
		SceneBase* pMainScene;
		MsTime msTime;
		float logoAlpha;
	};
}

#endif // _Include_EngineLogoScene_hpp_