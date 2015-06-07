/*************************************************************
*	@file   	DirectX11Shader.hpp
*	@brief  	DirectX11Shaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/08
*************************************************************/

#ifndef _Include_DirectX11Shader_hpp_	// インクルードガード
#define _Include_DirectX11Shader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<map>
#include<vector>
#include<array>
#include<list>
#include<d3dcompiler.h>
#include<d3d11shader.h>
#include"DirectX11BaseApi.hpp"
#include"../Resource/ShaderResource.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11Shader
	//	@brief  シェーダー管理クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/08
	------------------------------------------------------------*/
	class DirectX11Shader : public DirectX11BaseApi
	{
	public:
		DirectX11Shader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11Shader()override final;

		/*!-----------------------------------------------------------
		//	@brief		頂点シェーダーの読み込み
		//	@note		頂点レイアウトの指定がなければ自動取得
		//	@param[in]	_filePath			読み込むファイルパス
		//	@param[in]	_entryPoint			エントリーポイント名[省略可]
		//	@param[in]	_target				SMバージョン名[省略可]
		//	@param[in]	_pInclude			インクルード指定ポインタ[省略化]
		//	@param[in]	_pDefines			定義マクロ配列ポインタ[省略可]
		//	@param[in]	_pDesc				手動頂点レイアウトポインタ[省略可]
		//	@param[in]	_size				手動頂点レイアウトサイズ[省略可]
		//	@param[in]	_flags1				コンパイルオプションフラグ１[省略可]
		//	@param[in]	_flags2				コンパイルオプションフラグ２[省略可]
		//	@return		頂点シェーダーポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		VertexShader*	LoadVertexShaderFromFile(
			LPCWSTR _filePath,
			LPCSTR _entryPoint = "main",
			LPCSTR _target = "vs_5_0",
			ID3DInclude* const _pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE,
			const D3D_SHADER_MACRO* _pDefines = nullptr,
			const D3D11_INPUT_ELEMENT_DESC* _pDesc = nullptr,
			const UINT _size = 0,
			const UINT _flags1 = 0,
			const UINT _flags2 = 0);

		/*!-----------------------------------------------------------
		//	@brief		ジオメトリシェーダーの読み込み
		//	@param[in]	_filePath			読み込むファイルパス
		//	@param[in]	_entryPoint			エントリーポイント名[省略可]
		//	@param[in]	_target				SMバージョン名[省略可]
		//	@param[in]	_pInclude			インクルード指定ポインタ[省略化]
		//	@param[in]	_pDefines			定義マクロ配列ポインタ[省略可]
		//	@param[in]	_Flags1				コンパイルオプションフラグ１[省略可]
		//	@param[in]	_Flags2				コンパイルオプションフラグ２[省略可]
		//	@return		ジオメトリシェーダーポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		GeometryShader*	LoadGeometryShaderFromFile(
			LPCWSTR _filePath,
			LPCSTR _entryPoint = "main",
			LPCSTR _target = "gs_5_0",
			ID3DInclude* const _pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE,
			const D3D_SHADER_MACRO* _pDefines = nullptr,
			const UINT _Flags1 = 0,
			const UINT _Flags2 = 0);

		/*!-----------------------------------------------------------
		//	@brief		ピクセルシェーダー読み込み
		//	@param[in]	_filePath			読み込むファイルパス
		//	@param[in]	_entryPoint			エントリーポイント名[省略可]
		//	@param[in]	_target				SMバージョン名[省略可]
		//	@param[in]	_pInclude			インクルード指定ポインタ[省略化]
		//	@param[in]	_pDefines			定義マクロ配列ポインタ[省略可]
		//	@param[in]	_Flags1				コンパイルオプションフラグ１[省略可]
		//	@param[in]	_Flags2				コンパイルオプションフラグ２[省略可]
		//	@return		ピクセルシェーダーポインタハンドル
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		PixelShader*	LoadPixelShaderFromFile(
			LPCWSTR _filePath,
			LPCSTR _entryPoint = "main",
			LPCSTR _target = "ps_5_0",
			ID3DInclude* const _pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE,
			const D3D_SHADER_MACRO* _pDefines = nullptr,
			const UINT _Flags1 = 0,
			const UINT _Flags2 = 0);

		/*!-----------------------------------------------------------
		//	@brief		コンピュートシェーダーの読み込み
		//	@param[in]	_filePath			読み込むファイルパス
		//	@param[in]	_entryPoint		エントリーポイント名[省略可]
		//	@param[in]	_target			SMバージョン名[省略可]
		//	@param[in]	_pInclude			インクルード指定ポインタ[省略化]
		//	@param[in]	_pDefines			定義マクロ配列ポインタ[省略可]
		//	@param[in]	_Flags1				コンパイルオプションフラグ１[省略可]
		//	@param[in]	_Flags2				コンパイルオプションフラグ２[省略可]
		//	@return		コンピュートシェーダーポインタハンドル
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		ComputeShader*	LoadComputeShaderFromFile(
			LPCWSTR _filePath,
			LPCSTR _entryPoint = "main",
			LPCSTR _target = "cs_5_0",
			ID3DInclude* const _pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE,
			const D3D_SHADER_MACRO* _pDefines = nullptr,
			const UINT _Flags1 = 0,
			const UINT _Flags2 = 0);

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
		UavBuffer*	CreateUnorderedAccessBuffer(
			const D3D11_USAGE _usage,
			const D3D11_RESOURCE_MISC_FLAG _miscFlag,
			const UINT _elementSize,
			const UINT _nCount,
			const void* _pInitData = nullptr,
			const D3D11_BIND_FLAG _addBindFlag = (D3D11_BIND_FLAG)0);

		/*!-----------------------------------------------------------
		//	@brief		頂点シェーダーリソース解放
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		void	ReleaseVertexShader();

		/*!-----------------------------------------------------------
		//	@brief		ジオメトリシェーダーリソース解放
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		void	ReleaseGeometryShader();

		/*!-----------------------------------------------------------
		//	@brief		ピクセルシェーダーリソース解放
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		void	ReleasePixelShader();

		/*!-----------------------------------------------------------
		//	@brief		コンピュートシェーダーリソース解放
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		void	ReleaseComputeShader();

		/*!-----------------------------------------------------------
		//	@brief		アンオーダーアクセスバッファ解放
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/15
		------------------------------------------------------------*/
		void	ReleaseUavBuffer();

	private:
		std::map<string, VertexShader*>		pVertexShaderFileMap;
		std::map<string, GeometryShader*>	pGeometryShaderFileMap;
		std::map<string, PixelShader*>		pPixelShaderFileMap;
		std::map<string, ComputeShader*>	pComputeShaderFileMap;
		std::list<UavBuffer*> pUavList;

	private:
		/*!-----------------------------------------------------------
		//	@brief		頂点レイアウトの手動作成
		//	@param[out]	_ppOut	出力する頂点レイアウト
		//	@param[in]	_pBlob	コンパイルされたシェーダーデータ
		//	@param[in]	_desc	入力する頂点属性情報
		//	@param[in]	_size	入力する頂点属性情報サイズ
		//	@return		S_OK：作成成功	それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		HRESULT	CreateManualInputLayout(
			ID3D11InputLayout** _ppOut,
			ID3DBlob* _pBlob,
			CONST D3D11_INPUT_ELEMENT_DESC* _pDesc,
			UINT _size);

		/*!-----------------------------------------------------------
		//	@brief		頂点レイアウトを自動生成
		//	@param[in]	_ppOut	出力する頂点レイアウト
		//	@return		_pBlob	シェーダーバッファ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/09
		------------------------------------------------------------*/
		HRESULT	CreateAutoInputLayout(
			ID3D11InputLayout** _ppOut,
			ID3DBlob* _pBlob);
	};
}

#endif // _Include_DirectX11Shader_hpp_