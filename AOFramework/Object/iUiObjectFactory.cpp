#include"stdafx.hpp"
#include"iUiObject.hpp"
#include"iUiObjectFactory.hpp"
#include"iUiObjectExecutor.hpp"
#include"../Define/SingletonTypeDefine.inl"
#include"../Shader/iRenderUi/iRenderUiShader.hpp"


using namespace ao;


iUiObjectFactory::iUiObjectFactory() :
pInstance(nullptr),
pExecutor(nullptr)
{

}


iUiObjectFactory::~iUiObjectFactory()
{
	this->DeleteBuffer();
}


//------------------------------------------------------------------------------
/**	@brief		アトラスのインスタンス作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/29	04:54	T.Aoyama	作成
//------------------------------------------------------------------------------
iUiObjectExecutor* iUiObjectFactory::CreateInstance(
	const ao::string _filePath,
	const UINT _numAlloc,
	const SamplerState _sampState)
{
	//バッファが存在していなら解放
	this->DeleteBuffer();

	//アトラスとなるテクスチャ読み込み
	BaseTexture* pTexture = ao::sResource()->LoadTextureFromFile(_filePath, _sampState);
	this->pInstance = ao::sUiIRenderer()->CreateInstance(pTexture, _numAlloc);

#ifdef __AO_DEBUG__
	if (this->pInstance == nullptr){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[iUiObjectFactory::LoadAtlasMap]Failed create instance.\n"));
		return nullptr;
	}
#endif

	//レンダリング実行者を作成
	this->pExecutor = new iUiObjectExecutor(this->pInstance);

	return this->pExecutor;
}


//------------------------------------------------------------------------------
/**	@brief		UIInstansオブジェクト作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/29	05:02	T.Aoyama	作成
//------------------------------------------------------------------------------
iUiObject* iUiObjectFactory::CreateObject(
	const float2& _leftTop,
	const float2& _rightTop,
	const float2& _leftBottom,
	const float2& _rightBottom)
{
#ifdef __AO_DEBUG__
	if (this->pInstance == nullptr){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[iUiObjectFactory::CreateObject]Not yet been loaded Atlas.\n"));
		return nullptr;
	}
#endif

	iUiObject* pUiObject = new iUiObject(this->pInstance, _leftTop, _rightTop, _leftBottom, _rightBottom);
	this->pBufferList.push_back(pUiObject);

	return pUiObject;
}


//------------------------------------------------------------------------------
/**	@brief		UIInstansオブジェクト作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/29	05:02	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iUiObjectFactory::CreateObjectArray(
	iUiObject** const _ppOutArray,
	const int _xNum,const int _yNum,
	const int _xSize,const int _ySize,
	const int _atlasWidth,const int _atlasHeight)
{
#ifdef __AO_DEBUG__
	if (this->pInstance == nullptr){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[iUiObjectFactory::CreateObject]Not yet been loaded Atlas.\n"));
		return E_FAIL;
	}
#endif

	const float xDiv = 1.0f / _atlasWidth;
	const float yDiv = 1.0f / _atlasHeight;

	size_t divSum = _xNum * _yNum;
	for (int y = 0; y < _yNum; ++y)
	{
		for (int x = 0; x < _xNum; ++x)
		{
			int index = x + _xNum*y;
			ao::float2 lt((x*_xSize)*xDiv,(float)(y*_ySize)*yDiv);
			ao::float2 rt(lt.x+_xSize*xDiv,lt.y);
			ao::float2 lb(lt.x,lt.y+_ySize*yDiv);
			ao::float2 rb(rt.x,lb.y);
			_ppOutArray[index] = new iUiObject(this->pInstance, lt, rt, lb, rb);
			this->pBufferList.push_back(_ppOutArray[index]);
		}
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		バッファ解放 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/29	16:58	T.Aoyama	作成
//------------------------------------------------------------------------------
void iUiObjectFactory::DeleteBuffer()
{
	for (auto& it : this->pBufferList)
		ao::macro::SAFE_DELETE(it);

	ao::macro::SAFE_DELETE(this->pExecutor);

	this->pBufferList.clear();
}