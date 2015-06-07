/*************************************************************
*	@file   	iUiObjectExecutor.hpp
*	@brief  	iUiObjectExecutorクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/03/29
*************************************************************/

#ifndef _Include_iUiObjectExecutor_hpp_	// インクルードガード
#define _Include_iUiObjectExecutor_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include"../Api/DirectX11GraphicsState.hpp"


namespace ao
{
	struct UiInstance;

	/*!-----------------------------------------------------------
	//	@class  iUiObjectExecutor
	//	@brief  UIインスタンスの実行クラス
	//	@author	Tatsunori Aoyama
	//	@date	2015/03/29
	------------------------------------------------------------*/
	class iUiObjectExecutor
	{
	public:
		iUiObjectExecutor(UiInstance* _pInstance);
		~iUiObjectExecutor();

		/*!-----------------------------------------------------------
		//	@brief		インスタンスのレンダリング実行
		//	@return		_blendState	ブレンドステート[省略可]
		//	@param[in]	_fillState	描画モード[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		HRESULT ExecuteCommandList(
			const BlendState _blendState = BlendState::Alpha,
			const FillState _fillState = FillState::Solid);

	private:
		UiInstance* pInstance;
	};
}

#endif // _Include_iUiObjectExecutor_hpp_