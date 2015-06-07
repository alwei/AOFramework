/*************************************************************
*	@file   	SystemScene.hpp
*	@brief  	SystemSceneクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/07/28
*************************************************************/

#ifndef _Include_SystemScene_hpp_	// インクルードガード
#define _Include_SystemScene_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include<DirectXMath.h>
#include<dxgi.h>
#include"../Audio/XAudio2.hpp"
#include"../PhysX/PhysXScene.hpp"

namespace ao
{
	class Window;
	class SceneBase;
	class ApiScene;
	class ShaderScene;
	class SceneControlProxy;
	class ImmdiateTaskSystem;
	class AoTaskSystem;

	/*!-----------------------------------------------------------
	//	@class  SystemScene
	//	@brief  Frameworkのシステムシーン
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/28
	------------------------------------------------------------*/
	class SystemScene
	{
	public:
		~SystemScene();

		/*!-----------------------------------------------------------
		//	@brief		インスタンス呼び出し
		//	@return		自身のポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		static	SystemScene*	GetInstance(){
			static SystemScene singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pFirstScene	最初に設定するシーン
		//	@param[in]  _hInst			インスタンスハンドル
		//	@param[in]	_wResolution	解像度（幅）[省略可]
		//	@param[in]	_hResolution	解像度（高さ）[省略可]
		//	@param[in]  _nWidth			ウィンドウ幅[省略可]
		//	@param[in]  _nHeight		ウィンドウ高さ[省略可]
		//	@param[in]	_renderFormat	レンダリングフォーマット[省略可]
		//	@param[in]  _isFullScreen	フルスクリーンフラグ[省略可]
		//  @param[in]  _pOverWindow	オーバーライドするウィンドウ[省略可]
		//	@return		HRESULTによる結果
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/28
		------------------------------------------------------------*/
		HRESULT	Initialize(
			SceneBase* _pFirstScene,
			const HINSTANCE _hInst,
			const UINT _wResolution = 640,
			const UINT _hResolution = 480,
			const UINT _nWidth = 640,
			const UINT _nHeight = 480,
			const DXGI_FORMAT _renderFormat = DXGI_FORMAT_R16G16B16A16_FLOAT,
			const BOOL _isFullScreen = FALSE,
			Window* const _pOverWindow = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウ乗っ取り
		//	@param[in]	_overrideWindow	オーバーライドするウィンドウ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		void	OverrideWindow(Window* _overrideWindow){
			this->useWindow = _overrideWindow;
		}

		/*!-----------------------------------------------------------
		//	@brief		終了処理
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		void	Finalize();

		/*!-----------------------------------------------------------
		//	@brief		実行
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/28
		------------------------------------------------------------*/
		void	Run();

	public:
		BOOL isKilled;
		BOOL isMsgUpdate;
		HANDLE hMsgThread;
		Window*	useWindow;
		ApiScene* pApiScene;
		ShaderScene* pShaderScene;
		SceneControlProxy* pSceneCtrlProxy;
		ImmdiateTaskSystem* pImmdiateTaskSystem;
		AoTaskSystem* pAoTaskSystem;
	private:
		/*!-----------------------------------------------------------
		//	@brief		メッセージ処理用のスレッド
		//	@param[in]  _argList	任意ポインタ
		//	@return		0：正常終了
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		static	unsigned WINAPI	TransactMessage(void* _argList);

		/*!-----------------------------------------------------------
		//	@brief		ゲーム処理
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/02
		------------------------------------------------------------*/
		inline	void	GameProcess();

	private:
		SystemScene();
		SystemScene(const SystemScene&) = delete;
		void operator=(const SystemScene&) = delete;
	};
}

#endif // _Include_SystemScene_hpp_