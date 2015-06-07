#include"stdafx.hpp"
#include"DirectX11DebugFont.hpp"
#include"../Math/Math.hpp"
#include"../Shader/Function/ShaderFunction.hpp"
#include"../Utility/Macro.inl"


using namespace ao;


static const char FontVertexShader[] =
{
	"cbuffer cbMatrixWP : register( b0 )                "
	"{                                                  "
	"	float4x4 matWorld;                              "
	"	float4x4 matOrtho;                              "
	"}                                                  "

	"struct VSInput                                     "
	"{                                                  "
	"	float3 pos	: POSITION;                         "
	"	float2 uv	: TEXCOORD;                         "
	"};                                                 "

	"struct VSOutput                                    "
	"{                                                  "
	"	float4 pos	: SV_POSITION;                      "
	"	float2 uv	: TEXCOORD0;                        "
	"};                                                 "

	"VSOutput	main(VSInput In)                        "
	"{                                                  "
	"	VSOutput Out = (VSOutput)0;						"
	"	Out.pos		= mul(float4(In.pos,1),matWorld);   "
	"	Out.pos		= mul(Out.pos,matOrtho);            "
	"	Out.uv		= In.uv;                            "
	"	return Out;                                     "
	"}                                                  "
};

static const char FontPixelShader[] =
{
	"cbuffer cbColor	: register( b0 )                	"
	"{                                                  	"
	"	float4 color;                                		"
	"}														"

	"Texture2D		g_texture : register( t0 );             "
	"SamplerState	g_sampler : register( s0 );             "

	"struct PSInput                                         "
	"{                                                      "
	"	float4 pos	: SV_POSITION;                          "
	"	float2 uv	: TEXCOORD0;                            "
	"};                                                     "

	"float4	main(PSInput In) : SV_TARGET					"
	"{                                                      "
	"	return color * g_texture.Sample(g_sampler,In.uv);	"
	"}                                                      "
};


//------------------------------------------------------------------------------
/**	@brief		文字テクスチャ作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/24	12:11	T.Aoyama	作成
//------------------------------------------------------------------------------
void DebugChar::Create(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext,
	const DWORD _code,
	const LOGFONT& _lf,
	int _fontSize)
{
	this->pd3dDevice = _pd3dDevice;
	this->pd3dDeviceContext = _pd3dDeviceContext;

	HFONT hFont;
	FONT_INFO fontInfo;
	std::vector<BYTE> buffer;
	this->CreateFontInfo(&hFont, _lf, _fontSize);
	this->GenerateFontBitMap(&buffer, &fontInfo, _code, hFont);
	this->WriteBitChar(&buffer, &fontInfo);

	//ハッシュ作成
	this->hash = (_code << 8) | _fontSize;
}


//------------------------------------------------------------------------------
/**	@brief		フォント情報作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/24	12:13	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	void DebugChar::CreateFontInfo(HFONT* _pFont, const LOGFONT& _lf, int _fontSize)
{
	//フォント情報設定
	LOGFONT lf;
	::CopyMemory(&lf, &_lf, sizeof(LOGFONT));
	lf.lfHeight = _fontSize;

	(*_pFont) = CreateFontIndirect(&lf);
}


//------------------------------------------------------------------------------
/**	@brief		ビット情報生成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/24	13:57	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	void DebugChar::GenerateFontBitMap(
	std::vector<BYTE>* _pBuffer,
	FONT_INFO* _pFont,
	const DWORD _code,
	const HFONT& _hFont)
{
	HDC	hDC = ::GetDC(nullptr);
	HFONT	logFont = (HFONT)SelectObject(hDC, _hFont);

	//フォントビットマップ取得
	CONST MAT2	mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };

	//デバイスコンテキストに設定されているフォントビットマップ情報取得
	GetTextMetrics(hDC, &_pFont->tm);
	DWORD bitSize = GetGlyphOutline(hDC, _code, GGO_GRAY4_BITMAP, &_pFont->gm, 0, nullptr, &mat);

	if (bitSize == GDI_ERROR)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DebugChar::GenerateFontBitMap]Faield <GetGlyphOutline> Function.\n"));
		return;
	}

	//空白の場合
	if (bitSize == 0)
	{
		return;
	}

	//バッファ領域をリサイズ
	_pBuffer->resize(bitSize);

	//フォントBITデータ取得
	GetGlyphOutline(hDC, _code, GGO_GRAY4_BITMAP, &_pFont->gm, bitSize, &((*_pBuffer)[0]), &mat);

	SelectObject(hDC, logFont);
	ReleaseDC(nullptr, hDC);
}


//------------------------------------------------------------------------------
/**	@brief		テクスチャ書き込み */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/24	21:51	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	void DebugChar::WriteBitChar(std::vector<BYTE>* _pBuffer, FONT_INFO* _pFont)
{
	D3D11_TEXTURE2D_DESC desc;
	::SecureZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = _pFont->gm.gmCellIncX;
	desc.Height = _pFont->tm.tmHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;				//動的な書き込みOK
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//シェーダーリソースとして扱う
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//CPUからのアクセス許可

	SmartComPtr<ID3D11Texture2D> pTex2D;
	this->pd3dDevice->CreateTexture2D(&desc, 0, pTex2D.ToCreator());

	//フォント情報をテクスチャに書き込む部分
	D3D11_MAPPED_SUBRESOURCE hMappedResouce;

	//マッピング準備処理
	this->pd3dDeviceContext->Map(pTex2D.GetComPtr(), 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedResouce);
	LPBYTE	pBits = (LPBYTE)hMappedResouce.pData;

	//フォント情報の書き込み
	int	offsetPosX = _pFont->gm.gmptGlyphOrigin.x;
	int	offsetPosY = _pFont->tm.tmAscent - _pFont->gm.gmptGlyphOrigin.y;
	int wBmp = _pFont->gm.gmBlackBoxX + (4 - (_pFont->gm.gmBlackBoxX % 4)) % 4;
	int hBmp = _pFont->gm.gmBlackBoxY;
	int level = 17;

	ULONG64 alpha, color;
	::SecureZeroMemory(pBits, hMappedResouce.RowPitch*_pFont->tm.tmHeight);

	//ビットデータ書き込み処理
	auto WriteBit = [&](int _x, int _y)
	{
		alpha = (65535 * (*_pBuffer)[_x - offsetPosX + wBmp*(_y - offsetPosY)]) / (level - 1);
		color = 0x0000FFFFFFFFFFFF | (alpha << 48);

		memcpy((LPBYTE)pBits + hMappedResouce.RowPitch*_y + 8 * _x, &color, sizeof(LONG64));
	};

	//横ビット計算
	auto WriteProcessStep02 = [=](int _y){
		for (int x = offsetPosX; x < offsetPosX + wBmp; ++x)
			WriteBit(x, _y);
	};

	//縦ビット計算
	auto WriteProcessStep01 = [=](){
		for (int y = offsetPosY; y < offsetPosY + hBmp; ++y)
			WriteProcessStep02(y);
	};

	//空白でなければ書き込み
	if (!_pBuffer->empty())
		WriteProcessStep01();

	this->pd3dDeviceContext->Unmap(pTex2D.GetComPtr(), 0);

	//シェーダーリソースビュー作成
	this->pd3dDevice->CreateShaderResourceView(pTex2D.GetComPtr(), nullptr, this->pTexture.ToCreator());
}


DirectX11DebugFont::DirectX11DebugFont(
	SmartComPtr<ID3D11Device> _pd3dDevice,
	SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
	pGraphicsState(nullptr),
	pRenderer(nullptr),
	DirectX11BaseApi(_pd3dDevice,_pd3dDeviceContext)
{
	this->fontName.assign(_T("no font name"));
	this->fontPath.assign(_T("no font path"));
	this->ChangeFont(_T(""), _T(""));
}


DirectX11DebugFont::~DirectX11DebugFont()
{
	for (auto& it : this->charTextureMap)
	{
		delete it.second;
	}

	for (auto& it : this->strTextureMap)
	{
		delete it.second;
	}
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/24	04:47	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11DebugFont::Initialize(
	const u32 _nWidth,
	const u32 _nHeight,
	DirectX11GraphicsState* const _pGraphicsState,
	DirectX11GraphicsRenderer* const _pRenderer)
{
	this->pGraphicsState = _pGraphicsState;
	this->pRenderer = _pRenderer;

	//フォントシェーダー作成
	ao::CreateVertexShader(
		this->pd3dDevice.GetComPtr(),
		this->fontShader.vertex.ToCreator(),
		this->fontShader.inputLayout.ToCreator(),
		(void*)FontVertexShader,
		sizeof(FontVertexShader));

	ao::CreatePixelShader(
		this->pd3dDevice.GetComPtr(),
		this->fontShader.pixel.ToCreator(),
		(void*)FontPixelShader,
		sizeof(FontPixelShader));

	//定数バッファ作成
	this->vsConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->psConst.Register(this->pd3dDevice, this->pd3dDeviceContext);
	this->vsConst.Create();
	this->psConst.Create();

	//左上原点の正射影行列作成
	MatrixOrthographicLH(&this->vsConst->matOrtho, (float)_nWidth, (float)_nHeight, 0, 1);
	this->vsConst->matOrtho._41 += 0 - 1.0f;
	this->vsConst->matOrtho._42 += 0 + 1.0f;
	MatrixTranspose(&this->vsConst->matOrtho, this->vsConst->matOrtho);
	::CopyMemory(&this->surfaceOrtho, &this->vsConst->matOrtho, sizeof(XMFLOAT4X4));

	//サンプラーステート作成
	this->CreateSamplerState();

	//デフォルトの矩形スプライト作成
	this->CreateDefaultSprite();

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		描画 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/25	03:09	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11DebugFont::Draw(
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
	...)
{
	CHAR_COMMAND charCommand;


	va_list args;
	va_start(args, _str);
	ao_vsprintf_s(charCommand.buffer.data(), charCommand.buffer.size(), _str.c_str(), args);
	va_end(args);

	//引数が無効
	if (_numChar < -1)
		return E_INVALIDARG;

	//文字列最大まで描画
	if (_numChar == -1)
		_numChar = 0xFFFF;

	charCommand.x = (float)_x;
	charCommand.y = (float)(_y * -1.0f);
	charCommand.bWidth = (float)_bWidth;
	charCommand.bHeight = (float)_bHeight;
	charCommand.fontSize = (float)_fontSize;
	charCommand.numChar = _numChar;
	charCommand.r = _r;
	charCommand.g = _g;
	charCommand.b = _b;
	charCommand.a = _a;
	charCommand.bR = _bR;
	charCommand.bG = _bG;
	charCommand.bB = _bB;
	charCommand.bA = _bA;
	this->charCommandList.push_back(charCommand);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		描画 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/25	03:09	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11DebugFont::Draw(
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
	...)
{
	STR_COMMAND strCommand;


	va_list args;
	va_start(args, _str);
	ao_vsprintf_s(strCommand.buffer.data(), strCommand.buffer.size(), _str.c_str(), args);
	va_end(args);

	strCommand.x = (float)_x;
	strCommand.y = (float)(_y * -1.0f);
	strCommand.bWidth = (float)_bWidth;
	strCommand.bHeight = (float)_bHeight;
	strCommand.fontSize = (float)_fontSize;
	strCommand.r = _r;
	strCommand.g = _g;
	strCommand.b = _b;
	strCommand.a = _a;
	strCommand.bR = _bR;
	strCommand.bG = _bG;
	strCommand.bB = _bB;
	strCommand.bA = _bA;
	this->strCommandList.push_back(strCommand);

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		バースト描画 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/26	01:58	T.Aoyama	作成
//------------------------------------------------------------------------------
void DirectX11DebugFont::RenderBurst()
{
	this->pGraphicsState->SetDepthStecilState(FALSE);
	this->pGraphicsState->SetBlendState(BlendState::Alpha);
	this->pGraphicsState->SetRasterizerState(D3D11_FILL_SOLID);
	this->pd3dDeviceContext->IASetInputLayout(this->fontShader.inputLayout.GetComPtr());
	this->pd3dDeviceContext->VSSetShader(this->fontShader.vertex.GetComPtr(), nullptr, 0);
	this->pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->pd3dDeviceContext->PSSetShader(this->fontShader.pixel.GetComPtr(), nullptr, 0);

	::CopyMemory(&this->vsConst->matOrtho, &this->surfaceOrtho, sizeof(XMFLOAT4X4));
	for (auto& it : this->charCommandList)
	{
		this->OnDraw(it);
	}

	for (auto& it : this->strCommandList)
	{
		this->OnDraw(it);
	}

	this->charCommandList.clear();
	this->strCommandList.clear();
}


//------------------------------------------------------------------------------
/**	@brief		フォント変更 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/24	09:59	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11DebugFont::ChangeFont(const string& _fontName, const string& _fontPath)
{
	DESIGNVECTOR	design;
	::SecureZeroMemory(&design, sizeof(DESIGNVECTOR));

	//フォント名とフォントパス記憶
	this->fontName = _fontName;
	this->fontPath = _fontPath;

	// フォントの生成
	this->fontInfo.lfHeight = 32;
	this->fontInfo.lfWidth = 0;
	this->fontInfo.lfEscapement = 0;
	this->fontInfo.lfOrientation = 0;
	this->fontInfo.lfWeight = FW_THIN;
	this->fontInfo.lfItalic = 0;
	this->fontInfo.lfUnderline = 0;
	this->fontInfo.lfStrikeOut = 0;
	this->fontInfo.lfCharSet = SHIFTJIS_CHARSET;
	this->fontInfo.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	this->fontInfo.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	this->fontInfo.lfQuality = PROOF_QUALITY;
	this->fontInfo.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;

	if (!_fontName.empty())
	{
		ao_strcpy_s(this->fontInfo.lfFaceName, _fontName.size(), _fontName.c_str());
	}
	else
	{
		::SecureZeroMemory(this->fontInfo.lfFaceName, sizeof(this->fontInfo.lfFaceName));
	}

	//指定のフォントを扱えれるようにする
	if (AddFontResourceEx(this->fontPath.c_str(), FR_PRIVATE, &design) == 0)
	{
		return E_INVALIDARG;
	}

	//フォントシステムリソース削除
	if (RemoveFontResourceEx(this->fontPath.c_str(), FR_PRIVATE, &design) == 0)
	{
		return E_INVALIDARG;
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		サンプラーステートの作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/25	03:18	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT DirectX11DebugFont::CreateSamplerState()
{
	// テクスチャイメージの情報を設定
	D3D11_SAMPLER_DESC	samplerDesc;

	//テクスチャのサンプラステートを設定
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;	// サンプリング時に使用するフィルタ。
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 ～ 1 の範囲外にある u テクスチャー座標の描画方法
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 ～ 1 の範囲外にある v テクスチャー座標
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;		// 0 ～ 1 の範囲外にある w テクスチャー座標
	samplerDesc.MipLODBias = 0;								// 計算されたミップマップ レベルからのバイアス
	samplerDesc.MaxAnisotropy = 16;							// サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 。
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;	// 比較オプション。
	// 境界色をバーストコピー
	::CopyMemory(samplerDesc.BorderColor, &XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(XMFLOAT4));
	//ミップマップの設定
	samplerDesc.MinLOD = 0;							// アクセス可能なミップマップの下限値
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;			// アクセス可能なミップマップの上限値

	//サンプラーステート作成
	HRESULT hr = E_FAIL;
	hr = this->pd3dDevice->CreateSamplerState(&samplerDesc, this->pSamplerState.ToCreator());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr,_T("Failed create sampler state.\n"));

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		デフォルトスプライト作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/25	03:50	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT DirectX11DebugFont::CreateDefaultSprite()
{
	const float SpriteBuffer[4][5] =
	{
		{ 0 + 0.0f, 0 + 0.0f, 0, 0.0f, 0.0f },
		{ 0 + 1.0f, 0 + 0.0f, 0, 1.0f, 0.0f },
		{ 0 + 0.0f, 0 - 1.0f, 0, 0.0f, 1.0f },
		{ 0 + 1.0f, 0 - 1.0f, 0, 1.0f, 1.0f }
	};

	HRESULT hr = E_FAIL;
	hr = ao::CreateVertexBuffer(
		this->pd3dDevice.GetComPtr(),
		this->pSpriteBuffer.ToCreator(),
		sizeof(SpriteBuffer),
		SpriteBuffer);

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create sprite vertex buffer[Left top]\n"));


	return S_OK;
}

//------------------------------------------------------------------------------
/**	@brief		文字テクスチャ作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/25	09:24	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	ID3D11ShaderResourceView* DirectX11DebugFont::CreateCharacterTexture(UINT _code, int _fontSize)
{
	//登録してあるテクスチャ読み込み
	int64_t hash = (_code << 8) | _fontSize;
	if (this->charTextureMap.find(hash) != this->charTextureMap.end())
	{
		return this->charTextureMap[hash]->GetTexture();
	}

	DebugChar* pCharTex = new DebugChar;
	pCharTex->Create(this->pd3dDevice, this->pd3dDeviceContext, _code, this->fontInfo, _fontSize);

	//MAPに登録（排他的）
	this->charTextureMap.insert(std::map<int64_t, DebugChar*>::value_type(pCharTex->GetHash(), pCharTex));

	return pCharTex->GetTexture();
}


//------------------------------------------------------------------------------
/**	@brief		文字列テクスチャ作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/26	09:24	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	BaseTexture* DirectX11DebugFont::CreateStringTexture(const string& _str, int _fontSize)
{
	//登録してあるテクスチャ読み込み
	string hashString = _str;
	std::array<AOCHAR, 16> hashParts;
	ao_sprintf_s(hashParts.data(), 16, _T("_%d"), _fontSize);
	hashString.append(hashParts.data());

	if (this->strTextureMap.find(hashString) != this->strTextureMap.end())
		return this->strTextureMap[hashString]->GetRenderTexture(SamplerState::POINT);

	DirectX11RenderTarget* pRt = new DirectX11RenderTarget(this->pd3dDevice, this->pd3dDeviceContext);

	UINT w = 0;;
	UINT h = (UINT)_fontSize;
	for (UINT i = 0; i < _str.size(); ++i)
	{
		w += _fontSize *(macro::IsCheckEm(_str[i]) + 1);
	}

	MatrixOrthographicLH(&this->vsConst->matOrtho, (float)w, (float)h, 0, 1);
	this->vsConst->matOrtho._41 += 0 - 1.0f;
	this->vsConst->matOrtho._42 += 0 + 1.0f;
	MatrixTranspose(&this->vsConst->matOrtho, this->vsConst->matOrtho);

	pRt->Create(w, h, DXGI_FORMAT_R16G16B16A16_FLOAT);
	pRt->Clear(0, 0, 0, 0);
	pRt->SetRenderTarget(TRUE);

	CHAR_COMMAND temp = { 0, 0, 0, 0, (float)_fontSize, 0xFFFF, 1, 1, 1, 1, 0, 0, 0, 0 };
	ao_strcpy_s(temp.buffer.data(), temp.buffer.size(), _str.c_str());
	this->OnDraw(temp);

	pRt->RestoreRenderTarget();
	pRenderer->SetTexture(0, 0, nullptr, TRUE, TRUE);

	::CopyMemory(&this->vsConst->matOrtho, &this->surfaceOrtho, sizeof(XMFLOAT4X4));

	//MAPに登録（排他的）
	this->strTextureMap.insert(std::map<string, DirectX11RenderTarget*>::value_type(hashString, pRt));

	return pRt->GetRenderTexture(SamplerState::POINT);
}


//------------------------------------------------------------------------------
/**	@brief		文字一つの描画 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/26	02:05	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	void DirectX11DebugFont::OnDraw(const CHAR_COMMAND& _commandInfo)
{
	float w = 0;
	float h = (_commandInfo.bHeight == -1) ? _commandInfo.fontSize : _commandInfo.bHeight;
	if (_commandInfo.bWidth == -1)
	{
		for (UINT i = 0; i < ao_strlen(_commandInfo.buffer.data()); ++i)
		{
			w += (ao::macro::IsCheckEm(_commandInfo.buffer[i])) ? _commandInfo.fontSize : _commandInfo.fontSize / 2;
		}
	}
	else
	{
		w = _commandInfo.bWidth;
	}

	XMFLOAT4X4 offsetMatrix;
	ao::MatrixIdentity(&offsetMatrix);
	ao::MatrixSetOffsetScale(&offsetMatrix, _commandInfo.x, _commandInfo.y, 0, w, h, 0);
	ao::MatrixTranspose(&this->vsConst->matWorld, offsetMatrix);

	this->psConst->color.x = _commandInfo.bR;
	this->psConst->color.y = _commandInfo.bG;
	this->psConst->color.z = _commandInfo.bB;
	this->psConst->color.w = _commandInfo.bA;

	this->vsConst.MapVertex();
	this->psConst.MapPixel();

	//背景用の白テクスチャ描画
	UINT offset = 0;
	UINT stride = 20;
	this->pRenderer->SetTexture(0, 0, nullptr, TRUE, TRUE);
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->pSpriteBuffer.GetComPtrPtr(), &stride, &offset);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->pd3dDeviceContext->Draw(4, 0);

	//文字数分描画
	UINT	charCode = 0;
	FLOAT	offsetPos = 0;

	//描画座標計算ラムダ
	auto CalcPosition = [&](int i)
	{
#ifdef UNICODE
		FLOAT div = (2.0f - macro::IsCheckEm(_commandInfo.buffer[i]));
		charCode = static_cast<UINT>(_commandInfo.buffer[i]);
		MatrixSetOffsetScale(&offsetMatrix, _commandInfo.x + offsetPos, _commandInfo.y, 0, _commandInfo.fontSize / div, _commandInfo.fontSize, 0);
		offsetPos += _commandInfo.fontSize / div;
#else
		if (macro::CharToUINT(&charCode, &_commandInfo.buffer[i]))
		{
			//全角文字なら文字の大きさとオフセットは通常のまま
			MatrixSetOffsetScale(&offsetMatrix, _commandInfo.x + offsetPos, _commandInfo.y, 0, _commandInfo.fontSize, _commandInfo.fontSize, 0);
			offsetPos += (float)_commandInfo.fontSize;
			return 1;
		}
		else
		{
			//半角文字なら文字の大きさとオフセットを半分に
			MatrixSetOffsetScale(&offsetMatrix, _commandInfo.x + offsetPos, _commandInfo.y, 0, _commandInfo.fontSize / 2.0f, _commandInfo.fontSize, 0);
			offsetPos += (float)(_commandInfo.fontSize / 2.0f);
			return 0;
		}
#endif
		return 0;
	};

	//文字数分描画
	for (int i = offset; i < _commandInfo.numChar && _commandInfo.buffer[i] != '\0'; ++i)
	{
		i += CalcPosition(i);

		//シェーダーリソースビューの生成（フォントテクスチャ）
		ID3D11ShaderResourceView* pSrView = this->CreateCharacterTexture(charCode, (int)_commandInfo.fontSize);

		//ワールド行列設定
		MatrixTranspose(&this->vsConst->matWorld, offsetMatrix);

		this->psConst->color.x = _commandInfo.r;
		this->psConst->color.y = _commandInfo.g;
		this->psConst->color.z = _commandInfo.b;
		this->psConst->color.w = _commandInfo.a;

		//定数バッファMAP
		this->vsConst.MapVertex();
		this->psConst.MapPixel();

		//文字描画
		UINT offset = 0;
		UINT stride = 20;
		ID3D11ShaderResourceView* pSetCharTex[] = { pSrView };
		this->pRenderer->SetTexture(0, 0, pSetCharTex, this->pSamplerState.GetComPtrPtr());
		this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->pSpriteBuffer.GetComPtrPtr(), &stride, &offset);
		this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
		this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->pd3dDeviceContext->Draw(4, 0);
	}
}


//------------------------------------------------------------------------------
/**	@brief		文字列の描画 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/09/26	09:12	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	void DirectX11DebugFont::OnDraw(const STR_COMMAND& _commandInfo)
{
	float w = 0;
	float h = (_commandInfo.bHeight == -1) ? _commandInfo.fontSize : _commandInfo.bHeight;
	if (_commandInfo.bWidth == -1)
	{
		for (UINT i = 0; i < ao_strlen(_commandInfo.buffer.data()); ++i)
		{
			w += (ao::macro::IsCheckEm(_commandInfo.buffer[i])) ? _commandInfo.fontSize : _commandInfo.fontSize / 2;
		}
	}
	else
	{
		w = _commandInfo.bWidth;
	}

	XMFLOAT4X4 offsetMatrix;
	ao::MatrixIdentity(&offsetMatrix);
	ao::MatrixSetOffsetScale(&offsetMatrix, _commandInfo.x, _commandInfo.y, 0, w, h, 0);
	ao::MatrixTranspose(&this->vsConst->matWorld, offsetMatrix);

	this->psConst->color.x = _commandInfo.bR;
	this->psConst->color.y = _commandInfo.bG;
	this->psConst->color.z = _commandInfo.bB;
	this->psConst->color.w = _commandInfo.bA;

	this->vsConst.MapVertex();
	this->psConst.MapPixel();

	//背景用の白テクスチャ描画
	UINT offset = 0;
	UINT stride = 20;
	this->pRenderer->SetTexture(0, 0, nullptr, TRUE, TRUE);
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->pSpriteBuffer.GetComPtrPtr(), &stride, &offset);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->pd3dDeviceContext->Draw(4, 0);

	//文字数分描画
	UINT	charCode = 0;
	FLOAT	offsetPos = 0;

	//シェーダーリソースビューの生成（フォントテクスチャ）
	BaseTexture* pTexture = this->CreateStringTexture(_commandInfo.buffer.data(), (int)_commandInfo.fontSize);

	offsetMatrix._11 = 0;
	offsetMatrix._22 = _commandInfo.fontSize;
	for (int i = 0; _commandInfo.buffer[i] != '\0'; ++i)
	{
#ifdef UNICODE
		offsetMatrix._11 += _commandInfo.fontSize *(macro::IsCheckEm(_commandInfo.buffer[i]) + 1);
#else
		offsetMatrix._11 += _commandInfo.fontSize * (macro::CharToUINT(&charCode, &_commandInfo.buffer[i])+1);
#endif
	}

	//ワールド行列設定
	MatrixTranspose(&this->vsConst->matWorld, offsetMatrix);

	this->psConst->color.x = _commandInfo.r;
	this->psConst->color.y = _commandInfo.g;
	this->psConst->color.z = _commandInfo.b;
	this->psConst->color.w = _commandInfo.a;

	//定数バッファMAP
	this->vsConst.MapVertex();
	this->psConst.MapPixel();

	//文字描画
	this->pRenderer->SetTexture(0, 0, pTexture);
	this->pd3dDeviceContext->IASetVertexBuffers(0, 1, this->pSpriteBuffer.GetComPtrPtr(), &stride, &offset);
	this->pd3dDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
	this->pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->pd3dDeviceContext->Draw(4, 0);
}