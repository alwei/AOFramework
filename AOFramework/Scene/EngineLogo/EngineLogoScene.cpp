#include"stdafx.hpp"
#include"EngineLogoScene.hpp"
#include"../../System/FpsController.hpp"
#include"../../Define/SingletonTypeDefine.inl"
#include"../../Window/Window.hpp"
#include"../../Scene/SceneControlProxy.hpp"
#include"../../Api/DirectX11GraphicsRenderer.hpp"
#include"../../Api/DirectX11Resource.hpp"
#include"../../Shader/RenderSprite/RenderSpriteShader.hpp"
#include"../../Shader/iRenderSprite/iRenderSpriteShader.hpp"
#include"../../Shader/RenderUi/RenderUiShader.hpp"


using namespace ao;


EngineLogoScene::EngineLogoScene() :
nowState(State::FadeStandby),
isSkip(FALSE),
pMainScene(nullptr),
ao::SceneBase(_T("EngineLogo"))
{

}


EngineLogoScene::~EngineLogoScene()
{
	if (this->pMainScene)
	{
		delete this->pMainScene;
	}
}


//------------------------------------------------------------------------------
/**	@brief		ロード前の初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/15	17:20	T.Aoyama	作成
//------------------------------------------------------------------------------
void EngineLogoScene::InitPreLoad()
{
	this->InitializeProjectionMatrix(ao::sRenderer()->GetProjMatrix());
	this->engineLogo = ao::sResource()->LoadSpriteFromFile(_T("Default/Logo/EngineLogo.png"));

	this->msTime.SetParameter(1.0f);
}


//------------------------------------------------------------------------------
/**	@brief		ロードスレッド */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/15	17:20	T.Aoyama	作成
//------------------------------------------------------------------------------
void EngineLogoScene::LoadThread()
{

}


//------------------------------------------------------------------------------
/**	@brief		開始コール */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/15	17:20	T.Aoyama	作成
//------------------------------------------------------------------------------
void EngineLogoScene::OnStartCall()
{

}


//------------------------------------------------------------------------------
/**	@brief		更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/15	17:20	T.Aoyama	作成
//------------------------------------------------------------------------------
void EngineLogoScene::OnUpdate()
{
	DWORD skipBit = 0;
	skipBit |= sXInput()->GetTrgButton(0, JoypadId::A);
	skipBit |= sXInput()->GetTrgButton(0, JoypadId::START);
	skipBit |= sKeyboard()->GetTrgKey(KeyCode::RETURN);

	if (skipBit)
	{
		this->isSkip = TRUE;
		sSceneControlProxy()->LoadLevel(this->pMainScene);
		this->pMainScene = nullptr;
		return;
	}


	this->msTime.Update();

	switch (this->nowState)
	{
	case State::FadeStandby:
		this->logoAlpha = 0;
		if (this->msTime.IsEnd()){
			this->nowState = State::FadeIn;
			this->msTime.SetParameter(2.0f);
		}
		break;
	case State::FadeIn:
		this->logoAlpha = this->msTime.GetNormalizedTime();
		if (this->msTime.IsEnd()){
			this->nowState = State::FadeNow;
			this->msTime.SetParameter(3.0f);
		}
		break;
	case State::FadeNow:
		this->logoAlpha = 1;
		if (this->msTime.IsEnd()){
			this->nowState = State::FadeOut;
			this->msTime.SetParameter(2.0f);
		}
		break;
	case State::FadeOut:
		this->logoAlpha = 1-this->msTime.GetNormalizedTime();
		if (this->msTime.IsEnd())
		{
			this->nowState = State::FadeEnd;
			sSceneControlProxy()->LoadLevel(this->pMainScene);
			this->pMainScene = nullptr;
		}
		break;
	}
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/15	17:20	T.Aoyama	作成
//------------------------------------------------------------------------------
void EngineLogoScene::OnRender()
{
	if (this->isSkip)
	{
		return;
	}

	float w = 640 * ao::sWindow()->GetClientWidth()/1280.0f;
	float h = 64 * ao::sWindow()->GetClientHeight()/720.0f;
	ao::sUiRenderer()->Render(
		this->engineLogo,
		0,0,w, h,0,
		1,1,1,this->logoAlpha,
		ao::BlendState::Alpha,
		ao::FillState::Solid);

	//レンダラー登録
	ao::sRenderer()->RenderPushback(ao::sSpriteRenderer());
	ao::sRenderer()->RenderPushback(ao::sSpriteIRenderer());

	//ポストレンダリング登録
	ao::sRenderer()->PostRenderPushback(ao::sUiRenderer());
}