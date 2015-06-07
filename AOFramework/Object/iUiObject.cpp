#include"stdafx.hpp"
#include"iUiObject.hpp"
#include"../Define/SingletonTypeDefine.inl"
#include"../Shader/iRenderUi/iRenderUiShader.hpp"


using namespace ao;


iUiObject::iUiObject(
	ao::UiInstance* _pInstance,
	const ao::float2& _leftTop,
	const ao::float2& _rightTop,
	const ao::float2& _leftBottom,
	const ao::float2& _rightBottom) :
	pInstance(_pInstance), GameObject(_T("iUiObject")),
	uv(_leftTop, _rightTop, _leftBottom, _rightBottom)
{

}


iUiObject::~iUiObject()
{

}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/29	03:45	T.Aoyama	作成
//------------------------------------------------------------------------------
void iUiObject::Render(
	const int _x, const int _y,
	const float _w, const float _h,
	const float _degree,
	const float _r, const float _g, const float _b, const float _a)
{
	ao::sUiIRenderer()->PushRenderCommand(
		this->pInstance, _x, _y, _w, _h,
		this->uv.lt, this->uv.rt, this->uv.lb, this->uv.rb,
		_degree, _r, _g, _b, _a);
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/29	03:45	T.Aoyama	作成
//------------------------------------------------------------------------------
void iUiObject::Render(
	const int _x, const int _y,
	const float _w, const float _h,
	const float _degree,
	const float4& _color)
{
	ao::sUiIRenderer()->PushRenderCommand(
		this->pInstance, _x, _y, _w, _h,
		this->uv.lt, this->uv.rt, this->uv.lb, this->uv.rb,
		_degree, _color.r, _color.g, _color.b, _color.a);
}