/*************************************************************
*	@file   	DirectX11DebugFont.hpp
*	@brief  	DirectX11DebugFontクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/09/24
*************************************************************/

#ifndef _Include_DirectX11DebugFont_hpp_	// インクルードガード
#define _Include_DirectX11DebugFont_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<mutex>
#include"DirectX11BaseApi.hpp"
#include"DirectX11RenderTarget.hpp"
#include"DirectX11GraphicsState.hpp"
#include"DirectX11GraphicsRenderer.hpp"
#include"../Resource/ConstBuffer.hpp"
#include"../Resource/Sprite.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DebugChar
	//	@brief  デバッグ用の文字
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/24
	------------------------------------------------------------*/
	class DebugChar
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		テクスチャ作成
		//	@param[in]	_pd3dDevice			デバイスポインタ
		//	@param[in]	_pd3dDeviceContext	デバイスコンテキスト
		//  @param[in]  _code				文字コード
		//  @param[in]  _lf					フォント情報
		//	@param[in]	_fontSize			フォントサイズ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		void	Create(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext,
			const DWORD _code,
			const LOGFONT& _lf,
			int _fontSize);


		/*!-----------------------------------------------------------
		//	@brief		テクスチャ取得
		//	@return		テクスチャ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		ID3D11ShaderResourceView* GetTexture()
		{
			return this->pTexture.GetComPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		ハッシュ値取得
		//	@return		ハッシュ値
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		int64_t GetHash()
		{
			return this->hash;
		}

	private:
		SmartComPtr<ID3D11ShaderResourceView> pTexture;
		SmartComPtr<ID3D11Device> pd3dDevice;
		SmartComPtr<ID3D11DeviceContext> pd3dDeviceContext;
		int64_t hash;

	private:
		struct FONT_INFO
		{
			TEXTMETRIC		tm;
			GLYPHMETRICS	gm;
			DWORD			bitSize;
		};

		/*!-----------------------------------------------------------
		//	@brief		フォント情報作成
		//  @param[out] _pFont		出力するフォント
		//  @param[in]  _lf			フォント情報
		//  @param[in]  _fontSize	フォントサイズ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		inline	void CreateFontInfo(HFONT* _pFont,const LOGFONT& _lf, int _fontSize);

		/*!-----------------------------------------------------------
		//	@brief		ビット情報生成
		//  @param[out] _pBuffer	バッファ
		//	@param[out]	_pFontInfo	出力するフォント情報
		//  @param[in]  _code		文字コード
		//  @param[in]  _hFont		フォントハンドル
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		inline	void GenerateFontBitMap(
			std::vector<BYTE>* _pBuffer,
			FONT_INFO* _pFont,
			const DWORD _code,
			const HFONT& _hFont);

		/*!-----------------------------------------------------------
		//	@brief		テクスチャ書き込み
		//	@param[in]	_pBuffer	バッファ
		//	@param[in]	_pFont		フォント情報
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		inline	void WriteBitChar(std::vector<BYTE>* _pBuffer,FONT_INFO* _pFont);
	};

	/*!-----------------------------------------------------------
	//	@class  DirectX11DebugFont
	//	@brief  デバッグフォント
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/24
	------------------------------------------------------------*/
	class DirectX11DebugFont : public DirectX11BaseApi
	{
	public:
		DirectX11DebugFont(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11DebugFont()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//  @param[in]  _pGraphicsState		グラフィックステート
		//  @param[in]  _pRenderer			レンダラー
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		HRESULT	Initialize(
			const u32 _nWidth,
			const u32 _nHeight,
			DirectX11GraphicsState* const _pGraphicsState,
			DirectX11GraphicsRenderer* const _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		文字列個別描画
		//	@param[in]	_x			X座標
		//	@param[in]	_y			Y座標
		//	@param[in]	_bWidth		背景横幅
		//	@param[in]	_bHeight	背景縦幅
		//	@param[in]	_fontSize	フォントサイズ
		//	@param[in]	_numChar	描画する文字数
		//	@param[in]	_r			赤色
		//	@param[in]	_g			緑色
		//	@param[in]	_b			青色
		//	@param[in]	_a			透過率
		//	@param[in]	_bR			赤色背景
		//	@param[in]	_bG			緑色背景
		//	@param[in]	_bB			青色背景
		//	@param[in]	_bA			透過率背景
		//	@param[in]	_str		文字列
		//	@param[in]	...			可変要素
		//	@return		S_OK：描画成功　それ以外：描画失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		HRESULT Draw(
			int _x,
			int _y,
			int _bWidth,
			int _bHeight,
			int _fontSize,
			int _numChar,
			float _r,
			float _g,
			float _b,
			float _a,
			float _bR,
			float _bG,
			float _bB,
			float _bA,
			string _str,
			...);

		/*!-----------------------------------------------------------
		//	@brief		文字列個別描画
		//	@param[in]	_x			X座標
		//	@param[in]	_y			Y座標
		//	@param[in]	_bWidth		背景横幅
		//	@param[in]	_bHeight	背景縦幅
		//	@param[in]	_fontSize	フォントサイズ
		//	@param[in]	_r			赤色
		//	@param[in]	_g			緑色
		//	@param[in]	_b			青色
		//	@param[in]	_a			透過率
		//	@param[in]	_bR			赤色背景
		//	@param[in]	_bG			緑色背景
		//	@param[in]	_bB			青色背景
		//	@param[in]	_bA			透過率背景
		//	@param[in]	_str		文字列
		//	@param[in]	...			可変要素
		//	@return		S_OK：描画成功　それ以外：描画失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		HRESULT Draw(
			int _x,
			int _y,
			int _bWidth,
			int _bHeight,
			int _fontSize,
			float _r,
			float _g,
			float _b,
			float _a,
			float _bR,
			float _bG,
			float _bB,
			float _bA,
			string _str,
			...);

		/*!-----------------------------------------------------------
		//	@brief		バースト描画
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		void	RenderBurst();

		/*!-----------------------------------------------------------
		//	@brief		フォントの変更
		//	@param[in]	_fontName	フォント名
		//  @param[in]  _fontPath	フォントパス
		//	@return		S_OK：変更成功　それ以外：変更失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		HRESULT	ChangeFont(const string& _fontName, const string& _fontPath);

	private:
		struct FontShader
		{
			SmartComPtr<ID3D11VertexShader> vertex;
			SmartComPtr<ID3D11InputLayout> inputLayout;
			SmartComPtr<ID3D11PixelShader> pixel;
		};

		struct VsConst
		{
			XMFLOAT4X4 matWorld;
			XMFLOAT4X4 matOrtho;
		};

		struct PsConst
		{
			XMFLOAT4 color;
		};

		struct CHAR_COMMAND
		{
			float x, y;
			float bWidth, bHeight;
			float fontSize;
			int numChar;
			float r, g, b, a;
			float bR, bG, bB, bA;
			std::array<AOCHAR, 2048> buffer;
		};

		struct STR_COMMAND
		{
			float x, y;
			float bWidth, bHeight;
			float fontSize;
			float r, g, b, a;
			float bR, bG, bB, bA;
			std::array<AOCHAR, 2048> buffer;
		};

	private:
		string fontName;
		string fontPath;
		LOGFONT fontInfo;
		FontShader fontShader;
		XMFLOAT4X4 surfaceOrtho;
		XMFLOAT4X4 stringOrtho;
		ConstBuffer<VsConst> vsConst;
		ConstBuffer<PsConst> psConst;
		SmartComPtr<ID3D11Buffer> pSpriteBuffer;
		SmartComPtr<ID3D11SamplerState> pSamplerState;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
		std::map<int64_t, DebugChar*> charTextureMap;
		std::map<string, DirectX11RenderTarget*> strTextureMap;
		std::vector<CHAR_COMMAND> charCommandList;
		std::vector<STR_COMMAND> strCommandList;

	private:
		/*!-----------------------------------------------------------
		//	@brief		サンプラーステート作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/25
		------------------------------------------------------------*/
		inline	HRESULT	CreateSamplerState();

		/*!-----------------------------------------------------------
		//	@brief		デフォルトスプライト作成
		//	@return		S_OK：作成成功　それ以外：
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/25
		------------------------------------------------------------*/
		inline	HRESULT	CreateDefaultSprite();

		/*!-----------------------------------------------------------
		//	@brief		文字テクスチャ作成
		//	@param[in]	_code		文字コード
		//  @param[in]  _fontSize	フォントサイズ
		//  @return     シェーダーリソースビュー
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/25
		------------------------------------------------------------*/
		inline	ID3D11ShaderResourceView* CreateCharacterTexture(UINT _code, int _fontSize);

		/*!-----------------------------------------------------------
		//	@brief		文字列テクスチャ作成
		//	@param[in]	_str		文字列
		//  @param[in]  _fontSize	フォントサイズ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		inline	BaseTexture* CreateStringTexture(const string& _str, int _fontSize);

		/*!-----------------------------------------------------------
		//	@brief		文字一つの描画
		//	@param[in]	_commandInfo	コマンド情報
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		inline	void OnDraw(const CHAR_COMMAND& _commandInfo);

		/*!-----------------------------------------------------------
		//	@brief		文字列の描画
		//	@param[in]	_commandInfo	コマンド情報
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		inline	void OnDraw(const STR_COMMAND& _commandInfo);
	};
}

#endif // _Include_DirectX11DebugFont_hpp_