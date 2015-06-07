#include"stdafx.hpp"
#include"DirectX11Application.hpp"
#include"DirectX11Adapter.hpp"
#include"DirectX11EnumDisplay.hpp"
#include"DirectX11RenderTarget.hpp"
#include"DirectX11Viewport.hpp"


using namespace ao;

#if defined(__AO_DEBUG__) || defined(__AO_RDEBUG__)
#define DIRECTX11_APP_CHECK_ERROR(_hr)	\
	if( FAILED(_hr) )					\
	{									\
		return E_FAIL;					\
	}
#else
//引数にメソッドや関数の戻り値を考慮している
#define DIRECTX11_APP_CHECK_ERROR(_hr) _hr;
#endif


DirectX11Application::DirectX11Application() :
	pAdapter(new DirectX11Adapter()),
	pEnumDisplay(new DirectX11EnumDisplay()),
	pBackBuffer(nullptr),
	bInitDisplay(false)
{

}


DirectX11Application::~DirectX11Application()
{
	delete this->pViewport;
	delete this->pBackBuffer;
	delete this->pEnumDisplay;
	delete this->pAdapter;
}


//------------------------------------------------------------------------------
/**	@brief		ディスプレイの初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	00:08	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11Application::InitDisplay(CONST DXGI_FORMAT _backBufferFormat)
{
	//アダプター作成
	DIRECTX11_APP_CHECK_ERROR(this->pAdapter->Create());
	this->bInitDisplay = true;

	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create pAdapter\n"));

	//アダプターに接続されているプライマリディスプレイを取得
	return this->pEnumDisplay->EnumDisplay(this->pAdapter->inst.GetComPtr(), _backBufferFormat);
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/06/27	20:45	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11Application::InitDirect3D(
	CONST HWND _hWnd,
	CONST UINT _nWidth,
	CONST UINT _nHeight,
	CONST DXGI_FORMAT _renderFormat,
	CONST BOOL _isWindowMode)
{
	this->hWnd = _hWnd;

	if (false == this->bInitDisplay)
	{
		DIRECTX11_APP_CHECK_ERROR(this->InitDisplay(_renderFormat));
	}

	DXGI_MODE_DESC desc;
	if (this->pEnumDisplay->GetEnumDisplayDesc(&desc, _nWidth, _nHeight) == nullptr)
	{
		AO_FAILED_FUNCTION_MESSAGE(_T("Can not found display format.\n"));
		return E_INVALIDARG;
	}

	//使用フォーマットを確定
	this->useFormat = _renderFormat;

	//デバイス作成
	DIRECTX11_APP_CHECK_ERROR(this->CreateDevice());

	//スワップチェイン作成
	DIRECTX11_APP_CHECK_ERROR(this->CreateSwapChain(&desc, false));

	//レンダーターゲット作成
	DIRECTX11_APP_CHECK_ERROR(this->CreateRenderTargetView());

	//深度ステンシル作成
	DIRECTX11_APP_CHECK_ERROR(this->CreateDepthStencilView());

	//一度レンダーターゲットを設定する
	ID3D11RenderTargetView* pSetRT[] = { this->pBackBuffer->pRtView.GetComPtr() };
	this->pd3dDeviceContext->OMSetRenderTargets(1, pSetRT, this->pBackBuffer->pDsView.GetComPtr());

	//ビューポートの作成
	DIRECTX11_APP_CHECK_ERROR(this->CreateViewport(_nWidth, _nHeight));

	//ウィンドウの設定
	DIRECTX11_APP_CHECK_ERROR(this->SetWindowAssociation());

	//デフォルトの深度ステンシルステートを設定
	DIRECTX11_APP_CHECK_ERROR(this->SetDefaultDepthStencilState());

	//フルスクリーンに変更
	if (_isWindowMode == FALSE)
	{
		DIRECTX11_APP_CHECK_ERROR(this->ChangeWindowMode());
	}

	//デバイスポインタ登録
	DIRECTX11_APP_CHECK_ERROR(this->pBackBuffer->Initialize());

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		ウィンドウモード変更 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/23	09:27	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11Application::ChangeWindowMode(CONST BOOL _isResize)
{
	HRESULT hr(E_FAIL);

#ifdef __AO_DEBUG__
	if (this->swapChain == nullptr)
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[%s]\nFailed swap chain is null.\n"),__AO_FUNCTION__);
		return hr;
	}
#endif

	DXGI_SWAP_CHAIN_DESC desc;
	hr = this->swapChain->GetDesc(&desc);

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get swap chain desc.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get swap chain desc.\n"));


	// GetFullscreenState
	BOOL fullScreen;
	hr = this->swapChain->GetFullscreenState(&fullScreen, nullptr);

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get full screen state\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get full screen state.\n"));


	// SetFullscreenState
	hr = this->swapChain->SetFullscreenState(!fullScreen, nullptr);

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed set full screen state\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded set full screen state\n"));


	switch (fullScreen)
	{
	case TRUE:
		ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Change window mode.\n"));
		this->isWindowMode = true;
		break;
	case FALSE:
		ao::debug::PrintfColor(ConsoleColor::H_CYAN, _T("Change full screen mode.\n"));
		this->isWindowMode = false;
		break;
	}

	// 初期起動をフルスクリーンモードにした場合、ウィンドウモードに変更すると
	// ウィンドウがアクティブにならないので表示させる。
	::ShowWindow(this->hWnd, SW_SHOW);

	// フルスクリーンからウィンドウモードに切り替えるとき WM_SIZE イベントが発生せず、結果的に IDXGISwapChain::ResizeBuffers がコールされない。
	// 環境にもよるようだが、画面上に何も表示されない現象が発生する可能性があるので
	// ChangeWindowModeOptimization() は ChangeWindowMode() 内でコールするように修正し、ここでの処理は無効にする
	if (_isResize)
	{
		hr = this->ChangeWindowModeOptimization(0, 0);
		AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed change window mode optimization.\n"));
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		デバイス作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	00:30	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT DirectX11Application::CreateDevice()
{
	HRESULT hr(E_FAIL);

	// デバイス作成時に使用するビデオカードの性能一覧
	D3D_FEATURE_LEVEL featureLevel[] =
	{
		D3D_FEATURE_LEVEL_11_1,	// Direct3D 11.1 SM 5
		D3D_FEATURE_LEVEL_11_0,	// Direct3D 11.0 SM 5
		D3D_FEATURE_LEVEL_10_1,	// Direct3D 10.1 SM 4
		D3D_FEATURE_LEVEL_10_0,	// Direct3D 10.0 SM 4
		D3D_FEATURE_LEVEL_9_3,	// Direct3D 9.3  SM 3
		D3D_FEATURE_LEVEL_9_2,	// Direct3D 9.2  SM 2
		D3D_FEATURE_LEVEL_9_1,	// Direct3D 9.1  SM 2
	};

	// 今回のFrameworkはDirectX11のみ対応
	const int featureCount = 2;

#ifdef __AO_DEBUG__
	// デバッグコンパイルの場合、デバッグレイヤーを有効にする。
	// ソフトウェア レイヤー
	UINT createDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
	ao::debug::PrintfColor(ConsoleColor::H_YELLOW, _T("\nUse debug layer.\n\n"));
#else
	UINT createDeviceFlag = 0;
#endif

	//採用するDirectXレベル
	D3D_FEATURE_LEVEL useFeatureLevel;

	// デバイスを作成する。
	hr = D3D11CreateDevice(
		this->pAdapter->inst.GetComPtr(),		// 使用するアダプターを設定。NULLの場合はデフォルトのアダプター。
		D3D_DRIVER_TYPE_UNKNOWN,				// D3D_DRIVER_TYPEのいずれか。ドライバーの種類。pAdapterが NULL 以外の場合は、D3D_DRIVER_TYPE_UNKNOWNを指定する。
		nullptr,								// ソフトウェアラスタライザを実装するDLLへのハンドル。D3D_DRIVER_TYPE を D3D_DRIVER_TYPE_SOFTWARE に設定している場合は NULL にできない。
		createDeviceFlag,						// D3D11_CREATE_DEVICE_FLAGの組み合わせ。デバイスを作成時に使用されるパラメータ。
		featureLevel,							// D3D_FEATURE_LEVELのポインタ
		featureCount,							// D3D_FEATURE_LEVEL配列の要素数
		D3D11_SDK_VERSION,						// DirectX SDKのバージョン。この値は固定。
		this->pd3dDevice.ToCreator(),			// 初期化されたデバイス
		&useFeatureLevel,						// 採用されたフィーチャーレベル
		this->pd3dDeviceContext.ToCreator());	// 初期化されたデバイスコンテキスト

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create device.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create device.\n"));

	// アダプターの情報を取得
	DXGI_ADAPTER_DESC desc;
	hr = this->pAdapter->inst->GetDesc(&desc);

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get adapter desc.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get adapter desc.\n"));

	// DirectXのバージョン、シェーダモデルのバージョンと採用したアダプターの名前をデバックウィンドウに出力
	switch (useFeatureLevel)
	{
	case D3D_FEATURE_LEVEL_11_1:
		ao::debug::PrintfColor(ConsoleColor::H_PURPLE, _T("Direct3D:11.1 SM:5 [%s]\n\n"), desc.Description);
		break;
	case D3D_FEATURE_LEVEL_11_0:
		ao::debug::PrintfColor(ConsoleColor::H_PURPLE, _T("Direct3D:11.0 SM:5 [%s]\n\n"), desc.Description);
		break;
	case D3D_FEATURE_LEVEL_10_1:
		ao::debug::PrintfColor(ConsoleColor::L_PURPLE, _T("Direct3D:10.1 SM:4 [%s]\n\n"), desc.Description);
		break;
	case D3D_FEATURE_LEVEL_10_0:
		ao::debug::PrintfColor(ConsoleColor::L_PURPLE, _T("Direct3D:10.0 SM:4 [%s]\n\n"), desc.Description);
		break;
	case D3D_FEATURE_LEVEL_9_3:
		ao::debug::PrintfColor(ConsoleColor::L_PURPLE, _T("Direct3D:9.3 SM:3 [%s]\n\n"), desc.Description);
		break;
	case D3D_FEATURE_LEVEL_9_2:
		ao::debug::PrintfColor(ConsoleColor::L_YELLOW, _T("Direct3D:9.2 SM:2 [%s]\n\n"), desc.Description);
		break;
	case D3D_FEATURE_LEVEL_9_1:
		ao::debug::PrintfColor(ConsoleColor::L_YELLOW, _T("Direct3D:9.1 SM:2 [%s]\n\n"), desc.Description);
		break;
	}

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		スワップチェイン作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	00:51	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT DirectX11Application::CreateSwapChain(
	CONST DXGI_MODE_DESC* _pDisplayMode,
	const bool _isMultiSampleEnable)
{
	HRESULT hr(E_FAIL);
	SmartComPtr<IDXGIFactory> pFactory;

	// DXGI_SWAP_CHAIN_DESC
	DXGI_SWAP_CHAIN_DESC sd;
	::SecureZeroMemory(&sd, sizeof(sd));

	// DXGI_MODE_DESC
	memcpy(&(sd.BufferDesc), _pDisplayMode, sizeof(DXGI_MODE_DESC));
	// バックバッファフォーマット
	sd.BufferDesc.Format = this->useFormat;
	// バックバッファのサーフェス使用法および CPU アクセス オプションを表す DXGI_USAGE 列挙型のメンバー
	// バックバッファにレンダリング可能にする。
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	//スワップ チェーンのバッファー数を表す値。
	sd.BufferCount = 1;
	// 出力ウィンドウへの HWND ハンドル。このメンバーを NULL にはできない。
	sd.OutputWindow = this->hWnd;
	// 出力がウィンドウ モードの場合は TRUE。それ以外の場合は FALSE。
	// 初期起動時の表示モードはウィンドウモードに固定とする。
	sd.Windowed = TRUE;
	// サーフェスのスワップ処理後にバックバッファの内容を保持するか。
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;           // バックバッファの内容を保持しない。
	// スワップ チェーンの動作のオプション。
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// フルスクリーンモードに切り替えると、アプリケーション ウィンドウのサイズに合わせて解像度が変更される。

	// ファクトリーを作成する
	// CreateDXGIFactoryで作成したファクトリーを使用すると実行できるがワーニングエラーになるので IDXGIAdapter から作成する。
	hr = this->pAdapter->inst->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get parent factory.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get parent factory.\n"));


	// マルチサンプリングの設定
	// マルチサンプルを使用しない場合の設定
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	int startMultisample;

	if (_isMultiSampleEnable == TRUE)
	{
		// サンプリング数が最大のものを採用する
		startMultisample = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
	}
	else
	{
		// マルチサンプルを無効にする。
		startMultisample = 1;
	}

	//マルチサンプルクオリティ
	UINT Quality = 0;


	// ====================================================================== ラムダ式
	//
	// 0 以外のときフォーマットとサンプリング数の組み合わせをサポートする
	// オンラインヘルプではCheckMultisampleQualityLevelsの戻り値が 0 のときサポートされないとかかれているが
	// pNumQualityLevels の戻り値が 0 かで判断する。
	// Direct3D 10 版 のオンラインヘルプにはそうかかれているので、オンラインヘルプの間違い。
	auto CreateQualitySwapChain = [&](int i)->bool
	{
		if (Quality == 0)
			return false;

		sd.SampleDesc.Count = i;
		sd.SampleDesc.Quality = Quality - 1;

		// スワップチェーンを作成する。
		hr = pFactory->CreateSwapChain(this->pd3dDevice.GetComPtr(), &sd, this->swapChain.ToCreator());

		if (SUCCEEDED(hr))
		{
			ao::debug::PrintfColor(ConsoleColor::H_GREEN, _T("MaltiSample count[%d] quality[%d] OK\n\n"), sd.SampleDesc.Count, sd.SampleDesc.Quality);
			return true;
		}
		else
		{
			ao::debug::PrintfColor(ConsoleColor::H_RED, _T("MaltiSample count[%d] quality[%d] NG\n"), sd.SampleDesc.Count, sd.SampleDesc.Quality);
			return false;
		}
	};
	//
	// ====================================================================== ラムダ式


	//クオリティに依存して制御
	for (int i = startMultisample; i >= 0; i--)
	{
		// サポートするクォリティレベルの最大値を取得する
		// CheckMultisampleQualityLevels
		hr = this->pd3dDevice->CheckMultisampleQualityLevels(_pDisplayMode->Format, (UINT)i, &Quality);

		AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed check multisample quality levels.\n"));
		AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded check multisample quality levels.\n\n"));
		//スワップチェインの作成
		if (CreateQualitySwapChain(i))
		{
			break;
		}
	}

#ifdef __AO_DEBUG__
	if (this->swapChain == nullptr)
	{
		AO_FAILED_FUNCTION_MESSAGE(_T("Failed create swap chain.\n"));
		return hr;
	}
#endif

	CONST INT _nWidth = _pDisplayMode->Width;
	CONST INT _nHeight = _pDisplayMode->Height;
	CONST INT denominator = _pDisplayMode->RefreshRate.Denominator;
	CONST INT numerator = _pDisplayMode->RefreshRate.Numerator;
	ao::debug::PrintfColor(ConsoleColor::H_GREEN, _T("[%s]\nSelect display mode - resolution[ %d：%d ] refresh Rate[ %d / %d ]\n\n"),__AO_FUNCTION__,_nWidth, _nHeight, denominator, numerator);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		レンダーターゲットビュー作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	01:39	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT DirectX11Application::CreateRenderTargetView()
{
	HRESULT hr(S_OK);

	//バックバッファ生成
	this->pBackBuffer = new DirectX11RenderTarget(this->pd3dDevice, this->pd3dDeviceContext);

	// スワップチェーンからバックバッファを取得する
	hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)this->pBackBuffer->pRtTex.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get buffer.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get buffer.\n"));


	//シェーダーリソースビュー作成
	hr = this->pd3dDevice->CreateShaderResourceView(this->pBackBuffer->pRtTex.GetComPtr(), nullptr, this->pBackBuffer->pRtSrView.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create shader resource view.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create shader resource view.\n"));


	// レンダリングターゲットビューの作成
	hr = this->pd3dDevice->CreateRenderTargetView(this->pBackBuffer->pRtTex.GetComPtr(), nullptr, this->pBackBuffer->pRtView.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create render target view.\n\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create render target view.\n\n"));


	//レンダーターゲットのテクスチャ設定
	this->pBackBuffer->rtTexture.pSrView = this->pBackBuffer->pRtSrView;

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		深度ステンシル作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	01:39	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT DirectX11Application::CreateDepthStencilView()
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC chainDesc;
	hr = this->swapChain->GetDesc(&chainDesc);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get chain desc\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get swap chain desc\n"));


	D3D11_TEXTURE2D_DESC descDS;
	::SecureZeroMemory(&descDS, sizeof(descDS));
	descDS.Width = chainDesc.BufferDesc.Width;					// バックバッファと同じにする。
	descDS.Height = chainDesc.BufferDesc.Height;				// バックバッファと同じにする。
	descDS.MipLevels = 1;										// ミップマップを作成しない
	descDS.ArraySize = 1;										// テクスチャーの配列数
	descDS.Format = DXGI_FORMAT_R32_TYPELESS;					// フォーマット
	descDS.SampleDesc.Count = chainDesc.SampleDesc.Count;		// バックバッファと同じにする。
	descDS.SampleDesc.Quality = chainDesc.SampleDesc.Quality;	// バックバッファと同じにする。
	descDS.Usage = D3D11_USAGE_DEFAULT;							// GPU による読み取りおよび書き込みアクセスを必要とするリソース。
	descDS.CPUAccessFlags = 0;									// CPU アクセスが不要。
	descDS.MiscFlags = 0;										// その他のフラグも設定しない。

	// 深度ステンシルバッファとして作成する+シェーダーリソースビューとして作成する
	descDS.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	// 深度バッファ用のテクスチャー作成
	hr = this->pd3dDevice->CreateTexture2D(&descDS, nullptr, this->pBackBuffer->pDsTex.ToCreator());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create depth buffer texture.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create depth buffer texture.\n"));


	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	::SecureZeroMemory(&descDSV, sizeof(descDSV));
	::SecureZeroMemory(&descSRV, sizeof(descSRV));

	// テクスチャー作成時に指定したフォーマットと互換性があり、
	// 深度ステンシルビューとして指定できるフォーマットを指定する
	switch (descDS.Format)
	{
	case DXGI_FORMAT_R16_TYPELESS:
		descDSV.Format = DXGI_FORMAT_D16_UNORM;
		descSRV.Format = DXGI_FORMAT_R16_UNORM;
		break;
	case DXGI_FORMAT_R32_TYPELESS:
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descSRV.Format = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_R24G8_TYPELESS:
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descSRV.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	default:
		descDSV.Format = descDS.Format;
	}

	descSRV.Texture2D.MipLevels = 1;
	descDSV.Texture2D.MipSlice = 0;

	if (chainDesc.SampleDesc.Count > 1)
	{
		// マルチサンプルを使用している場合
		descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		// マルチサンプルを使用していない場合
		descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	}

	//シェーダーリソースビュー作成
	hr = this->pd3dDevice->CreateShaderResourceView(this->pBackBuffer->pDsTex.GetComPtr(), &descSRV, this->pBackBuffer->pDsSrView.ToCreator());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create shader resource view.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create shader resource view.\n"));


	// 深度ステンシルビューを作成
	hr = this->pd3dDevice->CreateDepthStencilView(this->pBackBuffer->pDsTex.GetComPtr(), &descDSV, this->pBackBuffer->pDsView.ToCreator());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create depth stencil view.\n\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create depth stencil view.\n\n"));


	//深度ステンシルテクスチャ設定
	this->pBackBuffer->dsTexture.pSrView = this->pBackBuffer->pDsSrView;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		ビューポートの作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	02:58	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11Application::CreateViewport(UINT _nWidth, UINT _nHeight)
{
	HRESULT hr(E_FAIL);

#ifdef __AO_DEBUG__
	if (this->swapChain == nullptr)
	{
		AO_FAILED_FUNCTION_MESSAGE(_T("Failed swap Chain is null.\n"));
		return hr;
	}
#endif

	DXGI_SWAP_CHAIN_DESC desc;
	// スワップチェーンの設定を取得する
	hr = this->swapChain->GetDesc(&desc);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get chain desc.\n\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get chain desc.\n\n"));


	//ビューポート作成
	this->pViewport = new DirectX11Viewport(this->pd3dDevice, this->pd3dDeviceContext);
	this->pViewport->Create(_nWidth, _nHeight);
	this->pViewport->SetViewport();

	//レンダーターゲットにビューポートを設定
	::CopyMemory(&this->pBackBuffer->viewport, &this->pViewport->viewport, sizeof(D3D11_VIEWPORT));

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		ウィンドウの状態を設定 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	03:09	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT DirectX11Application::SetWindowAssociation()
{
	HRESULT hr = E_FAIL;
	SmartComPtr<IDXGIFactory> pFactory;

	// ファクトリーを作成する
	// CreateDXGIFactoryで作成したファクトリーを使用すると実行できるがワーニングエラーになるので
	// IDXGIAdapter から作成する。
	hr = this->pAdapter->inst->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get parent factory.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get parent factory.\n"));


	// 表示モードの自動切換えを無効にする。
	// MakeWindowAssociation
	hr = pFactory->MakeWindowAssociation(this->hWnd, DXGI_MWA_NO_WINDOW_CHANGES);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed make window association.\n\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded make window association.\n\n"));

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		ウィンドウモード変更の最適化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/23	09:33	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT DirectX11Application::ChangeWindowModeOptimization(CONST UINT _nWidth, CONST UINT _nHeight)
{
	HRESULT hr = S_OK;

#ifdef __AO_DEBUG__
	if (this->swapChain == nullptr)
	{
		AO_FAILED_FUNCTION_MESSAGE(_T("Failed swap chain is null.\n"));
		return hr;
	}
#endif

	DXGI_SWAP_CHAIN_DESC desc;
	hr = this->swapChain->GetDesc(&desc);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get swap chain desc.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get swap chain desc.\n"));


	// OMSetRenderTargets
	this->pd3dDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	// スワップ チェーンのバック バッファー サイズ、フォーマット、およびバッファー数を変更する。
	// アプリケーション ウィンドウのサイズが変更されたときに呼び出す必要がある。
	hr = this->swapChain->ResizeBuffers(desc.BufferCount, _nWidth, _nHeight, desc.BufferDesc.Format, desc.Flags);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed resize swap chain back buffers _size format.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded resize swap chain back buffers _size format.\n"));


	// レンダリングターゲットビューの作成
	hr = this->CreateRenderTargetView();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create render target view.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create render target view.\n"));


	// 深度ステンシルビューの作成
	hr = this->CreateDepthStencilView();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create depth stencil view.\n\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create depth stencil view.\n\n"));


	//レンダーターゲットの設定
	this->pBackBuffer->SetRenderTarget();

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		デフォルトのラスタライザを設定 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	03:28	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT DirectX11Application::SetDefaultRasterize()
{
	HRESULT hr = E_FAIL;

	// ID3D11RasterizerState
	SmartComPtr<ID3D11RasterizerState> pRasterState;
	// D3D11_RASTERIZER_DESC
	D3D11_RASTERIZER_DESC rsState;

	::SecureZeroMemory(&rsState, sizeof(D3D11_RASTERIZER_DESC));
	rsState.FillMode = D3D11_FILL_SOLID;		// ポリゴン面描画
	rsState.CullMode = D3D11_CULL_BACK;			// 裏面を描画しない
	rsState.FrontCounterClockwise = FALSE;		// 反時計回りを表面（左手系）
	rsState.DepthBias = 0;						// 深度バイアス設定
	rsState.DepthBiasClamp = 0;					// 深度バイアスのクランプ値を設定
	rsState.SlopeScaledDepthBias = 0;			// 深度バイアスの数を設定
	rsState.DepthClipEnable = TRUE;				// 深度クリップの有効化
	rsState.ScissorEnable = FALSE;				// シザー矩形無効
	rsState.AntialiasedLineEnable = TRUE;		// 線のアンチエイリアス有効

	// スワップチェーンのマルチサンプリングの設定にあわせる
	DXGI_SWAP_CHAIN_DESC swapDesc;

	hr = this->swapChain->GetDesc(&swapDesc);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get swap chain desc.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get swap chain desc.\n"));

	if (swapDesc.SampleDesc.Count != 1)
	{
		rsState.MultisampleEnable = TRUE;
	}
	else
	{
		rsState.MultisampleEnable = FALSE;
	}

	// CreateRasterizerState
	hr = this->pd3dDevice->CreateRasterizerState(&rsState, &pRasterState);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create rasterize state.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create rasterize state.\n"));


	// RSSetState
	this->pd3dDeviceContext->RSSetState(pRasterState.GetComPtr());

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		デフォルトの深度ステンシルステートを設定 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	03:37	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	HRESULT DirectX11Application::SetDefaultDepthStencilState()
{
	HRESULT hr = E_FAIL;

	D3D11_DEPTH_STENCIL_DESC dsState;
	SmartComPtr<ID3D11DepthStencilState> pDepthStencilState;

	::SecureZeroMemory(&dsState, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsState.DepthEnable = TRUE;								// 深度テストの有効化
	dsState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	// 深度ステンシルバッファの書き込み設定
	dsState.DepthFunc = D3D11_COMPARISON_LESS;				// 深度データの比較設定
	dsState.StencilEnable = FALSE;							// ステンシル無効化

	//深度ステンシルステート設定
	hr = this->pd3dDevice->CreateDepthStencilState(&dsState, &pDepthStencilState);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create depth stencil state.\n\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create depth stencil state.\n\n"));


	//深度ステンシルの状態を設定
	this->pd3dDeviceContext->OMSetDepthStencilState(pDepthStencilState.GetComPtr(), 0);

	return hr;
}