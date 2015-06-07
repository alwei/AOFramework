/*************************************************************
*	@file   	Buffer.hpp
*	@brief  	Bufferクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/12/21
*************************************************************/

#ifndef _Include_Buffer_hpp_	// インクルードガード
#define _Include_Buffer_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include<d3d11.h>
#include"../Utility/SmartComPtr.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  Buffer
	//	@brief  バッファクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/12/21
	------------------------------------------------------------*/
	class Buffer
	{
	public:
		Buffer();
		~Buffer();

		/*!-----------------------------------------------------------
		//	@brief		バッファ生成
		//	@param[in]	_pd3dDevice	デバイスポインタ
		//	@return		S_OK：生成成功　それ以外：生成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		HRESULT Create(
			const void* _initSrc,
			const UINT _dataSize,
			const D3D11_USAGE _usage,
			const UINT _cpuAccessFlag,
			SmartComPtr<ID3D11Device> _pd3dDevice);

		/*!-----------------------------------------------------------
		//	@brief		マップ処理
		//	@param[in]	_mapSrc				マップソース
		//	@param[in]	_dataSize			データサイズ
		//	@param[in]	_pd3dDeviceContext	デバイスコンテキスト
		//	@return		S_OK：マップ成功　それ以外：マップ失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		HRESULT	Map(
			const void* _mapSrc,
			const UINT _dataSize,
			const D3D11_MAP _mapFlag,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

	public:
		SmartComPtr<ID3D11Buffer> buffer;
	};
}

#endif // _Include_Buffer_hpp_