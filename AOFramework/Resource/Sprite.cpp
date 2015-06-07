#include"stdafx.hpp"
#include"Sprite.hpp"
#include"../Shader/Function/ShaderFunction.hpp"
#include"../Debug/DebugSystem.hpp"

using namespace ao;


static const float LeftTopBuffer[3][8] =
{
	{ 0 + 0.0f, 0 + 0.0f, 0, 0, 0, -1, 0.0f, 0.0f },
	{ 0 + 2.0f, 0 + 0.0f, 0, 0, 0, -1, 2.0f, 0.0f },
	{ 0 + 0.0f, 0 - 2.0f, 0, 0, 0, -1, 0.0f, 2.0f },
};

static const float CenterBuffer[3][8] =
{
	{ -0.5f, +0.5f, 0.f, 0, 0, -1, 0.0f, 0.0f },
	{ +1.5f, +0.5f, 0.f, 0, 0, -1, 2.0f, 0.0f },
	{ -0.5f, -1.5f, 0.f, 0, 0, -1, 0.0f, 2.0f }
};


Sprite::Sprite() : vBufferSize(0), pTexture(nullptr)
{

}


//------------------------------------------------------------------------------
/**	@brief		頂点バッファ作成(左上原点) */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/07	02:14	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT Sprite::CreateCornerVertexBuffer(ID3D11Device* _pd3dDevice)
{
	this->pivotType = SpritePivot::LeftTop;

	HRESULT hr(E_FAIL);
	hr = this->CreateCommonVertexBuffer(
		this->pGpuVertexBuffer.ToCreator(),
		_pd3dDevice,
		sizeof(LeftTopBuffer),
		LeftTopBuffer);

#ifdef __AO_DEBUG__
	if (FAILED(hr))
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[Sprite::CreateCornerVertexBuffer]\n"));
#endif

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		頂点バッファ作成(中心原点) */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/07	02:14	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT Sprite::CreateCenterVertexBuffer(ID3D11Device* const _pd3dDevice)
{
	this->pivotType = SpritePivot::Center;

	HRESULT hr(E_FAIL);
	hr = this->CreateCommonVertexBuffer(
		this->pGpuVertexBuffer.ToCreator(),
		_pd3dDevice,
		sizeof(CenterBuffer),
		CenterBuffer);

#ifdef __AO_DEBUG__
	if (FAILED(hr))
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[Sprite::CreateCornerVertexBuffer]\n"));
#endif

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		確保した頂点データを取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/03/08	15:08	T.Aoyama	作成
//------------------------------------------------------------------------------
const void* Sprite::GetVertexBufferData(void* const _pOutBufferData)
{
	if (_pOutBufferData == nullptr)
	{
		switch (this->pivotType)
		{
		case SpritePivot::LeftTop:
			return LeftTopBuffer;
			break;
		case SpritePivot::Center:
			return CenterBuffer;
			break;
		}

		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[Sprite::GetVertexBufferData]\nType of the specified sprite is not supported.\n"));
		return nullptr;
	}

	switch (this->pivotType)
	{
	case SpritePivot::LeftTop:
		::CopyMemory(_pOutBufferData, LeftTopBuffer, sizeof(LeftTopBuffer));
		break;
	case SpritePivot::Center:
		::CopyMemory(_pOutBufferData, CenterBuffer, sizeof(CenterBuffer));
		break;
	}

	return _pOutBufferData;
}


//------------------------------------------------------------------------------
/**	@brief		共通の頂点バッファ作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/07	02:40	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT Sprite::CreateCommonVertexBuffer(
	ID3D11Buffer** _ppOutVBuffer,
	ID3D11Device* const _pd3dDevice,
	const UINT _vBufferSize,
	const void* const _pSrcData)
{
	this->vBufferSize = _vBufferSize;

	HRESULT hr(E_FAIL);
	hr = ao::CreateVertexBuffer(
		_pd3dDevice,
		_ppOutVBuffer,
		_vBufferSize,
		_pSrcData);
#ifdef __AO_DEBUG__
	if (FAILED(hr))
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[Sprite::CreateCommonVertexBuffer]\nFailed create sprite vertex buffer.\n"));
#endif

	return hr;
}