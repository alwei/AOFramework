/*************************************************************
*	@file   	ShaderFunction.hpp
*	@brief  	シェーダー自作関数群
*	@author		Tatsunori Aoyama
*	@date		2014/08/10
*************************************************************/
#ifndef _Include_ShaderFunction_hpp_	// インクルードガード
#define _Include_ShaderFunction_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<vector>
#include<d3d11.h>
#include<d3d11shader.h>
#include"../../Define/SystemDefine.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@brief		文字列シェーダーのコンパイル
	//	@param[out]	_ppOutBlob			出力するBlobポインタ
	//	@param[in]	_pSrcData			データポインタ
	//	@param[in]	_SrcDataSize		データサイズ
	//	@param[in]	_pEntrypoint		エントリー名
	//	@param[in]	_pTarget			バージョン名
	//	@param[in]	_pDefines			シェーダーマクロ配列[省略可]
	//	@param[in]	_pInclude			インクルードポインタ[省略可]
	//	@param[out]	_ppErrorMsgs		出力するエラーポインタ[省略可]
	//	@return		S_OK：コンパイル成功　それ以外：コンパイル失敗
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CompileStringShader(
		ID3DBlob** _ppOutBlob,
		void* const _pSrcData,
		UINT _SrcDataSize,
		LPCSTR _pEntrypoint,
		LPCSTR _pTarget,
		const D3D_SHADER_MACRO* const _pDefines=nullptr,
		ID3DInclude* const _pInclue=nullptr,
		ID3DBlob** _ppErrorMsgs=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		頂点レイアウトの1要素を取得
	//	@param[out]	_pInputElementDesc	出力する頂点レイアウト情報
	//	@param[in]	_pReflection		リフレクションポインタ
	//	@param[in]	_paramIndex			パラメーターインデックス
	//	@param[in]	_byteOffset			バイトオフセット
	//	@return		1要素のバイト数
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	UINT GetInputElementDesc(
		D3D11_INPUT_ELEMENT_DESC* _pInputElementDesc,
		ID3D11ShaderReflection* _pReflection,
		UINT _paramIndex,
		UINT _byteOffset);

	/*!-----------------------------------------------------------
	//	@brief		頂点レイアウトを自動抽出
	//	@param[in]	_pd3dDevice		デバイスポインタ
	//	@param[out]	_ppOut			出力する頂点レイアウト
	//	@param[in]	_pBlob			コンパイル済みシェーダー
	//	@return		S_OK：抽出成功　それ以外：抽出失敗
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	 CreateAutoVertexInputLayout(
		ID3D11Device* const _pd3dDevice,
		ID3D11InputLayout**_ppOut,
		ID3DBlob* const _pBlob);

	/*!-----------------------------------------------------------
	//	@brief		頂点シェーダー作成
	//	@param[in]	_pd3dDevice			デバイスポインタ
	//	@param[out]	_ppOutShader		出力するシェーダーポインタ
	//	@param[out]	_ppOutInputLayout	出力する頂点レイアウト
	//	@param[in]	_pSrcData			データポインタ
	//	@param[in]	_SrcDataSize		データサイズ
	//	@param[in]	_pEntrypoint		エントリー名[省略可]
	//	@param[in]	_pTarget			バージョン名[省略可]
	//	@param[in]	_pDefines			シェーダーマクロ配列[省略可]
	//	@param[in]	_pInclude			インクルードポインタ[省略可]
	//	@param[in]	_pClassLinkage		クラスリンケージポインタ[省略可]
	//	@param[out]	_ppErrorMsgs		出力するエラーポインタ[省略可]
	//	@return		S_OK：作成成功　それ以外：作成失敗
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateVertexShader(
		ID3D11Device* _pd3dDevice,
		ID3D11VertexShader**_ppOutShader,
		ID3D11InputLayout**_ppOutInputLayout,
		void* const _pSrcData,
		UINT _SrcDataSize,
		LPCSTR _pEntrypoint="main",
		LPCSTR _pTarget="vs_5_0",
		const D3D_SHADER_MACRO* const _pDefines=nullptr,
		ID3DInclude* const _pInclude=nullptr,
		ID3D11ClassLinkage* _pClassLinkage=nullptr,
		ID3DBlob**_ppErrorMsgs=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		ジオメトリシェーダー作成
	//	@param[in]	_pd3dDevice			デバイスポインタ
	//	@param[out]	_ppOutShader		出力するシェーダーポインタ
	//	@param[in]	_pSrcData			データポインタ
	//	@param[in]	_SrcDataSize		データサイズ
	//	@param[in]	_pEntrypoint		エントリー名[省略可]
	//	@param[in]	_pTarget			バージョン名[省略可]
	//	@param[in]	_pDefines			シェーダーマクロ配列[省略可]
	//	@param[in]	_pInclude			インクルードポインタ[省略可]
	//	@param[in]	_pClassLinkage		クラスリンケージポインタ[省略可]
	//	@param[out]	_ppErrorMsgs		出力するエラーポインタ[省略可]
	//	@return		S_OK：作成成功　それ以外：作成失敗
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateGeometryShader(
		ID3D11Device* _pd3dDevice,
		ID3D11GeometryShader**_ppOutShader,
		void* const _pSrcData,
		UINT _SrcDataSize,
		LPCSTR _pEntrypoint="main",
		LPCSTR _pTarget="gs_5_0",
		const D3D_SHADER_MACRO* const _pDefines=nullptr,
		ID3DInclude* const _pInclude=nullptr,
		ID3D11ClassLinkage* _pClassLinkage=nullptr,
		ID3DBlob**_ppErrorMsgs=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		ピクセルシェーダー作成
	//	@param[in]	_pd3dDevice			デバイスポインタ
	//	@param[out]	_ppOutShader		出力するシェーダーポインタ
	//	@param[in]	_pSrcData			データポインタ
	//	@param[in]	_SrcDataSize		データサイズ
	//	@param[in]	_pEntrypoint		エントリー名[省略可]
	//	@param[in]	_pTarget			バージョン名[省略可]
	//	@param[in]	_pDefines			シェーダーマクロ配列[省略可]
	//	@param[in]	_pInclude			インクルードポインタ[省略可]
	//	@param[in]	_pClassLinkage		クラスリンケージポインタ[省略可]
	//	@param[out]	_ppErrorMsgs		出力するエラーポインタ[省略可]
	//	@return		S_OK：作成成功　それ以外：作成失敗
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreatePixelShader(
		ID3D11Device* _pd3dDevice,
		ID3D11PixelShader**_ppOutShader,
		void* const _pSrcData,
		UINT _SrcDataSize,
		LPCSTR _pEntrypoint="main",
		LPCSTR _pTarget="ps_5_0",
		const D3D_SHADER_MACRO* const _pDefines=nullptr,
		ID3DInclude* const _pInclude=nullptr,
		ID3D11ClassLinkage* _pClassLinkage=nullptr,
		ID3DBlob**_ppErrorMsgs=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		コンピュートシェーダー作成
	//	@param[in]	_pd3dDevice			デバイスポインタ
	//	@param[out]	_ppOutShader		出力するシェーダーポインタ
	//	@param[in]	_pSrcData			データポインタ
	//	@param[in]	_SrcDataSize		データサイズ
	//	@param[in]	_pEntrypoint		エントリー名[省略可]
	//	@param[in]	_pTarget			バージョン名[省略可]
	//	@param[in]	_pDefines			シェーダーマクロ配列[省略可]
	//	@param[in]	_pInclude			インクルードポインタ[省略可]
	//	@param[in]	_pClassLinkage		クラスリンケージポインタ[省略可]
	//	@param[out]	_ppErrorMsgs		出力するエラーポインタ[省略可]
	//	@return		S_OK：作成成功　それ以外：作成失敗
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateComputeShader(
		ID3D11Device* const _pd3dDevice,
		ID3D11ComputeShader** const _ppOutShader,
		void* const _pSrcData,
		const UINT _SrcDataSize,
		const LPCSTR _pEntrypoint="main",
		const LPCSTR _pTarget="cs_5_0",
		const D3D_SHADER_MACRO* const _pDefines=nullptr,
		ID3DInclude* const _pInclude=nullptr,
		ID3D11ClassLinkage* _pClassLinkage=nullptr,
		ID3DBlob**_ppErrorMsgs=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		バッファの作成
	//	@param[in]	_pd3dDevice			デバイスポインタ
	//	@param[out]	_ppOutBuffer		出力するバッファのダブルポインタ
	//	@param[in]	_pSrcData			入力するデータ
	//	@param[in]	_size				入力するデータサイズ
	//	@param[in]	_usage				仕様用途
	//	@param[in]	_cpuAccessFlag		CPUのアクセス許可フラグ
	//	@param[in]	_bindFlag		バインドされるフラグ
	//	@return		S_OK：バッファの作成に成功　それ以外：バッファの作成に失敗
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateBuffer(
		ID3D11Device* const _pd3dDevice,
		ID3D11Buffer** const _ppOutBuffer,
		const void* const _pSrcData,
		const UINT _size,
		const D3D11_USAGE _usage,
		const UINT _cpuAccessFlag,
		const D3D11_BIND_FLAG _bindFlag);

	/*!-----------------------------------------------------------
	//	@brief		頂点バッファの作成
	//	@param[in]	_pd3dDevice	デバイスポインタ
	//	@param[out]	_ppOutBuffer		出力するバッファのダブルポインタ
	//	@param[in]	_size				入力するデータサイズ
	//	@param[in]	_pSrcData			入力するデータ[省略可]
	//	@param[in]	_usage				仕様用途[省略可]
	//	@param[in]	_cpuAccessFlag		CPUのアクセス許可フラグ[省略可]
	//	@return		S_OK：作成成功　それ以外：作成失敗
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateVertexBuffer(
		ID3D11Device* const _pd3dDevice,
		ID3D11Buffer** const _ppOutBuffer,
		const UINT _size,
		const void* const _pSrcData = nullptr,
		const D3D11_USAGE _usage = D3D11_USAGE_DEFAULT,
		const UINT _cpuAccessFlag = FALSE);

	/*!-----------------------------------------------------------
	//	@brief		インデックスバッファの作成
	//	@param[in]	_pd3dDevice	デバイスポインタ
	//	@param[out]	_ppOutBuffer		出力するバッファのダブルポインタ
	//	@param[in]	_size				入力するデータサイズ
	//	@param[in]	_pSrcData			入力するデータ[省略可]
	//	@param[in]	_usage				仕様用途[省略可]
	//	@param[in]	_cpuAccessFlag		CPUのアクセス許可フラグ[省略可]
	//	@return		S_OK：作成成功　それ以外：作成失敗
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateIndexBuffer(
		ID3D11Device* const _pd3dDevice,
		ID3D11Buffer** const _ppOutBuffer,
		const UINT _size,
		const void* const _pSrcData,
		const D3D11_USAGE _usage = D3D11_USAGE_DEFAULT,
		const UINT _cpuAccessFlag = FALSE);

	/*!-----------------------------------------------------------
	//	@brief		定数バッファの作成
	//	@param[in]	_pd3dDevice			デバイスポインタ
	//	@param[out]	_ppOutBuffer		出力するバッファのダブルポインタ
	//	@param[in]	_size				入力するデータサイズ
	//	@param[in]	_pSrcData			入力するデータ[省略可]
	//	@param[in]	_usage				仕様用途[省略可]
	//	@param[in]	_cpuAccessFlag		CPUのアクセス許可フラグ[省略可]
	//	@return		S_OK：作成成功　それ以外：作成失敗
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	HRESULT	CreateConstantBuffer(
		ID3D11Device* const _pd3dDevice,
		ID3D11Buffer** const _ppOutBuffer,
		const UINT _size,
		const void* const _pSrcData = nullptr,
		const D3D11_USAGE _usage = D3D11_USAGE_DYNAMIC,
		const UINT _cpuAccessFlag = D3D10_CPU_ACCESS_WRITE);

	/*!-----------------------------------------------------------
	//	@brief		アンオーダーアクセスバッファの作成
	//	@note		頂点バッファはD3D11_RESOURCE_MISC_BUFFER_STRUCTUREDを指定できない
	//	@param[in]	_pd3dDevice		デバイスポインタ
	//	@param[out]	_ppOutBuffer	出力するバッファ
	//	@param[out]	_ppOutUaView	出力するアクセスビュー
	//	@param[in]	_usage			使用用途
	//	@param[in]	_miscFlag		リソースフラグ
	//	@param[in]	_elementSize	要素サイズ
	//	@param[in]	_nCount			要素数
	//	@param[in]	_pInitData		初期化データ[省略可]
	//	@param[in]	_addFkag		追加フラグ[省略可]
	//	@return		S_OK：作成成功　それ以外：作成失敗
	//	@author		Tatsunori Aoyama
	//	@date		2014/11/15
	------------------------------------------------------------*/
	HRESULT	CreateUnorderedAccessBuffer(
		ID3D11Device* const _pd3dDevice,
		ID3D11Buffer** const _ppOutBuffer,
		ID3D11UnorderedAccessView** const _ppOutUaView,
		const D3D11_RESOURCE_MISC_FLAG _miscFlag,
		const UINT _elementSize,
		const UINT _nCount,
		const void* _pInitData = nullptr,
		const D3D11_BIND_FLAG _addBindFlag = (D3D11_BIND_FLAG)0);

	/*!-----------------------------------------------------------
	//	@brief		対象ビューからシェーダーリソースビューを取得
	//	@param[in]	_pd3dDevice		使用するデバイスポインタ
	//	@param[out]	_ppOutSrView	出力するシェーダーリソースビュー
	//	@param[in]	_pView			抽出するレンダーターゲットビュー
	//	@param[in]  _pDesc			シェーダーリソースビュー情報
	//	@return		S_OK：取得成功　それ以外：取得失敗
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	template<class T>
	inline	HRESULT	GetShaderResourceView(
		ID3D11Device* const _pd3dDevice,
		ID3D11ShaderResourceView** const _ppOutSrView,
		T* const _pView,
		const D3D11_SHADER_RESOURCE_VIEW_DESC* const _pDesc = nullptr
		)
	{
		SmartComPtr<ID3D11Resource> pTemp;
		_pView->GetResource(pTemp.ToCreator());
		HRESULT hr = _pd3dDevice->CreateShaderResourceView( pTemp.GetComPtr(),_pDesc,_ppOutSrView );
#ifdef __AO_DEBUG__
		if( FAILED( hr ) )
			ao::debug::PrintfColor(ConsoleColor::H_RED,_T("[GetShaderResourceView]Failed create shader resource view.\n"));
#endif
		return hr;
	}

	/*!-----------------------------------------------------------
	//	@brief		頂点バッファの書き換え
	//	@param[in]	_pd3dDeviceContext	デバイスコンテキストポインタ
	//	@param[in]	pBuffer				バッファポインタ
	//	@param[in]	_pSrcData			元データポインタ
	//	@param[in]	_size				元データサイズ
	//	@author		Tatsunori Aoyama
	//	@date		2013/08/15
	------------------------------------------------------------*/
	inline	void	OverWriteVertexBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		ID3D11Buffer* const _pBuffer,
		const void* const _pSrcData,
		const UINT _size
		)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		_pd3dDeviceContext->Map( _pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
		::CopyMemory( mappedResource.pData, _pSrcData, _size );
		_pd3dDeviceContext->Unmap( _pBuffer, 0 );
	}

	/*!-----------------------------------------------------------
	//	@brief		頂点シェーダーの定数バッファを更新
	//	@note		GPUのアクセス拒否がなし(リソースは動的に作成すること)
	//	@param[in]	_pd3dDeviceContext	デバイスコンテキストポインタ
	//	@param[in]	_slotId				スロット番号
	//	@param[in]	_numBuffer			バッファの数
	//	@param[in]	_ppBuffer			設定するバッファ
	//	@param[in]	_pSrcData			設定するデータソース
	//	@param[in]	_pDstBox			サブリソースの定義BOX[省略可]
	//	@author		Tatsunori Aoyama
	//	@date		2013/06/16
	------------------------------------------------------------*/
	void	UpdateVertexConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const D3D11_BOX* const _pDstBox=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		ジオメトリシェーダーの定数バッファを更新
	//	@note		GPUのアクセス拒否がなし(リソースは動的に作成すること)
	//	@param[in]	_slotId				スロット番号
	//	@param[in]	_numBuffer			バッファの数
	//	@param[in]	_ppBuffer			設定するバッファ
	//	@param[in]	_pSrcData			ソース元となるデータ
	//	@param[in]	_pDstBox			サブリソースの定義BOX[省略可]
	//	@author		Tatsunori Aoyama
	//	@date		2013/08/07
	------------------------------------------------------------*/
	void	UpdateGeometryConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const D3D11_BOX* const _pDstBox=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		ピクセルシェーダーの定数バッファを更新
	//	@note		GPUのアクセス拒否がなし(リソースは動的に作成すること)
	//	@param[in]	_slotId				スロット番号
	//	@param[in]	_numBuffer			バッファの数
	//	@param[in]	_ppBuffer			設定するバッファ
	//	@param[in]	_pSrcData			ソース元となるデータ
	//	@param[in]	_pDstBox			サブリソースの定義BOX[省略可]
	//	@author		Tatsunori Aoyama
	//	@date		2013/08/07
	------------------------------------------------------------*/
	void	UpdatePixelConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const D3D11_BOX* const _pDstBox=nullptr);

	/*!-----------------------------------------------------------
	//	@brief		コンピュートシェーダーの定数バッファを更新
	//	@note		GPUのアクセス拒否がなし(リソースは動的に作成すること)
	//	@param[in]	_slotId				スロット番号
	//	@param[in]	_numBuffer			バッファの数
	//	@param[in]	_ppBuffer			設定するバッファ
	//	@param[in]	_pSrcData			ソース元となるデータ
	//	@param[in]	_pDstBox			サブリソースの定義BOX[省略可]
	//	@author		Tatsunori Aoyama
	//	@date		2014/08/10
	------------------------------------------------------------*/
	void	UpdateComputeConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const D3D11_BOX* const _pDstBox = nullptr);

	/*!-----------------------------------------------------------
	//	@brief		頂点シェーダーの定数バッファを更新
	//	@note		MAPによるGPUアクセス拒否あり
	//	@param[in]	_pd3dDeviceContext	MAPするデバイスコンテキスト先
	//	@param[in]	_slotId				スロット番号
	//	@param[in]	_numBuffer			バッファの数
	//	@param[in]	_ppBuffer			設定するバッファ
	//	@param[in]	_pSrcData			ソース元となるデータ
	//	@param[in]	_SrcDataSize		ソース元のデータサイズ
	//	@param[in]	_mapFlag			MAPフラグ[省略可]
	//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
	//	@author		Tatsunori Aoyama
	//	@date		2013/07/12
	------------------------------------------------------------*/
	HRESULT	MapVertexConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const UINT _SrcDataSize,
		const D3D11_MAP _mapFlag=D3D11_MAP_WRITE_DISCARD);

	/*!-----------------------------------------------------------
	//	@brief		ジオメトリシェーダーの定数バッファを更新
	//	@note		MAPによるGPUアクセス拒否あり
	//	@param[in]	_pd3dDeviceContext	MAPするデバイスコンテキスト先
	//	@param[in]	_slotId				スロット番号
	//	@param[in]	_numBuffer			バッファの数
	//	@param[in]	_ppBuffer			設定するバッファ
	//	@param[in]	_pSrcData			ソース元となるデータ
	//	@param[in]	_SrcDataSize		ソース元のデータサイズ
	//	@param[in]	_mapFlag			MAPフラグ[省略可]
	//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
	//	@author		Tatsunori Aoyama
	//	@date		2013/07/12
	------------------------------------------------------------*/
	HRESULT	MapGeometryConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const UINT _SrcDataSize,
		const D3D11_MAP _mapFlag=D3D11_MAP_WRITE_DISCARD);

	/*!-----------------------------------------------------------
	//	@brief		ピクセルシェーダーの定数バッファを更新
	//	@note		MAPによるGPUアクセス拒否あり
	//	@param[in]	_pd3dDeviceContext	MAPするデバイスコンテキスト先
	//	@param[in]	_slotId				スロット番号
	//	@param[in]	_numBuffer			バッファの数
	//	@param[in]	_ppBuffer			設定するバッファ
	//	@param[in]	_pSrcData			ソース元となるデータ
	//	@param[in]	_SrcDataSize		ソース元のデータサイズ
	//	@param[in]	_mapFlag			MAPフラグ1[省略可]
	//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
	//	@author		Tatsunori Aoyama
	//	@date		2013/08/07
	------------------------------------------------------------*/
	HRESULT	MapPixelConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const UINT _SrcDataSize,
		const D3D11_MAP _mapFlag=D3D11_MAP_WRITE_DISCARD);

	/*!-----------------------------------------------------------
	//	@brief		コンピュートシェーダーの定数バッファを更新
	//	@param[in]	_pd3dDeviceContext	MAPするデバイスコンテキスト先
	//	@param[in]	_slotId				スロット番号
	//	@param[in]	_numBuffer			バッファの数
	//	@param[in]	_ppBuffer			設定するバッファ
	//	@param[in]	_pSrcData			ソース元となるデータ
	//	@param[in]	_SrcDataSize		ソース元のデータサイズ
	//	@param[in]	_mapFlag			MAPフラグ[省略可]
	//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
	//	@author		Tatsunori Aoyama
	//	@date		2013/11/04
	------------------------------------------------------------*/
	HRESULT	MapComputeConstantBuffer(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _slotId,
		const UINT _numBuffer,
		ID3D11Buffer* const *_ppBuffer,
		const void* const _pSrcData,
		const UINT _SrcDataSize,
		const D3D11_MAP _mapFlag = D3D11_MAP_WRITE_DISCARD);

	/*!-----------------------------------------------------------
	//	@brief		テクスチャのセット
	//	@param[in]	_pd3dDeviceContext	デバイスコンテキストポインタ
	//	@param[in]	_ppSrView			シェーダーリソースビューポインタ
	//	@param[in]	_ppSampState		サンプラーステートポインタ
	//	@param[in]	_srViewSlot			ビュースロット
	//	@param[in]	_sampStateSlot		サンプラースロット
	//	@author		Tatsunori Aoyama
	//	@date		2013/08/09
	------------------------------------------------------------*/
	void	SetTexture(
		ID3D11DeviceContext* const _pd3dDeviceContext,
		const UINT _srViewSlot = 0,
		const UINT _sampStateSlot = 0,
		ID3D11ShaderResourceView* const *_ppSrView = nullptr,
		ID3D11SamplerState* const *_ppSampState = nullptr);
}

#endif // _Include_ShaderFunction_hpp_