/*************************************************************
*	@file   	DirectX11EnumDisplay.hpp
*	@brief  	DirectX11EnumDisplay�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/06/27
*************************************************************/

#ifndef _Include_DirectX11EnumDisplay_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11EnumDisplay_hpp_

//------------------------------------------------------------
// �C���N���[�h
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
	//	@brief  �f�B�X�v���C�񋓃N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/06/27
	------------------------------------------------------------*/
	class DirectX11EnumDisplay : private NonCopyable
	{
	public:
		DirectX11EnumDisplay();
		~DirectX11EnumDisplay();

		/*!-----------------------------------------------------------
		//	@brief		�f�B�X�v���C��
		//	@param[in]  _pAdapter	�A�_�v�^�[�|�C���^
		//	@param[in]  _getFormat	�擾����t�H�[�}�b�g
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		HRESULT EnumDisplay(IDXGIAdapter* _pAdapter, DXGI_FORMAT _getFormat);

		/*!-----------------------------------------------------------
		//	@brief		�w��̃f�B�X�v���C�T�C�Y���擾
		//	@param[in]	_nWidth		�w�肷�鉡�T�C�Y
		//	@param[in]  _nHeight	�w�肷��c�T�C�Y
		//	@return		�f�B�X�v���C�̏��|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/27
		------------------------------------------------------------*/
		DXGI_MODE_DESC*	GetEnumDisplayDesc(
			DXGI_MODE_DESC* _pOut,
			const UINT _nWidth,
			const UINT _nHeight);

		/*!-----------------------------------------------------------
		//	@brief		�񋓂����f�B�X�v���C�����擾
		//	@return		�񋓂����f�B�X�v���C��
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
