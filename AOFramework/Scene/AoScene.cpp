#include"stdafx.hpp"
#include"AoScene.hpp"
#include"../System/FpsController.hpp"
#include"../Define/SingletonTypeDefine.inl"
#include"../Api/DirectX11GraphicsRenderer.hpp"
#include"../Api/DirectX11DebugFont.hpp"
#include"../Api/DirectX11Profiler.hpp"
#include"../Shader/RenderMesh/RenderMeshShader.hpp"
#include"../Shader/iRenderMesh/iRenderMeshShader.hpp"
#include"../Shader/RenderMeshAnimation/RenderMeshAnimationShader.hpp"
#include"../Shader/RenderSprite/RenderSpriteShader.hpp"
#include"../Shader/iRenderSprite/iRenderSpriteShader.hpp"
#include"../Shader/GpuMeshParticle/GpuMeshParticleShader.hpp"
#include"../Shader/ZPrePass/ZPrePassShader.hpp"
#include"../Shader/CrossFilter/CrossFilterShader.hpp"
#include"../Shader/LightBloom/LightBloomShader.hpp"
#include"../Shader/ExtractBrightness/ExtractBrightnessShader.hpp"


using namespace ao;


//------------------------------------------------------------------------------
/**	@brief		�ˉe�s�񏉊��� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/17	09:33	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneBase::InitializeProjectionMatrix(const XMFLOAT4X4* const _pProjMtx)
{
	::CopyMemory(&this->projMatrix, _pProjMtx, sizeof(XMFLOAT4X4));
}


//------------------------------------------------------------------------------
/**	@brief		�r���[�s�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/17	09:13	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneBase::UpdateViewMatrix(const XMFLOAT4X4* const _pViewMtx)
{
	ao::sRenderer()->UpdateViewMatrix(_pViewMtx);
	ao::sMeshRenderer()->UpdateViewMatrix(_pViewMtx);
	ao::sMeshIRenderer()->UpdateViewMatrix(_pViewMtx);
	ao::sMeshARenderer()->UpdateViewMatrix(_pViewMtx);
	ao::sSpriteRenderer()->UpdateViewMatrix(_pViewMtx);
	ao::sSpriteIRenderer()->UpdateViewMatrix(_pViewMtx);
	ao::sGpuMeshParticleRenderer()->UpdateViewMatrix(_pViewMtx);
	::CopyMemory(&this->viewMatrix, _pViewMtx, sizeof(XMFLOAT4X4));
}


//------------------------------------------------------------------------------
/**	@brief		���C�g�r���[�s��X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/19	00:44	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneBase::UpdateLightViewMatrix(const XMFLOAT4X4* const _pLightViewMtx)
{
	ao::sDepthRenderer()->UpdateViewMatrix(_pLightViewMtx);
	ao::sMeshRenderer()->UpdateLightViewMatrix(_pLightViewMtx);
	ao::sMeshIRenderer()->UpdateLightViewMatrix(_pLightViewMtx);
	ao::sMeshARenderer()->UpdateLightViewMatrix(_pLightViewMtx);
	::CopyMemory(&this->lightViewMatrix, _pLightViewMtx, sizeof(XMFLOAT4X4));
}


//------------------------------------------------------------------------------
/**	@brief		���C�g������` */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/17	09:16	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneBase::UpdateLightDirection(float _x, float _y, float _z, float _ambient)
{
	this->lightDir.x = _x;
	this->lightDir.y = _y;
	this->lightDir.z = _z;

	ao::Vector3Normalize(&this->lightDir, this->lightDir);

	ao::sMeshRenderer()->UpdateLightDirection(this->lightDir.x, this->lightDir.y, this->lightDir.z, _ambient);
	ao::sMeshIRenderer()->UpdateLightDirection(this->lightDir.x, this->lightDir.y, this->lightDir.z, _ambient);
	ao::sMeshARenderer()->UpdateLightDirection(this->lightDir.x, this->lightDir.y, this->lightDir.z, _ambient);
}


//------------------------------------------------------------------------------
/**	@brief		�i�W���W�X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/17	09:17	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneBase::UpdateEyePosition(float _x, float _y, float _z,float _specularPower,int _specularRange)
{
	this->eyePos.x = _x;
	this->eyePos.y = _y;
	this->eyePos.z = _z;

	ao::sMeshRenderer()->UpdateEyePosition(this->eyePos.x, this->eyePos.y, this->eyePos.z, _specularPower, _specularRange);
	ao::sMeshIRenderer()->UpdateEyePosition(this->eyePos.x, this->eyePos.y, this->eyePos.z, _specularPower);
	ao::sMeshARenderer()->UpdateEyePosition(this->eyePos.x, this->eyePos.y, this->eyePos.z, _specularPower);
}


//------------------------------------------------------------------------------
/**	@brief		�N���A�J���[��ڑ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/21	15:21	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneBase::SetClearColor(float _r,float _g,float _b,float _a)
{
	this->clearColor.x = _r;
	this->clearColor.y = _g;
	this->clearColor.z = _b;
	this->clearColor.w = _a;
}


//------------------------------------------------------------------------------
/**	@brief		���[�h�V�[����ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/21	16:41	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneBase::PushLoadScene(ILoadScene* _pPushLoadScene)
{
	this->isLoadEnd = FALSE;
	this->pLoadScene = _pPushLoadScene;


	ao::sProfiler()->Begin(ProcessLoadType::INIT);
	this->pLoadScene->Initialize();
	ao::sProfiler()->End(ProcessLoadType::INIT);
}


//------------------------------------------------------------------------------
/**	@brief		�X�^�[�g���̃R�[�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/01	02:48	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneBase::StartCall()
{
	this->OnStartCall();

	this->isStart = TRUE;
}


//------------------------------------------------------------------------------
/**	@brief		�X�V���� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/21	15:24	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneBase::Update()
{
	if (this->pLoadScene)
	{
		ao::sProfiler()->Begin(ProcessLoadType::LOAD);
		this->pLoadScene->Update();
		ao::sProfiler()->End(ProcessLoadType::LOAD);
		if (this->isLoadEnd)
		{
			this->pLoadScene = nullptr;
		}
		return;
	}

	ao::sProfiler()->Begin(ProcessLoadType::UPDATE);
	this->OnUpdate();
	ao::sProfiler()->End(ProcessLoadType::UPDATE);
}


//------------------------------------------------------------------------------
/**	@brief		�����_�����O */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/21	15:10	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneBase::Render()
{
	if (this->pLoadScene)
	{
		this->pLoadScene->Render();
		return;
	}

	this->RenderBegin();
	this->OnRender();
	this->RenderEnd();
}


//------------------------------------------------------------------------------
/**	@brief		�����_�����O�J�n */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/07	11:10	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneBase::RenderBegin()
{
	ao::sProfiler()->Begin(ProcessLoadType::RENDER_BURST);

	ao::sRenderer()->RenderBegin(
		this->clearColor.x,
		this->clearColor.y,
		this->clearColor.z,
		this->clearColor.w);

	ao::sLightBloomRender()->Clear();
	ao::sCrossFilterRender()->Clear();
	ao::sExtractBrightnessRender()->Clear();
	ao::sDepthRenderer()->Clear();
	ao::sMeshIRenderer()->Clear();
	ao::sSpriteIRenderer()->Clear();
}


//------------------------------------------------------------------------------
/**	@brief		�����_�����O�I�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/07	11:11	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void SceneBase::RenderEnd()
{
	//�o�[�X�g�����_�����O
	ao::sRenderer()->PreRenedrBurst();
	ao::sRenderer()->RenderBurst();
	ao::sRenderer()->PostRenderBurst();

	ao::sProfiler()->End(ProcessLoadType::RENDER_BURST);

	//�v���t�@�C�����ʕ`��
	ao::sProfiler()->RenderResult();

	//�f�o�b�O�t�H���g�`��
	ao::sDebugFont()->RenderBurst();

	ao::sProfiler()->Begin(ProcessLoadType::RENDER_PRESENT);
	ao::sRenderer()->RenderEnd();
	ao::sProfiler()->End(ProcessLoadType::RENDER_PRESENT);

	ao::sProfiler()->Begin(ProcessLoadType::RENDER_CLEAR);
	ao::sRenderer()->ClearRender();
	ao::sProfiler()->End(ProcessLoadType::RENDER_CLEAR);
}