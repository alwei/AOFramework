/*************************************************************
*	@file   	ShaderScene.hpp
*	@brief  	ShaderSceneクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_ShaderScene_hpp_	// インクルードガード
#define _Include_ShaderScene_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<d3d11.h>
#include"SystemComposite.hpp"
#include"../Utility/SmartComPtr.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  ShaderScene
	//	@brief  シェーダー専用のシーン
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class ShaderScene : public SystemComposite
	{
	public:
		ShaderScene(class SystemScene* _pSystemScene);
		~ShaderScene();

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pd3dDevice			デバイスポインタ
		//	@param[in]	_pd3dDeviceContext	デバイスコンテキスト
		//	@param[in]	_pGraphicsState		グラフィックステート
		//	@param[in]	_pRenderer			レンダラー
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		HRESULT	Initialize(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext,
			class DirectX11GraphicsState* _pGraphicsState,
			class DirectX11GraphicsRenderer* _pRenderer);

	public:
		class RenderUiShader*				pRenderUiShader;
		class iRenderUiShader*				pIRenderUiShader;
		class RenderMeshShader*				pRenderMeshShader;
		class iRenderMeshShader*			pIRenderMeshShader;
		class RenderMeshAnimationShader*	pRenderMeshAnimationShader;
		class RenderSpriteShader*			pRenderSpriteShader;
		class iRenderSpriteShader*			pIRenderSpriteShader;
		class ZPrePassShader*				pRenderDepthShader;
		class ExtractBrightnessShader*		pExtractBrightnessShader;
		class GaussianFilterShader*			pGaussianFilterShader;
		class LightBloomShader*				pLightBloomShader;
		class CrossFilterShader*			pCrossFilterShader;
		class FXAAShader*					pFxaaShader;
		class GpuMeshParticleShader*		pGpuMeshParticleShader;
	};
}

#endif // _Include_ShaderScene_hpp_