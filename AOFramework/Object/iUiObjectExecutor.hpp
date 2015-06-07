/*************************************************************
*	@file   	iUiObjectExecutor.hpp
*	@brief  	iUiObjectExecutor�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/03/29
*************************************************************/

#ifndef _Include_iUiObjectExecutor_hpp_	// �C���N���[�h�K�[�h
#define _Include_iUiObjectExecutor_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include"../Api/DirectX11GraphicsState.hpp"


namespace ao
{
	struct UiInstance;

	/*!-----------------------------------------------------------
	//	@class  iUiObjectExecutor
	//	@brief  UI�C���X�^���X�̎��s�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2015/03/29
	------------------------------------------------------------*/
	class iUiObjectExecutor
	{
	public:
		iUiObjectExecutor(UiInstance* _pInstance);
		~iUiObjectExecutor();

		/*!-----------------------------------------------------------
		//	@brief		�C���X�^���X�̃����_�����O���s
		//	@return		_blendState	�u�����h�X�e�[�g[�ȗ���]
		//	@param[in]	_fillState	�`�惂�[�h[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		HRESULT ExecuteCommandList(
			const BlendState _blendState = BlendState::Alpha,
			const FillState _fillState = FillState::Solid);

	private:
		UiInstance* pInstance;
	};
}

#endif // _Include_iUiObjectExecutor_hpp_