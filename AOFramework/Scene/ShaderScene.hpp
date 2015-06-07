/*************************************************************
*	@file   	ShaderScene.hpp
*	@brief  	ShaderScene�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_ShaderScene_hpp_	// �C���N���[�h�K�[�h
#define _Include_ShaderScene_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<d3d11.h>
#include"SystemComposite.hpp"
#include"../Utility/SmartComPtr.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  ShaderScene
	//	@brief  �V�F�[�_�[��p�̃V�[��
	//	@author	Tatsunori Aoyama
	//	@date	2015/02/28
	------------------------------------------------------------*/
	class ShaderScene : public SystemComposite
	{
	public:
		ShaderScene(class SystemScene* _pSystemScene);
		~ShaderScene();

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	_pd3dDevice			�f�o�C�X�|�C���^
		//	@param[in]	_pd3dDeviceContext	�f�o�C�X�R���e�L�X�g
		//	@param[in]	_pGraphicsState		�O���t�B�b�N�X�e�[�g
		//	@param[in]	_pRenderer			�����_���[
		//	@return		S_OK�F�����������@����ȊO�F���������s
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