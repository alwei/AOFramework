/*************************************************************
*	@file   	DirectX11EnumDisplay.hpp
*	@brief  	DirectX11EnumDisplayクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/06/27
*************************************************************/

#ifndef _Include_DirectX11EnumDisplay_hpp_	// インクルードガード
#define _Include_DirectX11EnumDisplay_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include<vector>
#include"DirectX11Header.hpp"
#include"../Define/SystemDefine.hpp"
#include"../Utility/NonCopyable.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11EnumDisplay
	//	@brief  ディスプレイ列挙クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/06/27
	------------------------------------------------------------*/
	class DirectX11EnumDisplay : private NonCopyable
	{
	public:
		DirectX11EnumDisplay();
		~DirectX11EnumDisplay();

		/*!-----------------------------------------------------------
		//	@brief		ディスプレイ列挙
		//	@param[in]  _pAdapter	アダプターポインタ
		//	@param[in]  _getFormat	取得するフォーマット
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		HRESULT EnumDisplay(IDXGIAdapter* _pAdapter, DXGI_FORMAT _getFormat);

		/*!-----------------------------------------------------------
		//	@brief		指定のディスプレイサイズ情報取得
		//	@param[in]	_nWidth		指定する横サイズ
		//	@param[in]  _nHeight	指定する縦サイズ
		//	@return		ディスプレイの情報ポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		DXGI_MODE_DESC*	GetEnumDisplayDesc(
			DXGI_MODE_DESC* _pOut,
			const UINT _nWidth,
			const UINT _nHeight);

		/*!-----------------------------------------------------------
		//	@brief		列挙したディスプレイ数を取得
		//	@return		列挙したディスプレイ数
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		const size_t	GetNumEnumDisplay();

	private:
		BOOL bEnumDisplayMode;
		std::vector<DXGI_MODE_DESC> dxGiDescVector;
	};
}

#endif // _Include_DirectX11EnumDisplay_hpp_
