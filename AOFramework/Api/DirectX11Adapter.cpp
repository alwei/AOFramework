#include"stdafx.hpp"
#include"DirectX11Adapter.hpp"
#include"../Utility/Macro.inl"
#include"../Debug/DebugSystem.hpp"


using namespace ao;


//�ÓI�����o�̎��̉�
SmartComPtr<IDXGIFactory> DirectX11Adapter::factory;


DirectX11Adapter::DirectX11Adapter()
{
	
}


DirectX11Adapter::~DirectX11Adapter()
{

}


//------------------------------------------------------------------------------
/**	@brief		�A�_�v�^�[�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/15	23:02	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT DirectX11Adapter::Create()
{
	HRESULT hr = S_FALSE;

	if (this->inst != nullptr)
	{
#ifdef __AO_DEBUG__
		ao::debug::PrintfColor(ConsoleColor::H_YELLOW, _T("[DirectX11Adapter::Create]Adapter has already been created.\n"));
		return hr;
#endif
	}

	//�t�@�N�g���[�̍쐬
	if (DirectX11Adapter::factory == nullptr)
	{
		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), ao::macro::pointer_cast<void**>(factory.ToCreator()));
#ifdef __AO_DEBUG__
		if (FAILED(hr))
		{
			ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Adapter::Create]\nFailed create DXGIFactory.\n"));
			return hr;
		}
#endif
	}

	//�f�t�H���g�̃A�_�v�^�[�擾
	hr = this->factory->EnumAdapters(0, this->inst.ToCreator());
#ifdef __AO_DEBUG__
	if (FAILED(hr))
	{
		ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[DirectX11Adapter::Create]Faield enum adapters.\n"));
		return hr;
	}
#endif

	return hr;
}