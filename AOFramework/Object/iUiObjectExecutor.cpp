#include"stdafx.hpp"
#include"iUiObjectExecutor.hpp"
#include"../Shader/iRenderUi/iRenderUiShader.hpp"
#include"../Define/SingletonTypeDefine.inl"


using namespace ao;


iUiObjectExecutor::iUiObjectExecutor(UiInstance* _pInstance) : pInstance(_pInstance)
{

}


iUiObjectExecutor::~iUiObjectExecutor()
{

}


//------------------------------------------------------------------------------
/**	@brief		インスタンスのレンダリング実行 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/03/29	16:40	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT iUiObjectExecutor::ExecuteCommandList(ao::BlendState _blendState,ao::FillState _fillState)
{
#ifdef __AO_DEBUG__
	if (this->pInstance == nullptr){
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[iUiObjectExecutor::ExecuteCommandList]pInstance is nullptr.\n"));
		return E_FAIL;
	}
#endif

	sUiIRenderer()->ExecuteCommandList(this->pInstance, _blendState, _fillState);

	return S_OK;
}