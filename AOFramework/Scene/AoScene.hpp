/*************************************************************
*	@file   	AoScene.hpp
*	@brief  	AoScene関連のクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/07/31
*************************************************************/

#ifndef _Include_AoScene_hpp_	// インクルードガード
#define _Include_AoScene_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include<process.h>
#include<DirectXMath.h>
#include"../Math/Math.hpp"
#include"../Resource/LightingPackage.hpp"
#include"../Utility/NonCopyable.hpp"
#include"../Define/SystemDefine.hpp"


using namespace DirectX;


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  ISceneBase
	//	@brief  シーンのインターフェースクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/31
	------------------------------------------------------------*/
	class ISceneBase : private NonCopyable
	{
	public:
		ISceneBase(){};
		virtual	~ISceneBase()override{};

		/*!-----------------------------------------------------------
		//	@brief		ロード前に実行する
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		virtual	void	InitPreLoad() = 0;

		/*!-----------------------------------------------------------
		//	@brief		ロードスレッド
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		virtual	void	LoadThread() = 0;

		/*!-----------------------------------------------------------
		//	@brief		シーン実行前にコールされる
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		virtual	void	StartCall() = 0;

		/*!-----------------------------------------------------------
		//	@brief		更新
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		virtual	void	Update() = 0;

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		virtual	void	Render() = 0;

		/*!-----------------------------------------------------------
		//	@brief		フレームスキップ発生
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/16
		------------------------------------------------------------*/
		virtual void	FrameSkip() = 0;

		/*!-----------------------------------------------------------
		//	@brief		シーン名取得
		//	@return		シーン名
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/27
		------------------------------------------------------------*/
		virtual AOLPCSTR GetSceneName() = 0;

	protected:
		/*!-----------------------------------------------------------
		//	@brief		派生先の更新
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void	OnStartCall() = 0;

		/*!-----------------------------------------------------------
		//	@brief		派生先の更新
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void	OnUpdate() = 0;

		/*!-----------------------------------------------------------
		//	@brief		派生先のレンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void	OnRender() = 0;
	};


	/*!-----------------------------------------------------------
	//	@class  ILoadScene
	//	@brief  ロード用のシーンインターフェースクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/12/21
	------------------------------------------------------------*/
	class ILoadScene : private NonCopyable
	{
	public:
		ILoadScene(){}
		virtual ~ILoadScene()override{}

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void Initialize() = 0;

		/*!-----------------------------------------------------------
		//	@brief		更新
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void Update() = 0;

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void Render() = 0;

		/*!-----------------------------------------------------------
		//	@brief		シーン名取得
		//	@return		シーン名
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/27
		------------------------------------------------------------*/
		virtual AOLPCSTR GetSceneName() = 0;
	};


	/*!-----------------------------------------------------------
	//	@class  SceneBase
	//	@brief  シーンの基底クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/17
	------------------------------------------------------------*/
	class SceneBase : public ISceneBase
	{
	public:
		SceneBase(AOLPCSTR _sceneName) :
			isLoadEnd(TRUE),
			isStart(FALSE),
			enableFrameSkip(TRUE),
			enableUpdate(TRUE),
			enableRender(TRUE),
			lightDir(0, -1, 0),
			eyePos(0, 0, 0),
			clearColor(0, 0, 0, 0),
			sceneName(_sceneName),
			pLoadScene(nullptr)
		{
			ao::MatrixIdentity(&viewMatrix);
			ao::MatrixIdentity(&projMatrix);
		};
		virtual ~SceneBase()override{};

		/*!-----------------------------------------------------------
		//	@brief		ロード前に実行する
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		virtual	void	InitPreLoad()override{};

		/*!-----------------------------------------------------------
		//	@brief		ロードスレッド
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		virtual	void	LoadThread()override{};

		/*!-----------------------------------------------------------
		//	@brief		シーン実行前にコールされる
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	StartCall() final override;

		/*!-----------------------------------------------------------
		//	@brief		更新
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	Update() final override;

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	Render() final override;

		/*!-----------------------------------------------------------
		//	@brief		フレームスキップ発生
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/16
		------------------------------------------------------------*/
		virtual void	FrameSkip()override{};

		/*!-----------------------------------------------------------
		//	@brief		射影行列初期化
		//	@param[in]	_pProjMtx	射影行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	InitializeProjectionMatrix(const XMFLOAT4X4* const _pProjMtx);

		/*!-----------------------------------------------------------
		//	@brief		ビュー行列更新
		//	@param[in]	_pViewMtx	ビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	UpdateViewMatrix(const XMFLOAT4X4* const _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		ライトビュー行列更新
		//	@param[in]	_pLightViewMtx	ライトビュー行列
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/19
		------------------------------------------------------------*/
		void	UpdateLightViewMatrix(const XMFLOAT4X4* const _pLightViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		ライト方向定義
		//	@param[in]	_x			X方向
		//  @param[in]  _y			Y方向
		//  @param[in]  _z			Z方向
		//  @param[in]  _ambient	環境光の強さ[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	UpdateLightDirection(float _x, float _y, float _z,float _ambient=0.5f);

		/*!-----------------------------------------------------------
		//	@brief		視点座標更新
		//	@param[in]	_x				X座標
		//  @param[in]  _y				Y座標
		//  @param[in]  _z				Z座標
		//	@param[in]	_specularPower	スペキュラパワー
		//	@param[in]	_specularRange	スペキュラ範囲
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	UpdateEyePosition(float _x, float _y, float _z,float _specularPower=1.0f,int _specularRange=10);

		/*!-----------------------------------------------------------
		//	@brief		クリアカラーを設定
		//	@param[in]	_r	赤色[省略可]
		//	@param[in]	_g	緑色[省略可]
		//	@param[in]	_b	青色[省略可]
		//	@param[in]	_a	透過率[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		void	SetClearColor(float _r = 0, float _g = 0, float _b = 0, float _a = 0);

		/*!-----------------------------------------------------------
		//	@brief		ロードシーンを設定
		//	@param[in]	_pPushLoadScene	ロードシーン
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		void PushLoadScene(ILoadScene* _pPushLoadScene);

		/*!-----------------------------------------------------------
		//	@brief		シーン名取得
		//	@return		シーン名
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/27
		------------------------------------------------------------*/
		virtual AOLPCSTR GetSceneName() override
		{
			return this->sceneName.c_str();
		}

	public:
		BOOL isLoadEnd;
		BOOL isStart;
		BOOL enableUpdate;
		BOOL enableRender;
		BOOL enableFrameSkip;
		XMFLOAT3 lightDir;
		XMFLOAT3 eyePos;
		XMFLOAT4 clearColor;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 lightViewMatrix;
		XMFLOAT4X4 projMatrix;
		string sceneName;
		ILoadScene* pLoadScene;

	protected:
		/*!-----------------------------------------------------------
		//	@brief		派生先の最初のコール
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		virtual void OnStartCall()override{};

		/*!-----------------------------------------------------------
		//	@brief		派生先の更新
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void OnUpdate()override{};

		/*!-----------------------------------------------------------
		//	@brief		派生先のレンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void OnRender()override{};

	private:
		/*!-----------------------------------------------------------
		//	@brief		レンダリング開始
		//  @param[in]  _r	赤色[省略可]
		//  @param[in]  _g	緑色[省略可]
		//  @param[in]  _b	青色[省略可]
		//  @param[in]  _a	透過率[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/07
		------------------------------------------------------------*/
		void RenderBegin();

		/*!-----------------------------------------------------------
		//	@brief		レンダリング終了
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/07
		------------------------------------------------------------*/
		void RenderEnd();
	};
}


#endif // _Include_AoScene_hpp_
