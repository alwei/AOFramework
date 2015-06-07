#include"stdafx.hpp"
#include"CutSprite.hpp"
#include"../Debug/DebugSystem.hpp"


struct CutSpriteVertex
{
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};


static CutSpriteVertex CornerBuffer[4] =
{
	{ 0 + 0.0f, 0 + 0.0f, 0, 0, 0, -1, 0, 0 },
	{ 0 + 1.0f, 0 + 0.0f, 0, 0, 0, -1, 1, 0 },
	{ 0 + 0.0f, 0 - 1.0f, 0, 0, 0, -1, 0, 1 },
	{ 0 + 1.0f, 0 - 1.0f, 0, 0, 0, -1, 1, 1 }
};


static CutSpriteVertex CenterBuffer[4] =
{
	{ -0.5f, +0.5f, 0.f, 0, 0, -1, 0, 0 },
	{ +0.5f, +0.5f, 0.f, 0, 0, -1, 1, 0 },
	{ -0.5f, -0.5f, 0.f, 0, 0, -1, 0, 1 },
	{ +0.5f, -0.5f, 0.f, 0, 0, -1, 1, 1 }
};


using namespace ao;


//------------------------------------------------------------------------------
/**	@brief		頂点バッファ作成(左上原点) */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/07	02:14	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT CutSprite::CreateCornerCutVertexBuffer(
	ID3D11Device* _pd3dDevice,
	const float2& _leftTop,
	const float2& _rightTop,
	const float2& _leftButtom,
	const float2& _rightButtom)
{
	return this->CreateCornerCutVertexBuffer(
		_pd3dDevice,
		float2(_leftTop),
		float2(_rightTop),
		float2(_leftButtom),
		float2(_rightButtom));
}


//------------------------------------------------------------------------------
/**	@brief		頂点バッファ作成(中心原点) */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/07	02:14	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT CutSprite::CreateCenterCutVertexBuffer(
	ID3D11Device* const _pd3dDevice,
	const float2& _leftTop,
	const float2& _rightTop,
	const float2& _leftButtom,
	const float2& _rightButtom)
{
	return this->CreateCenterCutVertexBuffer(
		_pd3dDevice,
		float2(_leftTop),
		float2(_rightTop),
		float2(_leftButtom),
		float2(_rightButtom));
}


//------------------------------------------------------------------------------
/**	@brief		頂点バッファ作成（左上原点） */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/03/13	13:05	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT CutSprite::CreateCornerCutVertexBuffer(
	ID3D11Device* _pd3dDevice,
	const float2&& _leftTop,
	const float2&& _rightTop,
	const float2&& _leftButtom,
	const float2&& _rightButtom)
{
	this->pivotType = SpritePivot::LeftTop;

	CornerBuffer[(int)SpritePivot::LeftTop].u = _leftTop.x;
	CornerBuffer[(int)SpritePivot::LeftTop].v = _leftTop.y;
	CornerBuffer[(int)SpritePivot::RightTop].u = _rightTop.x;
	CornerBuffer[(int)SpritePivot::RightTop].v = _rightTop.y;
	CornerBuffer[(int)SpritePivot::LeftButtom].u = _leftButtom.x;
	CornerBuffer[(int)SpritePivot::LeftButtom].v = _leftButtom.y;
	CornerBuffer[(int)SpritePivot::RightButtom].u = _rightButtom.x;
	CornerBuffer[(int)SpritePivot::RightButtom].v = _rightButtom.y;

	HRESULT hr(E_FAIL);
	hr = this->CreateCommonVertexBuffer(
		this->pGpuVertexBuffer.ToCreator(),
		_pd3dDevice,
		sizeof(CornerBuffer),
		CornerBuffer);

	if (FAILED(hr)){
		ao::debug::PrintfColor(
			ConsoleColor::H_RED,
			_T("[CutSprite::CreateCornerCutVertexBuffer]\nFailed create corner cut sprite vertex buffer.\n"));
		return hr;
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		頂点バッファ作成（中心原点） */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/03/13	13:08	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT CutSprite::CreateCenterCutVertexBuffer(
	ID3D11Device* _pd3dDevice,
	const float2&& _leftTop,
	const float2&& _rightTop,
	const float2&& _leftButtom,
	const float2&& _rightButtom)
{
	this->pivotType = SpritePivot::Center;

	CenterBuffer[(int)SpritePivot::LeftTop].u = _leftTop.x;
	CenterBuffer[(int)SpritePivot::LeftTop].v = _leftTop.y;
	CenterBuffer[(int)SpritePivot::RightTop].u = _rightTop.x;
	CenterBuffer[(int)SpritePivot::RightTop].v = _rightTop.y;
	CenterBuffer[(int)SpritePivot::LeftButtom].u = _leftButtom.x;
	CenterBuffer[(int)SpritePivot::LeftButtom].v = _leftButtom.y;
	CenterBuffer[(int)SpritePivot::RightButtom].u = _rightButtom.x;
	CenterBuffer[(int)SpritePivot::RightButtom].v = _rightButtom.y;

	HRESULT hr(E_FAIL);
	hr = this->CreateCommonVertexBuffer(
		this->pGpuVertexBuffer.ToCreator(),
		_pd3dDevice,
		sizeof(CenterBuffer),
		CenterBuffer);

	if (FAILED(hr)){
		ao::debug::PrintfColor(
			ConsoleColor::H_RED,
			_T("[CutSprite::CreateCornerCutVertexBuffer]\nFailed create corner cut sprite vertex buffer.\n"));
		return hr;
	}

	return S_OK;
}