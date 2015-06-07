#include"stdafx.hpp"
#include"DirectX11Viewport.hpp"


using namespace ao;


DirectX11Viewport::DirectX11Viewport(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	DirectX11BaseApi(_pd3dDevice,_pd3dDeviceContext)
{

}


DirectX11Viewport::~DirectX11Viewport()
{

}


//------------------------------------------------------------------------------
/**	@brief		ì¬ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	02:29	T.Aoyama	ì¬
//------------------------------------------------------------------------------
void DirectX11Viewport::Create(
	CONST UINT _nWidth,
	CONST UINT _nHeight,
	CONST INT _topLeftX,
	CONST INT _topLeftY,
	CONST FLOAT _minDepth,
	CONST FLOAT _maxDepth
	)
{
	// D3D11_VIEWPORT
	this->viewport.TopLeftX = (FLOAT)_topLeftX;
	this->viewport.TopLeftY = (FLOAT)_topLeftY;
	this->viewport.Width = (FLOAT)_nWidth;
	this->viewport.Height = (FLOAT)_nHeight;
	this->viewport.MinDepth = _minDepth;
	this->viewport.MaxDepth = _maxDepth;
}