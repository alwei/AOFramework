/*************************************************************
*	@file   	FpsController.hpp
*	@brief  	FpsControllerクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2013/07/28
*************************************************************/

#ifndef _Include_FpsController_hpp_	// インクルードガード
#define _Include_FpsController_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include<process.h>
#include"CountTime.hpp"
#include"../Utility/NonCopyable.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  FpsController
	//	@brief  fps管理クラス
	//	@author	Tatsunori Aoyama
	//	@date	2013/07/28
	------------------------------------------------------------*/
	class FpsController : private NonCopyable
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		シングルトンによる呼び込み
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		static	FpsController*	GetInstance(){
			static FpsController singleton;
			return &singleton;
		}

		~FpsController();

		/*!-----------------------------------------------------------
		//	@brief		fps用のサブスレッド作成
		//	@param[in]  _frameRate	設定するフレームレート[省略可]
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		HRESULT	Create(DWORD _frameRate = 60);

		/*!-----------------------------------------------------------
		//	@brief		サブスレッドの破棄処理
		//	@return		S_OK：正常終了	それ以外：不正終了
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		HRESULT	Destroy();

		/*!-----------------------------------------------------------
		//	@brief		計測開始
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		void	Begin();

		/*!-----------------------------------------------------------
		//	@brief		計測終了
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		void	End();

		/*!-----------------------------------------------------------
		//	@brief		余剰ミリ秒のリセット
		//	@author		Tatsunori Aoyama
		//	@date		2013/11/23
		------------------------------------------------------------*/
		void	ResetOverTime(){
			this->overTime = 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		fps値の平均を取得
		//	@return		fps平均値
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		template<class T=float>
		const T	GetFps(){
			return static_cast<T>(this->aveFps);
		}

		/*!-----------------------------------------------------------
		//	@brief		処理落ちによる余剰のミリ秒を取得
		//	@return		処理落ちした余剰のミリ秒
		//	@author		Tatsunori Aoyama
		//	@date		2013/11/22
		------------------------------------------------------------*/
		template<class T=float>
		T	GetOverTime(){
			return static_cast<T>(this->overTime);
		}

		/*!-----------------------------------------------------------
		//	@brief		デルタタイム取得
		//	@return		デルタタイム(60FPS=0.016f)
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/02
		------------------------------------------------------------*/
		template<class T=float>
		const T	GetDeltaTime(){
			return static_cast<T>(this->deltaTime);
		}

		/*!-----------------------------------------------------------
		//	@brief		デルタ修正値取得
		//	@return		正規化されたデルタタイム
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/02
		------------------------------------------------------------*/
		template<class T = float>
		const T	GetDeltaFix(){
			return max(1, static_cast<T>(this->deltaFix));
		}

		/*!-----------------------------------------------------------
		//	@brief		更新可能かどうかフラグを取得する
		//	@return		true：更新可能false：更新不可
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		const bool	IsUpdatable() const {
			return this->bUpdate;
		}

	protected:
		HANDLE	hThread;		///< スレッドハンドル
		UINT	threadId;		///< スレッドID
		DWORD	frameRate;		///< フレームレート
		DWORD	frameCount;		///< フレームカウント
		DOUBLE	ms1Frame;		///< 1フレームあたりのミリ秒
		DOUBLE	overTime;		///< 差分のミリ秒
		DOUBLE	deltaFix;		///< デルタ修正値
		DOUBLE	deltaTime;		///< デルタタイム
		DOUBLE	aveFps;			///< fps平均値
		CountTime _1FTimer;		///< 1Fタイマー
		CountTime loopTimer;	///< ループタイマー
		bool	isKilled;		///< 終了フラグ
		bool	bUpdate;		///< 更新可能フラグ

		/*!-----------------------------------------------------------
		//	@brief		fpsカウントメソッド
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		inline	void	CountFps();

		/*!-----------------------------------------------------------
		//	@brief		fps制御メソッド
		//	@return		0：更新終了
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		inline	unsigned Process();

	private:
		/*!-----------------------------------------------------------
		//	@brief		サブスレッドに処理するプロシージャ
		//	@param[in]	argList	渡すポインタ
		//	@return		0：更新終了
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/28
		------------------------------------------------------------*/
		static unsigned WINAPI CallProc(void* argList);

		FpsController();
	};
}

#endif // _Include_FpsController_hpp_