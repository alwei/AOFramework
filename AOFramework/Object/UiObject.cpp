#include"stdafx.hpp"
#include"UiObject.hpp"
#include"../Define/SingletonTypeDefine.inl"
#include"../Shader/RenderUi/RenderUiShader.hpp"


using namespace ao;


UiObject::UiObject() :
pTexture(nullptr),
GameObject(_T("UiObject"))
{

}


UiObject::~UiObject()
{

}


//------------------------------------------------------------------------------
/**	@brief		リソース読み込み */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/02/10	11:42	T.Aoyama	作成
//------------------------------------------------------------------------------
void UiObject::LoadResource(const ao::string& _filePath, SamplerState _filter)
{
	this->pTexture = ao::sResource()->LoadTextureFromFile(_filePath, _filter);
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/02/10	11:42	T.Aoyama	作成
//------------------------------------------------------------------------------
void UiObject::Render(
	const int _x, const int _y,
	const float _w, const float _h,
	const float _degree,
	const float _r, const float _g, const float _b, const float _a)
{
	ao::sUiRenderer()->Render(this->pTexture, _x, _y, _w, _h, _degree, _r, _g, _b, _a);
}


//------------------------------------------------------------------------------
/**	@brief		レンダリング */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/02	19:41	T.Aoyama	作成
//------------------------------------------------------------------------------
void UiObject::Render(
	const int _x, const int _y,
	const float _w,const float _h,
	const float _degree,
	const float4& _color)
{
	ao::sUiRenderer()->Render(
		this->pTexture, _x, _y, _w, _h, _degree,
		_color.r, _color.g, _color.b, _color.a,
		ao::BlendState::Alpha,ao::FillState::Solid);
}