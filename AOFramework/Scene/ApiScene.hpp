/*************************************************************
*	@file   	ApiScene.hpp
*	@brief  	ApiSceneクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_ApiScene_hpp_	// インクルードガード
#define _Include_ApiScene_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"SystemComposite.hpp"
#include"../Api/DirectX11Application.hpp"

namespace ao
{
	class SystemScene;

	/*!-----------------------------------------------------------
	//	@class  ApiScene
	//	@brief  Api専用のシーン
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class ApiScene : public SystemComposite
	{
	public:
		ApiScene(SystemScene* _pSystemScene);
		~ApiScene()override final;

	public:
		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_hWnd			ウィンドウハンドル
		//	@param[in]	_wResolution	解像度（幅）[省略可]
		//	@param[in]	_hResolution	解像度（高さ）[省略可]
		//	@param[in]  _nWidth			ウィンドウ幅[省略可]
		//	@param[in]  _nHeight		ウィンドウ高さ[省略可]
		//	@param[in]	_renderFormat	レンダリングフォーマット[省略可]
		//	@param[in]  _isFullScreen	フルスクリーンフラグ[省略可]
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		HRESULT Initialize(
			const HWND _hWnd,
			const UINT _wResolution = 640,
			const UINT _hResolution = 480,
			const UINT _nWidth = 640,
			const UINT _nHeight = 480,
			const DXGI_FORMAT _renderFormat = DXGI_FORMAT_R16G16B16A16_FLOAT,
			const BOOL _isFullScreen = FALSE);

	public:
		class DirectX11Application			application;
		class DirectX11Resource*			pResource;
		class DirectX11Shader*				pShader;
		class DirectX11GraphicsRenderer*	pRenderer;
		class DirectX11GraphicsState*		pGraphicsState;
		class DirectX11DebugFont*			pDebugFont;
		class DirectX11Profiler*			pProfiler;
	};
}

#endif // _Include_ApiScene_hpp_