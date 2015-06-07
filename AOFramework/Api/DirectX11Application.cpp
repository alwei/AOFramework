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
//�����Ƀ��\�b�h��֐��̖߂�l���l�����Ă���
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
/**	@brief		�f�B�X�v���C�̏����� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	00:08	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11Application::InitDisplay(CONST DXGI_FORMAT _backBufferFormat)
{
	//�A�_�v�^�[�쐬
	DIRECTX11_APP_CHECK_ERROR(this->pAdapter->Create());
	this->bInitDisplay = true;

	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create pAdapter\n"));

	//�A�_�v�^�[�ɐڑ�����Ă���v���C�}���f�B�X�v���C���擾
	return this->pEnumDisplay->EnumDisplay(this->pAdapter->inst.GetComPtr(), _backBufferFormat);
}


//------------------------------------------------------------------------------
/**	@brief		������ */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/06/27	20:45	T.Aoyama	�쐬
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

	//�g�p�t�H�[�}�b�g���m��
	this->useFormat = _renderFormat;

	//�f�o�C�X�쐬
	DIRECTX11_APP_CHECK_ERROR(this->CreateDevice());

	//�X���b�v�`�F�C���쐬
	DIRECTX11_APP_CHECK_ERROR(this->CreateSwapChain(&desc, false));

	//�����_�[�^�[�Q�b�g�쐬
	DIRECTX11_APP_CHECK_ERROR(this->CreateRenderTargetView());

	//�[�x�X�e���V���쐬
	DIRECTX11_APP_CHECK_ERROR(this->CreateDepthStencilView());

	//��x�����_�[�^�[�Q�b�g��ݒ肷��
	ID3D11RenderTargetView* pSetRT[] = { this->pBackBuffer->pRtView.GetComPtr() };
	this->pd3dDeviceContext->OMSetRenderTargets(1, pSetRT, this->pBackBuffer->pDsView.GetComPtr());

	//�r���[�|�[�g�̍쐬
	DIRECTX11_APP_CHECK_ERROR(this->CreateViewport(_nWidth, _nHeight));

	//�E�B���h�E�̐ݒ�
	DIRECTX11_APP_CHECK_ERROR(this->SetWindowAssociation());

	//�f�t�H���g�̐[�x�X�e���V���X�e�[�g��ݒ�
	DIRECTX11_APP_CHECK_ERROR(this->SetDefaultDepthStencilState());

	//�t���X�N���[���ɕύX
	if (_isWindowMode == FALSE)
	{
		DIRECTX11_APP_CHECK_ERROR(this->ChangeWindowMode());
	}

	//�f�o�C�X�|�C���^�o�^
	DIRECTX11_APP_CHECK_ERROR(this->pBackBuffer->Initialize());

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�E�B���h�E���[�h�ύX */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/23	09:27	T.Aoyama	�쐬
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

	// �����N�����t���X�N���[�����[�h�ɂ����ꍇ�A�E�B���h�E���[�h�ɕύX�����
	// �E�B���h�E���A�N�e�B�u�ɂȂ�Ȃ��̂ŕ\��������B
	::ShowWindow(this->hWnd, SW_SHOW);

	// �t���X�N���[������E�B���h�E���[�h�ɐ؂�ւ���Ƃ� WM_SIZE �C�x���g�����������A���ʓI�� IDXGISwapChain::ResizeBuffers ���R�[������Ȃ��B
	// ���ɂ����悤�����A��ʏ�ɉ����\������Ȃ����ۂ���������\��������̂�
	// ChangeWindowModeOptimization() �� ChangeWindowMode() ���ŃR�[������悤�ɏC�����A�����ł̏����͖����ɂ���
	if (_isResize)
	{
		hr = this->ChangeWindowModeOptimization(0, 0);
		AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed change window mode optimization.\n"));
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�f�o�C�X�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	00:30	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	HRESULT DirectX11Application::CreateDevice()
{
	HRESULT hr(E_FAIL);

	// �f�o�C�X�쐬���Ɏg�p����r�f�I�J�[�h�̐��\�ꗗ
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

	// �����Framework��DirectX11�̂ݑΉ�
	const int featureCount = 2;

#ifdef __AO_DEBUG__
	// �f�o�b�O�R���p�C���̏ꍇ�A�f�o�b�O���C���[��L���ɂ���B
	// �\�t�g�E�F�A ���C���[
	UINT createDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
	ao::debug::PrintfColor(ConsoleColor::H_YELLOW, _T("\nUse debug layer.\n\n"));
#else
	UINT createDeviceFlag = 0;
#endif

	//�̗p����DirectX���x��
	D3D_FEATURE_LEVEL useFeatureLevel;

	// �f�o�C�X���쐬����B
	hr = D3D11CreateDevice(
		this->pAdapter->inst.GetComPtr(),		// �g�p����A�_�v�^�[��ݒ�BNULL�̏ꍇ�̓f�t�H���g�̃A�_�v�^�[�B
		D3D_DRIVER_TYPE_UNKNOWN,				// D3D_DRIVER_TYPE�̂����ꂩ�B�h���C�o�[�̎�ށBpAdapter�� NULL �ȊO�̏ꍇ�́AD3D_DRIVER_TYPE_UNKNOWN���w�肷��B
		nullptr,								// �\�t�g�E�F�A���X�^���C�U����������DLL�ւ̃n���h���BD3D_DRIVER_TYPE �� D3D_DRIVER_TYPE_SOFTWARE �ɐݒ肵�Ă���ꍇ�� NULL �ɂł��Ȃ��B
		createDeviceFlag,						// D3D11_CREATE_DEVICE_FLAG�̑g�ݍ��킹�B�f�o�C�X���쐬���Ɏg�p�����p�����[�^�B
		featureLevel,							// D3D_FEATURE_LEVEL�̃|�C���^
		featureCount,							// D3D_FEATURE_LEVEL�z��̗v�f��
		D3D11_SDK_VERSION,						// DirectX SDK�̃o�[�W�����B���̒l�͌Œ�B
		this->pd3dDevice.ToCreator(),			// ���������ꂽ�f�o�C�X
		&useFeatureLevel,						// �̗p���ꂽ�t�B�[�`���[���x��
		this->pd3dDeviceContext.ToCreator());	// ���������ꂽ�f�o�C�X�R���e�L�X�g

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create device.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create device.\n"));

	// �A�_�v�^�[�̏����擾
	DXGI_ADAPTER_DESC desc;
	hr = this->pAdapter->inst->GetDesc(&desc);

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get adapter desc.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get adapter desc.\n"));

	// DirectX�̃o�[�W�����A�V�F�[�_���f���̃o�[�W�����ƍ̗p�����A�_�v�^�[�̖��O���f�o�b�N�E�B���h�E�ɏo��
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
/**	@brief		�X���b�v�`�F�C���쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	00:51	T.Aoyama	�쐬
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
	// �o�b�N�o�b�t�@�t�H�[�}�b�g
	sd.BufferDesc.Format = this->useFormat;
	// �o�b�N�o�b�t�@�̃T�[�t�F�X�g�p�@����� CPU �A�N�Z�X �I�v�V������\�� DXGI_USAGE �񋓌^�̃����o�[
	// �o�b�N�o�b�t�@�Ƀ����_�����O�\�ɂ���B
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	//�X���b�v �`�F�[���̃o�b�t�@�[����\���l�B
	sd.BufferCount = 1;
	// �o�̓E�B���h�E�ւ� HWND �n���h���B���̃����o�[�� NULL �ɂ͂ł��Ȃ��B
	sd.OutputWindow = this->hWnd;
	// �o�͂��E�B���h�E ���[�h�̏ꍇ�� TRUE�B����ȊO�̏ꍇ�� FALSE�B
	// �����N�����̕\�����[�h�̓E�B���h�E���[�h�ɌŒ�Ƃ���B
	sd.Windowed = TRUE;
	// �T�[�t�F�X�̃X���b�v������Ƀo�b�N�o�b�t�@�̓��e��ێ����邩�B
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;           // �o�b�N�o�b�t�@�̓��e��ێ����Ȃ��B
	// �X���b�v �`�F�[���̓���̃I�v�V�����B
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// �t���X�N���[�����[�h�ɐ؂�ւ���ƁA�A�v���P�[�V���� �E�B���h�E�̃T�C�Y�ɍ��킹�ĉ𑜓x���ύX�����B

	// �t�@�N�g���[���쐬����
	// CreateDXGIFactory�ō쐬�����t�@�N�g���[���g�p����Ǝ��s�ł��邪���[�j���O�G���[�ɂȂ�̂� IDXGIAdapter ����쐬����B
	hr = this->pAdapter->inst->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get parent factory.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get parent factory.\n"));


	// �}���`�T���v�����O�̐ݒ�
	// �}���`�T���v�����g�p���Ȃ��ꍇ�̐ݒ�
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	int startMultisample;

	if (_isMultiSampleEnable == TRUE)
	{
		// �T���v�����O�����ő�̂��̂��̗p����
		startMultisample = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
	}
	else
	{
		// �}���`�T���v���𖳌��ɂ���B
		startMultisample = 1;
	}

	//�}���`�T���v���N�I���e�B
	UINT Quality = 0;


	// ====================================================================== �����_��
	//
	// 0 �ȊO�̂Ƃ��t�H�[�}�b�g�ƃT���v�����O���̑g�ݍ��킹���T�|�[�g����
	// �I�����C���w���v�ł�CheckMultisampleQualityLevels�̖߂�l�� 0 �̂Ƃ��T�|�[�g����Ȃ��Ƃ�����Ă��邪
	// pNumQualityLevels �̖߂�l�� 0 ���Ŕ��f����B
	// Direct3D 10 �� �̃I�����C���w���v�ɂ͂���������Ă���̂ŁA�I�����C���w���v�̊ԈႢ�B
	auto CreateQualitySwapChain = [&](int i)->bool
	{
		if (Quality == 0)
			return false;

		sd.SampleDesc.Count = i;
		sd.SampleDesc.Quality = Quality - 1;

		// �X���b�v�`�F�[�����쐬����B
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
	// ====================================================================== �����_��


	//�N�I���e�B�Ɉˑ����Đ���
	for (int i = startMultisample; i >= 0; i--)
	{
		// �T�|�[�g����N�H���e�B���x���̍ő�l���擾����
		// CheckMultisampleQualityLevels
		hr = this->pd3dDevice->CheckMultisampleQualityLevels(_pDisplayMode->Format, (UINT)i, &Quality);

		AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed check multisample quality levels.\n"));
		AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded check multisample quality levels.\n\n"));
		//�X���b�v�`�F�C���̍쐬
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
	ao::debug::PrintfColor(ConsoleColor::H_GREEN, _T("[%s]\nSelect display mode - resolution[ %d�F%d ] refresh Rate[ %d / %d ]\n\n"),__AO_FUNCTION__,_nWidth, _nHeight, denominator, numerator);

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�����_�[�^�[�Q�b�g�r���[�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	01:39	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	HRESULT DirectX11Application::CreateRenderTargetView()
{
	HRESULT hr(S_OK);

	//�o�b�N�o�b�t�@����
	this->pBackBuffer = new DirectX11RenderTarget(this->pd3dDevice, this->pd3dDeviceContext);

	// �X���b�v�`�F�[������o�b�N�o�b�t�@���擾����
	hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)this->pBackBuffer->pRtTex.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get buffer.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get buffer.\n"));


	//�V�F�[�_�[���\�[�X�r���[�쐬
	hr = this->pd3dDevice->CreateShaderResourceView(this->pBackBuffer->pRtTex.GetComPtr(), nullptr, this->pBackBuffer->pRtSrView.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create shader resource view.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create shader resource view.\n"));


	// �����_�����O�^�[�Q�b�g�r���[�̍쐬
	hr = this->pd3dDevice->CreateRenderTargetView(this->pBackBuffer->pRtTex.GetComPtr(), nullptr, this->pBackBuffer->pRtView.ToCreator());

	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create render target view.\n\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create render target view.\n\n"));


	//�����_�[�^�[�Q�b�g�̃e�N�X�`���ݒ�
	this->pBackBuffer->rtTexture.pSrView = this->pBackBuffer->pRtSrView;

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�[�x�X�e���V���쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	01:39	T.Aoyama	�쐬
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
	descDS.Width = chainDesc.BufferDesc.Width;					// �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.Height = chainDesc.BufferDesc.Height;				// �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.MipLevels = 1;										// �~�b�v�}�b�v���쐬���Ȃ�
	descDS.ArraySize = 1;										// �e�N�X�`���[�̔z��
	descDS.Format = DXGI_FORMAT_R32_TYPELESS;					// �t�H�[�}�b�g
	descDS.SampleDesc.Count = chainDesc.SampleDesc.Count;		// �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.SampleDesc.Quality = chainDesc.SampleDesc.Quality;	// �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.Usage = D3D11_USAGE_DEFAULT;							// GPU �ɂ��ǂݎ�肨��я������݃A�N�Z�X��K�v�Ƃ��郊�\�[�X�B
	descDS.CPUAccessFlags = 0;									// CPU �A�N�Z�X���s�v�B
	descDS.MiscFlags = 0;										// ���̑��̃t���O���ݒ肵�Ȃ��B

	// �[�x�X�e���V���o�b�t�@�Ƃ��č쐬����+�V�F�[�_�[���\�[�X�r���[�Ƃ��č쐬����
	descDS.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	// �[�x�o�b�t�@�p�̃e�N�X�`���[�쐬
	hr = this->pd3dDevice->CreateTexture2D(&descDS, nullptr, this->pBackBuffer->pDsTex.ToCreator());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create depth buffer texture.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create depth buffer texture.\n"));


	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	::SecureZeroMemory(&descDSV, sizeof(descDSV));
	::SecureZeroMemory(&descSRV, sizeof(descSRV));

	// �e�N�X�`���[�쐬���Ɏw�肵���t�H�[�}�b�g�ƌ݊���������A
	// �[�x�X�e���V���r���[�Ƃ��Ďw��ł���t�H�[�}�b�g���w�肷��
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
		// �}���`�T���v�����g�p���Ă���ꍇ
		descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		// �}���`�T���v�����g�p���Ă��Ȃ��ꍇ
		descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	}

	//�V�F�[�_�[���\�[�X�r���[�쐬
	hr = this->pd3dDevice->CreateShaderResourceView(this->pBackBuffer->pDsTex.GetComPtr(), &descSRV, this->pBackBuffer->pDsSrView.ToCreator());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create shader resource view.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create shader resource view.\n"));


	// �[�x�X�e���V���r���[���쐬
	hr = this->pd3dDevice->CreateDepthStencilView(this->pBackBuffer->pDsTex.GetComPtr(), &descDSV, this->pBackBuffer->pDsView.ToCreator());
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create depth stencil view.\n\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create depth stencil view.\n\n"));


	//�[�x�X�e���V���e�N�X�`���ݒ�
	this->pBackBuffer->dsTexture.pSrView = this->pBackBuffer->pDsSrView;

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�r���[�|�[�g�̍쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	02:58	T.Aoyama	�쐬
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
	// �X���b�v�`�F�[���̐ݒ���擾����
	hr = this->swapChain->GetDesc(&desc);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get chain desc.\n\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get chain desc.\n\n"));


	//�r���[�|�[�g�쐬
	this->pViewport = new DirectX11Viewport(this->pd3dDevice, this->pd3dDeviceContext);
	this->pViewport->Create(_nWidth, _nHeight);
	this->pViewport->SetViewport();

	//�����_�[�^�[�Q�b�g�Ƀr���[�|�[�g��ݒ�
	::CopyMemory(&this->pBackBuffer->viewport, &this->pViewport->viewport, sizeof(D3D11_VIEWPORT));

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�E�B���h�E�̏�Ԃ�ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	03:09	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11Application::SetWindowAssociation()
{
	HRESULT hr = E_FAIL;
	SmartComPtr<IDXGIFactory> pFactory;

	// �t�@�N�g���[���쐬����
	// CreateDXGIFactory�ō쐬�����t�@�N�g���[���g�p����Ǝ��s�ł��邪���[�j���O�G���[�ɂȂ�̂�
	// IDXGIAdapter ����쐬����B
	hr = this->pAdapter->inst->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed get parent factory.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded get parent factory.\n"));


	// �\�����[�h�̎����؊����𖳌��ɂ���B
	// MakeWindowAssociation
	hr = pFactory->MakeWindowAssociation(this->hWnd, DXGI_MWA_NO_WINDOW_CHANGES);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed make window association.\n\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded make window association.\n\n"));

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�E�B���h�E���[�h�ύX�̍œK�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/23	09:33	T.Aoyama	�쐬
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

	// �X���b�v �`�F�[���̃o�b�N �o�b�t�@�[ �T�C�Y�A�t�H�[�}�b�g�A����уo�b�t�@�[����ύX����B
	// �A�v���P�[�V���� �E�B���h�E�̃T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo���K�v������B
	hr = this->swapChain->ResizeBuffers(desc.BufferCount, _nWidth, _nHeight, desc.BufferDesc.Format, desc.Flags);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed resize swap chain back buffers _size format.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded resize swap chain back buffers _size format.\n"));


	// �����_�����O�^�[�Q�b�g�r���[�̍쐬
	hr = this->CreateRenderTargetView();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create render target view.\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create render target view.\n"));


	// �[�x�X�e���V���r���[�̍쐬
	hr = this->CreateDepthStencilView();
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create depth stencil view.\n\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create depth stencil view.\n\n"));


	//�����_�[�^�[�Q�b�g�̐ݒ�
	this->pBackBuffer->SetRenderTarget();

	return hr;
}


//------------------------------------------------------------------------------
/**	@brief		�f�t�H���g�̃��X�^���C�U��ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	03:28	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	HRESULT DirectX11Application::SetDefaultRasterize()
{
	HRESULT hr = E_FAIL;

	// ID3D11RasterizerState
	SmartComPtr<ID3D11RasterizerState> pRasterState;
	// D3D11_RASTERIZER_DESC
	D3D11_RASTERIZER_DESC rsState;

	::SecureZeroMemory(&rsState, sizeof(D3D11_RASTERIZER_DESC));
	rsState.FillMode = D3D11_FILL_SOLID;		// �|���S���ʕ`��
	rsState.CullMode = D3D11_CULL_BACK;			// ���ʂ�`�悵�Ȃ�
	rsState.FrontCounterClockwise = FALSE;		// �����v����\�ʁi����n�j
	rsState.DepthBias = 0;						// �[�x�o�C�A�X�ݒ�
	rsState.DepthBiasClamp = 0;					// �[�x�o�C�A�X�̃N�����v�l��ݒ�
	rsState.SlopeScaledDepthBias = 0;			// �[�x�o�C�A�X�̐���ݒ�
	rsState.DepthClipEnable = TRUE;				// �[�x�N���b�v�̗L����
	rsState.ScissorEnable = FALSE;				// �V�U�[��`����
	rsState.AntialiasedLineEnable = TRUE;		// ���̃A���`�G�C���A�X�L��

	// �X���b�v�`�F�[���̃}���`�T���v�����O�̐ݒ�ɂ��킹��
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
/**	@brief		�f�t�H���g�̐[�x�X�e���V���X�e�[�g��ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/16	03:37	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	HRESULT DirectX11Application::SetDefaultDepthStencilState()
{
	HRESULT hr = E_FAIL;

	D3D11_DEPTH_STENCIL_DESC dsState;
	SmartComPtr<ID3D11DepthStencilState> pDepthStencilState;

	::SecureZeroMemory(&dsState, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsState.DepthEnable = TRUE;								// �[�x�e�X�g�̗L����
	dsState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	// �[�x�X�e���V���o�b�t�@�̏������ݐݒ�
	dsState.DepthFunc = D3D11_COMPARISON_LESS;				// �[�x�f�[�^�̔�r�ݒ�
	dsState.StencilEnable = FALSE;							// �X�e���V��������

	//�[�x�X�e���V���X�e�[�g�ݒ�
	hr = this->pd3dDevice->CreateDepthStencilState(&dsState, &pDepthStencilState);
	AO_CHECK_ERROR_RETURN_HRESULT_MESSAGE(hr, _T("Failed create depth stencil state.\n\n"));
	AO_SUCCEEDED_FUNCTION_MESSAGE(_T("Succedded create depth stencil state.\n\n"));


	//�[�x�X�e���V���̏�Ԃ�ݒ�
	this->pd3dDeviceContext->OMSetDepthStencilState(pDepthStencilState.GetComPtr(), 0);

	return hr;
}